#include <fstream>
#include <codecvt>

int main() {
    
    std::wstring test = L"��ſ켼��?";
    std::string filepath = "file.txt";
    std::wofstream wof(filepath, std::ios::binary);
    wof.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
    // wof.open(L"file.txt");
    wof << L"\n�ȳ��ϼ���\n";
    wof << L"This is a test.";
    wof << L"This is another test.";
    wof << L"\nThis is the final test.\n";
    wof << test;
    wof.close();
    // file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>()));
    return 0;
}