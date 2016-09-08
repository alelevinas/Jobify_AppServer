#include <iostream>
#include <Server.h>
#include "src/Foo.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Foo f;

    f.hola();


    Server s;
    s.start();
    return 0;
}
