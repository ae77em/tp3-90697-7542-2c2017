#include "client_BinaryDataFile.h"
#include "common_Command.h"
#include "common_Socket.h"

#include "client_main.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[]){
    try {
        if (argc != 4){
            throw std::string("Cantidad de parámetros incorrecta.");
        }

        std::string host = std::string(argv[1]);
        uint16_t port = atoi(argv[2]);
        std::string filename = std::string(argv[3]);

        BinaryDataFile file(filename);

        Socket client;
        client.connect(host.c_str(), port);

        int lenght;
        char response[MAX_BUFFER_SIZE] = {0};
        std::string formatted_command;

        while (file.has_data()){
            try {
                file.read();

                Command command(file.get_file_registry());

                formatted_command = command.get_formatted_command();

                client.send(formatted_command.c_str(), command.get_size());

                client.receive(response, 1);
                response[1] = '\0';

                std::cout << std::string(response); // imprimo codigo de op

                lenght = 5; // tamaño del mensaje de error...
                if (response[0] != 'E'){
                    lenght = Command::get_size_of_response(response[0]) - 1;
                }

                client.receive(response, lenght);
                response[lenght] = '\0';

                std::cout << std::string(response) << std::endl; // imprimo data
            } catch (std::string ex) {
                std::cerr << ex << std::endl;
            }
        }
    } catch(std::string ex) {
        std::cerr << ex << std::endl;
    } catch(...) {
        std::cerr << "Ocurrió un error desconocido en el cliente.";
    }

    return EXIT_SUCCESS;
}
