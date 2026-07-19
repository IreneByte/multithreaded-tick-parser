#include "io.h"

using namespace std;

// Loads valid trades from a CSV file into a list
vector<Trade> loadTrades(const string& filename) {
    vector<Trade> trades;
    int skippedCount = 0;
    ifstream file(filename);
    ofstream errorLog("errors.csv");

    if (!file.is_open()) {
        // Exit early if the file is missing or corrupted
        throw runtime_error("Error: Could not open file!");
    }

    string line;
    
    // Loop through the file and separate good rows from bad rows
    while(getline(file, line)) {
        Trade tempTrade = parseLine(line);
        // A blank ticker means parseLine failed on this row, so only keep valid trades
        if (tempTrade.ticker != "") {
            trades.push_back(tempTrade);
        } else {
            // Count skipped invalid rows
            skippedCount++;
            // Write invalid rows to a file
            errorLog << line << endl;
        }
    }

    cout << "Skipped " << skippedCount << " malformed rows." << endl;
    this_thread::sleep_for(chrono::seconds(5)); // Display for 5 seconds

    return trades;
}
