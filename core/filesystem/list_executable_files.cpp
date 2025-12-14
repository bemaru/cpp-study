#include <ranges>
#include <filesystem>
#include <iostream>
#include <string> 

int main()
{
    std::string input_path{"C:\\Windows"};
    // std::cout << "Enter the directory path: ";
    // std::getline(std::cin, input_path);

    std::filesystem::path path(input_path);

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        std::cerr << "Invalid directory path!" << std::endl;
        return 1;
    }

    auto executable_files = std::filesystem::recursive_directory_iterator(path)
        | std::views::filter([](const std::filesystem::directory_entry& entry) {
        return entry.is_regular_file() && entry.path().extension() == ".exe";
            });

    std::cout << "Executable files in " << path << ":" << std::endl;

    for (const auto& entry : executable_files) {
        std::cout << entry.path() << std::endl;
    }

    return 0;
}
