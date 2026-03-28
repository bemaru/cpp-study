/**
 * std::string::npos와 find()
 *
 * - npos는 std::string::find()에서 문자열을 찾지 못했을 때 반환되는 값
 * - npos == std::numeric_limits<std::size_t>::max() (size_t의 최대값)
 * - 32비트/64비트에 따라 실제 값이 다르므로 직접 숫자 비교하지 말 것
 */

#include <iostream>
#include <string>
#include <limits>

int main()
{
    std::string text = "Hello, World!";

    // find(): 찾으면 위치 반환, 못 찾으면 npos 반환
    std::size_t found = text.find("World");
    if (found != std::string::npos) {
        std::cout << "'World' found at position: " << found << std::endl;  // 7
    }

    // 못 찾는 경우
    found = text.find("C++");
    if (found == std::string::npos) {
        std::cout << "'C++' not found" << std::endl;
    }

    // npos의 실제 값 (아키텍처에 따라 다름)
    std::cout << "npos:       " << std::string::npos << std::endl;
    std::cout << "size_t max: " << std::numeric_limits<std::size_t>::max() << std::endl;

    return 0;
}
