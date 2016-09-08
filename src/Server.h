//
// Created by ale on 7/09/16.
//

#ifndef JOBIFY_APPSERVER_SERVER_H
#define JOBIFY_APPSERVER_SERVER_H

#include <string>
#include <map>
#include "mongoose/mongoose.h"
#include "Controlador.h"
#include "ControladorPerfilUsuario.h"

using std::string;

class Server {
public:
    Server();

private:

    //DatabaseManager db_manager;
    std::map<string,ControladorPerfilUsuario*> controladores;

    string s_http_port = "8000";
    static struct mg_serve_http_opts s_http_server_opts;
    //static int s_sig_num = 0;
    void* s_db_handle = NULL;
    struct mg_str s_get_method = MG_MK_STR("GET");
    struct mg_str s_put_method = MG_MK_STR("PUT");
    struct mg_str s_delele_method = MG_MK_STR("DELETE");

 //   void ev_handler(mg_connection *nc, int ev, void *ev_data);

//    int is_equal(const mg_str *s1, const mg_str *s2);
//
//    int has_prefix(const mg_str *uri, const mg_str *prefix);

 //   void signal_handler(int sig_num);

public:
    bool start();

    void f();

    void _handle_request(mg_connection *pConnection, int pMessage, http_message *pHttp_message);

    virtual ~Server();
};


#endif //JOBIFY_APPSERVER_SERVER_H
