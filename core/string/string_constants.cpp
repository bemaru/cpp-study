#include <string>
#include <vector>

class StringConstants {
public:
	// 1. static const를 사용한 선언
	// 이 상수는 모든 인스턴스에서 공유됩니다.
	static const std::string greeting_message;

	// 2. constexpr를 사용한 선언
	// 컴파일 타임에 평가되는 상수입니다.
	static constexpr const char* goodbye_message = "Goodbye, World!";

	// 3. inline을 사용한 선언 및 정의
	// 선언과 정의를 동시에 하며, 중복 정의를 방지합니다.
	static inline const std::string error_message = "An error has occurred.";
	static inline const std::vector<std::string> fruit_list = { "apple", "banna", "cherry" };

	// 4. consteval을 사용한 메서드
	// 컴파일 타임에 평가되는 메서드입니다.
	static consteval const char* get_dynamic_greeting() {
		return "Hello, from a dynamic context!";
	}
};

// 1. static const에 대한 정의
const std::string StringConstants::greeting_message = "Hello, World!";

// 사용 예시
#include <iostream>

int main() {
	std::cout << StringConstants::greeting_message << std::endl; // "Hello, World!"
	std::cout << StringConstants::goodbye_message << std::endl; // "Goodbye, World!"
	std::cout << StringConstants::error_message << std::endl; // "An error has occurred."
	std::cout << StringConstants::get_dynamic_greeting() << std::endl; // "Hello, from a dynamic context!"

	return 0;
}
