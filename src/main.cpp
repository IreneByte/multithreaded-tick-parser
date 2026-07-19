// Real-Time Market Data Ingestion Engine
// Multithreaded C++ parser for stock trade data (CSV).
// Computes per-ticker VWAP, volume, and price range analytics.
// Author: Irene R.

#include <iostream> // Printing to console
#include <chrono> // Single vs multi-threaded benchmark
#include <thread> // Multiple threads
#include <iomanip> // Formatting decimals
#include "trade.h"
#include "metrics.h"
#include "threading.h"
#include "io.h"

using namespace std;

// To check if adding more threads will actually make the program faster
void runBenchmark(const vector<Trade>& trades) {
    double baselineTime, speedup;

    // Run each benchmark test for different thread counts
    for (int n : {1, 2, 4, 8}) {
        // Start timing the clock to get elapsed time
        auto start = chrono::high_resolution_clock::now();
        runMultithreaded(trades, n);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        // Save the single thread time to use it as a baseline to compare to other thread count times
        baselineTime = (n == 1) ? elapsed.count() : baselineTime;
        speedup = (n != 1) ? baselineTime / elapsed.count() : 1.00; // Calculate the speedup ratio

        // Print the time taken for each thread's test and the speedup ratio to 3 decimal places
        cout << "Threads: " << n << ", Time: " << fixed << setprecision(3) << elapsed.count() << " seconds, Speedup: " << speedup << "x" << endl;
    }
}

// Refresh the table on the screen to show larger slices of data processed over time
void runDashboard(const vector<Trade>& trades, int numWorkers) {
    int i = 1;

    while (true) {
        // Increase the data being processed by 10% each loop iteration
        int end = trades.size() * i / 10;

        // Stop the end index if it goes past the amount of trades correctly loaded in
        if (end > trades.size()) {
            end = trades.size();
        }

        vector<Trade> partialData(trades.begin(), trades.begin() + end);
        unordered_map<string, CompanyMetrics> merged = runMultithreaded(partialData, numWorkers);

        // Clear the screen to draw a new table each time
        system("cls");
        printReport(merged);

        // Pause so that the screen doesn't flicker too fast
        this_thread::sleep_for(chrono::milliseconds(500));
   
        i++;
    }
}

// Runs the program from start to finish
int main()
{
    vector<Trade> trades = loadTrades("trades.csv");
    runDashboard(trades, 4);
}