/**
 * @file timestamp_milliseconds.cpp
 * @brief Generate timestamp strings with millisecond precision
 *
 * This example demonstrates how to create formatted timestamp strings
 * including milliseconds using std::chrono and std::put_time.
 *
 * Output format: "YYYY-MM-DD HH:MM:SS.mmm"
 * Example: "2025-01-14 15:30:45.123"
 */

#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <chrono>
#include <time.h>
#include <iomanip>
#include <iostream>

/**
 * Generate a formatted timestamp string with millisecond precision
 * @return stringstream containing the formatted timestamp
 */
std::stringstream Timestamp()
{
    // Get current time point
    auto now = std::chrono::system_clock::now();

    // Convert to time_t for formatting with std::put_time
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    // Extract milliseconds from epoch
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    // Format: YYYY-MM-DD HH:MM:SS.mmm
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%F %T.")
       << std::setfill('0') << std::setw(3) << ms % 1000;
    return ss;
}

int main()
{
    std::cout << Timestamp().str() << std::endl;
    return 0;
}