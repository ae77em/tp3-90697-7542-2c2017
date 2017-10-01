#include "client_BinaryDataFile.h"
#include "common_Command.h"
#include "common_Socket.h"
#include "common_Structs.h"

#include <cstdlib>
#include <iostream>
#include <string>

int client_main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;

    try {
        if (argc != 5){
            throw std::string("Cantidad de parámetros incorrecta.");
        }

        std::string filename = std::string(argv[4]);
        std::string formatted_command;

        BinaryDataFile file(filename);

        std::string host = std::string(argv[2]);
        uint16_t port = atoi(argv[3]);

        Socket client;
        client.connect(host.c_str(), port);

        int lenght;
        char response[MAX_BUFFER_SIZE] = {0};

        while (!file.eof() && file.good()){
            try {
                file.read();

                Command command(file.get_file_registry());

                formatted_command = command.get_formatted_command();

                client.send(formatted_command.c_str(), command.get_size()+1);

                client.receive(response, 1);
                response[1] = '\0';

                std::cout << std::string(response); // imprimo codigo de op

                lenght = 5; // tamaño del mensaje de error...
                if (response[0] != 'E'){
                    lenght = Command::get_size_of(response[0]);
                }
                
                client.receive(response, lenght);
                response[lenght] = '\0';

                std::cout << std::string(response) << std::endl; // imprimo data
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
