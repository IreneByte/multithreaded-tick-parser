#pragma once
#include <vector> // List to hold parsed trades
#include <thread> // Multiple threads
#include "trade.h"
#include "metrics.h"

// Splits a big list of trades into smaller parts
std::vector<std::vector<Trade>> partitionTrades(const std::vector<Trade>& trades, int numThreads);

// Process each partition in a multi-threaded process
void processPartition(std::vector<Trade>& trades, std::unordered_map<std::string, CompanyMetrics>& result);

// Put al thread results back together in one unordered map
std::unordered_map<std::string, CompanyMetrics> runMultithreaded(const std::vector<Trade>& trades, int numWorkers);