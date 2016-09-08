//
// Created by ale on 7/09/16.
//

#include <iostream>
#include "Server.h"
#include "Controlador.h"
#include "ControladorPerfilUsuario.h"

/*
static int s_sig_num = 0;

static void signal_handler(int sig_num) {
    signal(sig_num, signal_handler);
    s_sig_num = sig_num;
}
*/
static int has_prefix(const struct mg_str *uri, const struct mg_str *prefix) {
    return uri->len > prefix->len && memcmp(uri->p, prefix->p, prefix->len) == 0;
}

static int is_equal(const struct mg_str *s1, const struct mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}



static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct mg_str s_get_method = MG_MK_STR("GET");
    struct mg_str s_put_method = MG_MK_STR("PUT");
    struct mg_str s_delele_method = MG_MK_STR("DELETE");
    static struct mg_serve_http_opts s_http_server_opts;

    struct http_message *hm = (struct http_message *) ev_data;

    Server* server = (Server*) nc->user_data;
    server->f();
    server->_handle_request(nc, ev, hm);


}

void Server::_handle_request(mg_connection *nc, int ev, http_message *hm) {
    std::cerr << "HANDLEO LA REQUEST\n";
    string response = "japon";
    string body;
    string uri;
    string method;

    //std::map<string, ControladorPerfilUsuario*>::iterator it;
    //it = controladores.find(method);

    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            std::cerr << "Is an http request." << std::endl;
            body.assign(hm->body.p, hm->body.len);
            uri.assign(hm->uri.p,hm->uri.len);
            method.assign(hm->method.p,hm->method.len);

            std::cerr << "\nMETHOD: " << method;
            std::cerr << "\nURI: " << uri;
            std::cerr << "\nBODY: " << body;

            if (method.compare("GET") == 0) {
                response = controladores[uri]->http_get(body); //o it->second.
            } else if (method.compare("PUT") == 0) {
                std::cerr << "\nENTRO EN PUT";
                std::map<string, ControladorPerfilUsuario*>::iterator it = controladores.find(uri);
                response = it->second->http_put(body);
                //response = controladores[uri]->http_put(body);
            } else if (method.compare("DELETE") == 0) {
                response = controladores[uri]->http_del(body);
            } else if (method.compare("UPDATE") == 0) {
                response = controladores[uri]->http_update(body);
            } else {
                //mg_printf(nc, "%s", "HTTP/1.0 501 Not Implemented\r\n");
            }
        default:
            break;
    }
    std::cerr << "La respuesta es: " << response << std::endl;
}

bool Server::start() {
    // int main(int argc, char *argv[]) {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    int i;

    /* Open listening socket */
    mg_mgr_init(&mgr, NULL);

    mgr.user_data = this;


    nc = mg_bind(&mgr, s_http_port.c_str(), ev_handler);
    mg_set_protocol_http_websocket(nc);
//    s_http_server_opts.document_root = "web_root";

    /* For each new connection, execute ev_handler in a separate thread */
  //  mg_enable_multithreading(nc);

    /* Parse command line arguments */
    /*
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0) {
            mgr.hexdump_file = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0) {
            s_db_path = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0) {
            s_http_server_opts.document_root = argv[++i];
        }
    }*/

    //signal(SIGINT, signal_handler);
    //signal(SIGTERM, signal_handler);

    /* Open database */
    /*if ((s_db_handle = db_open(s_db_path)) == NULL) {
        fprintf(stderr, "Cannot open DB [%s]\n", s_db_path);
        exit(EXIT_FAILURE);
    }*/

    /* Run event loop until signal is received */
    printf("Starting RESTful multi-threaded server on port %s\n", s_http_port.c_str());
    while (true) { // s_sig_num == 0) {
        mg_mgr_poll(&mgr, 1000);
    }

    /* Cleanup */
    mg_mgr_free(&mgr);
    // db_close(&s_db_handle);

    //printf("Exiting on signal %d\n", s_sig_num);

    return true;
}

Server::Server() {
    ControladorPerfilUsuario *c1 = new ControladorPerfilUsuario();
    controladores[string("/usr")] = c1;
}

Server::~Server() {
    ControladorPerfilUsuario *c1 = controladores["/usr"];
    delete c1;
}

void Server::f() {
    std::cout << "\n";
}



