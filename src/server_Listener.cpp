#include "server_Listener.h"
#include "common_Socket.h"
#include "server_ShuttingDownHandler.h"
#include "common_Exceptions.h"

#include <vector>
#include <cstring>
#include <iostream>

Listener::Listener(uint16_t port) {
    server.bind(port);
    server.listen();
}

Listener::Listener(const Listener& orig) { }

Listener::~Listener() { }

void Listener::shutdown(){
    std::cout << "entre a shutdown de listener..." << std::endl;
    int size = clients.size();
    for (int i = 0; i < size; ++i){
        clients[i]->shutdown();
        delete clients[i];
    }
    std::cout << "borre clientes de listener..." << std::endl;
    size = threads.size();
    for (int i = 0; i < size; ++i){
        threads[i]->join();
        delete threads[i];
    }
    std::cout << "borre threads de listener..." << std::endl;
    server.shutdown();
    std::cout << "hice shutdown de server listener..." << std::endl;
}

void Listener::run(){
    try {
        while(true){
            int fd = server.accept();
            Socket *client = new Socket(fd);
            ClientRequestHandler *rp = new ClientRequestHandler(*client);

            clients.push_back(client);
            threads.push_back(rp);

            rp->start();
        }
    } catch (accept_exception ex) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún requesto, para evitar problemas...
         * no hago nada...
         */
    }
}