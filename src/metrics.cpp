#include "metrics.h"

using namespace std;

// Calculates stock analytics from a collection of trades
unordered_map<string, CompanyMetrics> computeMetrics(const vector<Trade>& trades) {
    unordered_map<string, CompanyMetrics> statsMap;

    // Populating Company Metrics for each company
    // const auto& avoids copying the structs in memory on every loop iteration
    for (const Trade& t : trades) { 
        // Use & to get the address of the actual metrics in the map so they can be changed directly
        CompanyMetrics& stats = statsMap[t.ticker];

        // Add the current trade's volume and total weighted value to the running sums
        stats.totalVolume += t.volume;
        stats.weightedPriceSum += (t.volume * t.price);

        // Update high and low prices for this stock
        stats.minPrice = (stats.minPrice == -1.0) ? t.price : min(stats.minPrice, t.price);
        stats.maxPrice = (stats.maxPrice == -1.0) ? t.price : max(stats.maxPrice, t.price);
    }

    return statsMap;
}

// Merges all thread maps into one final map
unordered_map<string, CompanyMetrics> mergeResults(vector<unordered_map<string, CompanyMetrics>> partitions, int numWorkers) {
    unordered_map<string, CompanyMetrics> result;

    // Iterate through each partition in the split vector
    for (unordered_map<string, CompanyMetrics>& p : partitions) {
        // Iterate through each pair in each map
        for (const pair<string, CompanyMetrics> t : p) {
            string ticker = t.first;
            if (result.count(ticker) == 0) { // Add new aggregated stock if not in map
                result.insert(t);
            } else { // Combine totals if stock exists
                result[ticker].totalVolume += t.second.totalVolume;
                result[ticker].weightedPriceSum += t.second.weightedPriceSum;
                result[ticker].minPrice = min(t.second.minPrice, result[ticker].minPrice);
                result[ticker].maxPrice = max(t.second.maxPrice, result[ticker].maxPrice);
            }
        }
    }

    return result;
}

// Displays final calculated metrics to the screen
void printReport(const unordered_map<string, CompanyMetrics>& statsMap) {
    // Print the table header columns
    cout << left << setw(8) << "TICKER"
            << right << setw(12) << "VWAP"
            << setw(12) << "MIN"
            << setw(12) << "MAX"
            << setw(14) << "VOLUME" << endl;

    // Print final analytics for each unique company in the map
    for (const auto& pair : statsMap) {
        double vwap = 0.0;
        
        // VWAP = (Total Money Traded) / (Total Shares Traded)
        if (pair.second.totalVolume > 0) {
            vwap = pair.second.weightedPriceSum / pair.second.totalVolume;
        }

        cout << left  << setw(8)  << pair.first
             << right << setw(12) << vwap
             << setw(12) << pair.second.minPrice
             << setw(12) << pair.second.maxPrice
             << setw(14) << pair.second.totalVolume << endl;
    }
}
