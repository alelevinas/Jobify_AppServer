//
// Created by ale on 31/08/16.
//

#include <iostream>
#include "Foo.h"

using std::string;

std::string Foo::hola() {
    string a = "Hola desde foo\n";
    std::cout << a << std::endl;
    return a;
}
