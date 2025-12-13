/**
 * @file virtual_destructor.cpp
 * @brief Demonstrates the importance of virtual destructors in inheritance
 *
 * This example shows what happens when deleting a derived class object
 * through a base class pointer. Without a virtual destructor in the base
 * class, only the base class destructor is called, leading to memory leaks.
 *
 * Key learning:
 * - The base class First has a virtual destructor (line 27)
 * - When deleting a Second object through a First pointer, both destructors are called
 * - If First's destructor wasn't virtual, only ~First() would be called,
 *   causing strTwo to leak memory
 */

#include <iostream>
#include <cstring>

#pragma warning(disable: 4996)

using std::cout;
using std::cin;
using std::endl;

class First
{
private:
	char* strOne;

public:
	explicit First(const char* str)
	{
		cout << "First" << endl;
		strOne = new char[strlen(str) + 1];
	}
	virtual ~First()
	{
		cout << "~First()" << endl;
		delete[] strOne;
	}
};

class Second : public First
{
private:
	char* strTwo;

public:
	explicit Second(const char* str1, const char* str2) : First(str1)
	{
		cout << "Second" << endl;
		strTwo = new char[strlen(str2) + 1];
	}
	~Second()
	{
		cout << "~Second()" << endl;
		delete[] strTwo;
	}
};

int main(const int argc, const char* const argv[])
{
	First* ptr = new Second("aaa", "bbb");
	delete ptr;
	return 0;
}

/* 결과

First
Second
~First()

*/