//
// Created by ale on 31/08/16.
//

#include <iostream>
#include <string>
#include "Foo.h"

using std::string;

std::string Foo::hola() {
    std::cout << "Hola desde foo" << std::endl;
    string a = "Hola desde foo";
    return a;
}
