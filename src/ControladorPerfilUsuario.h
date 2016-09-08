//
// Created by ale on 8/09/16.
//

#ifndef JOBIFY_APPSERVER_CONTROLADORPERFILUSUARIO_H
#define JOBIFY_APPSERVER_CONTROLADORPERFILUSUARIO_H


#include "Controlador.h"

class ControladorPerfilUsuario {//: public Controlador {

    std::string uri;

public:
    ControladorPerfilUsuario() {}

    virtual ~ControladorPerfilUsuario() { }

    std::string http_get(std::string);
    std::string http_put(std::string);
    std::string http_del(std::string);
    std::string http_update(std::string);

};


#endif //JOBIFY_APPSERVER_CONTROLADORPERFILUSUARIO_H
