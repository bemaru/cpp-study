/**
 * VLA (Variable Length Array)
 *
 * - C99에서 표준으로 도입, C11부터는 선택적 지원
 * - C++에서는 표준이 아님 (일부 컴파일러가 확장으로 지원하지만 비권장)
 * - C++에서 런타임 크기 배열이 필요하면 std::vector 또는 동적 할당 사용
 */

#include <iostream>
#include <vector>

int main()
{
    // 컴파일 타임 상수 → 일반 배열 OK
    const int fixed_size = 10;
    int fixed_array[fixed_size];

    // 런타임 크기 → C++에서는 VLA 대신 std::vector
    int runtime_size = 10;
    // int vla[runtime_size];  // 비표준! GCC 확장으로는 동작하지만 이식성 없음
    std::vector<int> dynamic_array(runtime_size);

    std::cout << "fixed_array size: " << sizeof(fixed_array) / sizeof(int) << std::endl;
    std::cout << "dynamic_array size: " << dynamic_array.size() << std::endl;

    return 0;
}
