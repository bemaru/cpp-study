#include <iostream>

class First {
public:
    virtual ~First() {
        std::cout << "~First()" << std::endl;
    };
};

class Second : public First {
public:
    ~Second() {
        std::cout << "~Second()" << std::endl;
    }
};

class Third : public Second {
public:
    ~Third() {
        std::cout << "~Third()" << std::endl;
    }
};


int main(const int argc, const char* const argv[]) {
    First* ptr = new Third();

    delete ptr;

    return 0;
}