#include "client_BinaryDataFile.h"
#include "common_Command.h"
#include "common_Socket.h"

#include <cstdlib>
#include <iostream>


int main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;

    try {
        if (argc != 4){
            throw "Cantidad de parámetros incorrecta.";
        }

        std::string filename = std::string(argv[3]);
        std::string formatted_command;

        BinaryDataFile file(filename);

        std::string host = std::string(argv[1]);
        uint16_t port = atoi(argv[2]);

        Socket socket;
        socket.connect(host.c_str(), port);

        while (file.good()){
            try {
                file.read();
                file.get_file_registry();

                Command command(file.get_file_registry());

                formatted_command = command.get_formatted_command();

                socket.send(formatted_command.c_str(), command.get_size());
            } catch (std::string ex) {
                std::cerr << ex;
                status = EXIT_FAILURE;
            }
        }
    } catch(std::string ex) {
        std::cerr << ex;
        status = EXIT_FAILURE;
    } catch(...) {
        std::cerr << "Ocurrió un error desconocido en el cliente.";
        status = EXIT_FAILURE;
    }

    return status;
}
