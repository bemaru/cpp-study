All templated code should be in header files. You are trying to compile template in separate cpp file, but compiler does not know which versions of function it should generate.

템플릿 코드는 헤더 파일에 정의해야 합니다. 템플릿은 컴파일 시점에 구체적인 타입을 알아야 하므로, 이를 별도의 .cpp 파일에 정의하면 컴파일러가 어떤 타입에 대한 템플릿을 생성해야 할지 알 수 없습니다. 따라서 템플릿 코드도 헤더 파일에 포함되어야 합니다.