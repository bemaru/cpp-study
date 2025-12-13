#include <iostream>
#include <json/json.h>
#include <sstream>
#include <stdexcept>

int main() {
    std::string jsonString = R"({
        "intValue": 42,
        "floatValue": 3.14,
        "stringValue": "Hello",
        "boolValue": true,
    })";

    Json::CharReaderBuilder reader;
    Json::Value jsonData;
    std::istringstream jsonStream(jsonString);
    std::string errors;

    if (!Json::parseFromStream(reader, jsonStream, &jsonData, &errors)) {
        std::cerr << "Failed to parse JSON: " << errors << std::endl;
        return 1;
    }

    // valid conversions
    std::cout << "intValue: " << jsonData["intValue"].asInt() << std::endl;
    std::cout << "floatValue: " << jsonData["floatValue"].asDouble() << std::endl;
    std::cout << "stringValue: " << jsonData["stringValue"].asString() << std::endl;
    std::cout << "stringValue: " << jsonData["stringValue"].asString() << std::endl;
    std::cout << "boolValue: " << jsonData["boolValue"].asBool() << std::endl;


    // invalid conversions
    try {
        std::cout << jsonData["stringValue"].asBool() << std::endl;
    }
    catch (const std::exception& e) {
		std::cerr << "Exception caught while converting: " << e.what() << std::endl;
	}
    
    try {
        std::cout << jsonData["stringValue"].asDouble() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught while converting: " << e.what() << std::endl;
    }

    try {
        std::cout << jsonData["stringValue"].asFloat() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught while converting: " << e.what() << std::endl;
    }
    
    try {
        std::cout << jsonData["stringValue"].asInt() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught while converting: " << e.what() << std::endl;
    }

    return 0;
}
