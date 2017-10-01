#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "common_Socket.h"
#include "common_Command.h"

int main(int argc, char *argv[]){
    try {
        if (argc != 2){
            throw std::string("Cantidad de parámetros incorrecta.");
        }

        char aux_command_received[MAX_BUFFER_SIZE] = {0};
        std::string command_received;
        std::string response;
        char op[2];
        uint16_t lenght;
        uint16_t port = atoi(argv[1]);

        Socket server;
        Socket client(-1);

        server.bind_and_listen(port);
        server.accept(client);

        Command command;

        bool is_finished = false;

        while (!is_finished){
            try {
                client.receive(aux_command_received, 1);

                is_finished = strlen(aux_command_received) == 0;

                if (!is_finished){
                    strncpy(op, aux_command_received, 1);
                    op[1]= '\0';

                    lenght = Command::get_size_of_request(op[0]) - 1;

                    client.receive(aux_command_received, lenght);
                    aux_command_received[lenght] = '\0';

                    command_received = "";
                    command_received.assign(std::string(op));
                    command_received.append(std::string(aux_command_received));

                    response = command.execute(command_received);

                    std::cout
                            << command_received
                            << " -> "
                            << response
                            << std::endl;
                }
            } catch (std::string ex) {
                response = ex;
                std::cerr
                        << command_received
                        << " -> "
                        << response
                        << std::endl;
            }

            client.send(response.c_str(), response.length());
            aux_command_received[0] = '\0';
        }
    } catch (std::string ex) {
        std::cerr << ex;
    } catch (...) {
        std::cerr << "Ocurrió un error desconocido en el servidor.";
    }

    return EXIT_SUCCESS;
}
