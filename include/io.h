#pragma once
#include <iostream> // Printing to console
#include <string> // String text handling
#include <fstream> // Reading .csv file(s)
#include <vector> // List to hold parsed trades
#include <thread> // Multiple threads
#include <chrono> // Single vs multi-threaded benchmark
#include "trade.h"

// Loads valid trades from a CSV file into a list
std::vector<Trade> loadTrades(const std::string& filename);