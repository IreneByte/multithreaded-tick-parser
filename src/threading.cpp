#include "threading.h"

using namespace std;

// Splits data into equal parts for the threads
vector<vector<Trade>> partitionTrades(const vector<Trade>& trades, int numThreads) {
    vector<vector<Trade>> threads;
    int threadSize = trades.size() / numThreads; // How many trades processed in each thread

    for (int i = 0; i < numThreads; i++) {
        int start = i * threadSize; 
        int end = (i * threadSize) + threadSize;

        // Make sure the last thread takes any leftover rows
        if (trades.size() - end <= threadSize) {
            end = trades.size();
        }

        vector<Trade> partition(trades.begin() + start, trades.begin() + end); // Partition into chunks
        threads.push_back(partition); // Put each partition into the big map
    }

    return threads;
}

// Runs calculations on one assigned partition
void processPartition(vector<Trade>& trades, unordered_map<string, CompanyMetrics>& result) {
    result = computeMetrics(trades);
}

unordered_map<string, CompanyMetrics> runMultithreaded(const vector<Trade>& trades, int numWorkers) {
    vector<vector<Trade>> partitions = partitionTrades(trades, numWorkers);
    vector<unordered_map<string, CompanyMetrics>> partition_results(numWorkers);

    vector<thread> workers;
    for (int i = 0; i < numWorkers; i++) {
        // Pass references directly to prevent copying data
        workers.push_back(thread(processPartition, ref(partitions[i]), ref(partition_results[i])));
    }

    for (thread& w : workers) {
        // Joins background thread execution back into main
        w.join();
    }

    // Merge the results of the workers
    return mergeResults(partition_results, numWorkers);
}
