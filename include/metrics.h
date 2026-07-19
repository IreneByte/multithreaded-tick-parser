#pragma once
#include <iostream> // Printing to console
#include <string> // String text handling
#include <vector> // List to hold parsed trades
#include <unordered_map> // Key-value lookups
#include <algorithm> // Min and max
#include <iomanip> // Formatting decimals
#include "trade.h"

// Holds calculated metrics for a stock ticker
struct CompanyMetrics {
    double minPrice = -1.0; 
    double maxPrice = -1.0; // -1.0 is a flag meaning "no trades seen yet"
    long totalVolume = 0; 
    double weightedPriceSum = 0.0; // Cumulative (Price * Volume)
};

// Calculates stock stats from a list of trades
std::unordered_map<std::string, CompanyMetrics> computeMetrics(const std::vector<Trade>& trades);

// Combines worker maps into one map
std::unordered_map<std::string, CompanyMetrics> mergeResults(std::vector<std::unordered_map<std::string, CompanyMetrics>> partitions, int numWorkers);

// Prints a formatted data table in the terminal
void printReport(const std::unordered_map<std::string, CompanyMetrics>& statsMap);