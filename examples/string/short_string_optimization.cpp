/**
 * @file short_string_optimization.cpp
 * @brief Implementation of Short String Optimization (SSO)
 *
 * SSO is a technique used by std::string implementations to avoid heap
 * allocation for short strings. This example demonstrates how to implement
 * a simple string class with SSO.
 *
 * Key concepts:
 * - Strings <= 15 characters: stored in stack-allocated buffer (no heap allocation)
 * - Strings > 15 characters: stored in heap-allocated memory
 * - Uses union to save memory (only one storage method is active at a time)
 *
 * Benefits:
 * - Faster for short strings (no heap allocation overhead)
 * - Better cache locality
 * - Reduced memory fragmentation
 */

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <utility>

class ShortString {
public:
    // Maximum length for SSO (Short String Optimization)
    static const size_t SSO_MAX_LENGTH = 15;

    ShortString() : is_small(true), length(0) {
        data.small[0] = '\0'; // Initialize as empty string
    }

    ShortString(const char* str) {
        size_t len = std::strlen(str);
        if (len < SSO_MAX_LENGTH) {
            is_small = true;
            length = len;
            std::strcpy(data.small, str); // Store in stack buffer
        } else {
            is_small = false;
            length = len;
            data.large = new char[len + 1]; // Allocate on heap
            std::strcpy(data.large, str);
        }
    }

    ShortString(const ShortString& other) {
        length = other.length;
        is_small = other.is_small;
        if (is_small) {
            std::strcpy(data.small, other.data.small);
        } else {
            data.large = new char[length + 1];
            std::strcpy(data.large, other.data.large);
        }
    }

    ShortString& operator=(const ShortString& other) {
        if (this != &other) {
            if (!is_small && data.large) {
                delete[] data.large; // Free existing heap memory
            }
            length = other.length;
            is_small = other.is_small;
            if (is_small) {
                std::strcpy(data.small, other.data.small);
            } else {
                data.large = new char[length + 1];
                std::strcpy(data.large, other.data.large);
            }
        }
        return *this;
    }

    ~ShortString() {
        if (!is_small) {
            delete[] data.large; // Free heap memory if allocated
        }
    }

    const char* c_str() const {
        return is_small ? data.small : data.large;
    }

    size_t size() const {
        return length;
    }

private:
    union Data {
        char small[SSO_MAX_LENGTH + 1]; // Stack buffer for small strings
        char* large; // Heap pointer for large strings
        Data() : large(nullptr) {} // Initialize as nullptr
    } data;

    bool is_small; // Flag: true if using SSO, false if heap-allocated
    size_t length; // String length
};

int main() {
    ShortString shortStr("Hello, World!"); // Uses SSO (13 chars)
    ShortString longStr("This is a very long string that exceeds the limit."); // Uses heap (51 chars)

    std::cout << "Short String: " << shortStr.c_str() << " (size: " << shortStr.size() << ")\n";
    std::cout << "Long String: " << longStr.c_str() << " (size: " << longStr.size() << ")\n";

    return 0;
}
