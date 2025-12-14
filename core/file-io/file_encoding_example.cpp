#include <fstream>
#include <codecvt>
#include <iostream>

int main() {

    std::cout << "File Encoding Example\n";
    std::wstring test = L"안녕하세요";
    std::string filepath = "file.txt";
    std::wofstream wof(filepath, std::ios::binary);
    wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
    wof << L"안녕하세요\n";
    wof << L"This is a test.";
    wof << L"This is another test.";
    wof << L"\nThis is the final test.\n";
    wof << test;
    wof.close();
    return 0;
}