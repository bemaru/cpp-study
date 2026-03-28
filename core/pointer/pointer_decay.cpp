/**
 * Array-to-Pointer Decay
 *
 * 배열을 함수에 전달하면 포인터로 변환(decay)되어 크기 정보를 잃는다.
 * 해결 방법:
 *   1. 크기를 별도 파라미터로 전달
 *   2. 템플릿 참조로 크기를 유지
 *   3. std::array 또는 std::span(C++20) 사용
 */

#include <iostream>
#include <array>

// 1. 포인터 + 크기 전달 (전통적 방식)
void print_with_size(const int* arr, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

// 2. 템플릿 참조로 크기 유지
template <size_t N>
void print_with_template(const int (&arr)[N])
{
    for (size_t i = 0; i < N; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

// 3. std::array 사용 (C++11)
void print_with_stdarray(const std::array<int, 5>& arr)
{
    for (const auto& elem : arr)
        std::cout << elem << " ";
    std::cout << std::endl;
}

int main()
{
    int raw_array[5] = {10, 20, 30, 40, 50};
    std::array<int, 5> std_array = {10, 20, 30, 40, 50};

    // decay 발생: raw_array → int* 변환, sizeof 정보 손실
    std::cout << "sizeof(raw_array) in main: " << sizeof(raw_array) << std::endl;  // 20
    // 함수 안에서는 sizeof(arr) == sizeof(int*) == 8

    std::cout << "1. pointer + size: ";
    print_with_size(raw_array, 5);

    std::cout << "2. template ref:   ";
    print_with_template(raw_array);

    std::cout << "3. std::array:     ";
    print_with_stdarray(std_array);

    return 0;
}
