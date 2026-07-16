// Real-Time Market Data Ingestion Engine
// Multithreaded C++ parser for stock trade data (CSV).
// Computes per-ticker VWAP, volume, and price range analytics.
// Author: Irene R.

#include <iostream> // Printing to console
#include <string> // String text handling
#include <sstream> // String splitting
#include <fstream> // Reading .csv file(s)
#include <vector> // List to hold parsed trades

using namespace std;

struct Trade {
    std::string ticker;
    double price;
    long volume; // long covers massive volume sizes without overflow issues
    std::string timestamp; // Expected format: YYYY-MM-DDTHH:MM:SS
};

// Parses a single CSV line into a Trade struct
Trade parseLine(string inputString) {
    Trade trade;
    stringstream ss(inputString);
    string field1, field2, field3, field4;

    getline(ss, field1, ',');
    trade.ticker = field1;

    getline(ss, field2, ',');
    trade.price = stod(field2); // stod will throw an error if data is corrupted

    getline(ss, field3, ',');
    trade.volume = stol(field3); // stol will throw an error if data is corrupted

    getline(ss, field4, ',');
    trade.timestamp = field4;

    return trade;
}

int main()
{
    string filename = "trades.csv";
    ifstream file(filename);

    if (!file.is_open()) {
        // Exit early if the file is missing or corrupted
        cout << "Error: Could not open file!" << endl;
        return 1;

    } else {
        vector<Trade> trades;
        string line;
        
        // Read the file line-by-line until the end
        while(getline(file, line)) {
            Trade tempTrade = parseLine(line);
            trades.push_back(tempTrade);
        }

        // const auto& avoids copying the structs in memory on every loop iteration
        for (const auto& t : trades) { 
            cout << "Ticker: " << t.ticker << ", Price: " << t.price << ", Volume: " << t.volume << ", Timestamp: " << t.timestamp << endl;
        }
    }
}