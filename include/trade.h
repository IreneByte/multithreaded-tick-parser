#pragma once
#include <iostream> // Printing to console
#include <string> // String text handling
#include <sstream> // String splitting
#include <regex> // For pattern matching

// Holds data for a single trade record
struct Trade {
    std::string ticker;
    double price;
    long volume; // long covers massive volume sizes without overflow issues
    std::string timestamp; // Expected format: YYYY-MM-DDTHH:MM:SS
};

// Converts a text line into a trade object
Trade parseLine(std::string inputString);