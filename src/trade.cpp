#include "trade.h"

using namespace std;

// Parses a single CSV line into a Trade object
Trade parseLine(string inputString) {
    try {
        Trade trade;
        stringstream ss(inputString);
        string field1, field2, field3, field4;
        regex pattern(R"(^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}$)"); // Matches YYYY-MM-DDTHH:MM:SS format

        // If any field is missing, the row is malformed and unusable
        if( !getline(ss, field1, ',') || 
            !getline(ss, field2, ',') ||
            !getline(ss, field3, ',') || 
            !getline(ss, field4, ',')) 
        {
            throw runtime_error("Missing fields");
        }

        trade.ticker = field1;
        trade.price = stod(field2); // stod will throw an error if data is corrupted
        trade.volume = stol(field3); // stol will throw an error if data is corrupted
        trade.timestamp = field4;

        // If price is less than 0, or volume is less than 0, throw an error
        if (trade.price < 0 || trade.volume < 0) {
            throw invalid_argument("Negative price or volume");
        }

        if (trade.ticker.empty()) {
            throw invalid_argument("Ticker is empty");
        }

        if (trade.timestamp.length() != 19) {
            throw invalid_argument("Incorrect timestamp format");
        }

        if (!std::regex_match(trade.timestamp, pattern)) {
            throw std::invalid_argument("Malformed timestamp format");
        }

        return trade;
    } catch (const exception& e) {
        // Catch errors with e.what(), automatically outputting the reason for the error message
        return Trade{"", 0.0, 0, ""}; // The row failed the validation defined in this function, so return an empty Trade object so that it can later be detected and skipped
    }
}