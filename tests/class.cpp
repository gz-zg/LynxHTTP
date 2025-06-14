#include <iostream>

class A{
public:
    int a;
    double b;
};

class B{
public:
    B()
    {

    }
    int a;
    double b;
};

int main()
{
    A x;
    B y;
    std::cout << x.a << x.b << std::endl;
    std::cout << y.a << y.b << std::endl;

    return 0;
}