//
// Created by ale on 8/09/16.
//

#include <string>
#include <iostream>

#include "ControladorPerfilUsuario.h"


std::string ControladorPerfilUsuario::http_get(std::string) {
    return "HICE GET";
}

std::string ControladorPerfilUsuario::http_put(std::string) {
    std::cerr << "\nEJECUTO PUT";
    return std::string("HICE PUT");
}

std::string ControladorPerfilUsuario::http_del(std::string) {
    return "HICE DELETE";
}

std::string ControladorPerfilUsuario::http_update(std::string) {
    return "HICE UPDATE";
}


