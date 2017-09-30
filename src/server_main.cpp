#include <iostream>
#include <cstdlib>
#include <string>

#include "common_Socket.h"
#include "common_Command.h"

#define MAX_BUFFER_SIZE 30

int main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;

    try {
        if (argc != 2){
            throw "Cantidad de parámetros incorrecta.";
        }

        std::string data_received;
        char aux_data_received[MAX_BUFFER_SIZE] = {0};
        char op;
        uint16_t lenght;

        //BinaryDataFile file(filename);

        uint16_t port = atoi(argv[2]);

        Socket server;
        Socket client;

        server.bind_and_listen(port);
        server.accept(client);

        bool is_finished = false;
        while (!is_finished){
            server.receive(aux_data_received, 1);
            op = aux_data_received[0];

            lenght = Command::get_size_of(op);

            server.receive(aux_data_received, lenght);

            data_received = std::string(aux_data_received);

            //Command command()
        }

    } catch (std::string ex){
        std::cerr << ex;
        status = EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Ocurrió un error desconocido en el servidor.";
        status = EXIT_FAILURE;
    }

    return status;
}
