//
// Created by ale on 31/08/16.
//

#include <iostream>
#include "Foo.h"

std::string Foo::hola() {
    std::cout << "Hola desde foo" << std::endl;
    std::string a = "Hola desde foo";
    return a;
}
