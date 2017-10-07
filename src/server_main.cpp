#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "common_Socket.h"
#include "common_Command.h"
#include "server_RequestProccessor.h"

#include "server_main.h"

int server_main(int argc, char *argv[]){

    Command *command = new Command();

    try {
        if (argc != 3){
            throw std::string("Cantidad de parámetros incorrecta.");
        }

        uint16_t port = atoi(argv[2]);

        Socket server;

        server.bind(port);
        server.listen();

        std::vector<RequestProccessor*> threads;

        bool is_finished;
        char op[2];

        while(1){
            Socket *client = new Socket(-1);
            server.accept(*client);

            client->receive(op, 1);
            op[1] = '\0';

            is_finished = (op[0] == 'q');

            if (!is_finished){
                RequestProccessor *rp = new RequestProccessor(op[0], *client, command);
                threads.push_back(rp);

                rp->start();
            } else {
                for (RequestProccessor *thread : threads){
                    thread->join();
                    delete thread;
                }
                break;
            }
        }
    } catch (std::string ex) {
        std::cerr << ex;
    } catch (...) {
        std::cerr << "Ocurrió un error desconocido en el servidor.";
    }

    delete command;

    return EXIT_SUCCESS;
}
