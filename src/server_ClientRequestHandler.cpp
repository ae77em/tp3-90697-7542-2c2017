#include "server_ClientRequestHandler.hcommon_Command#include "common_Command.h"

#include <string>
#include <cstring>
#include <iostream>

ClientRequestHandler::ClientRequestHandler(Socket &c) : client(c) { }

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::run() {
    char aux_command_received[MAX_BUFFER_SIZE] = {0};
    char aux_op[2] = {0};
    std::string command_received;
    std::string response;
    int lenght;

    bool is_data_received = client.receive(aux_op, 1) == 1;

    while (is_data_received) {
        try {
            op = aux_op[0];
            aux_op[0] = '\0';
            
            lenght = Command::get_size_of_request(op) - 1;

            if (lenght != 0) {
                client.receive(aux_command_received, lenght);

                aux_command_received[lenght] = '\0';

                command_received = "";
                command_received.assign(std::string(1, op));
                command_received.append(std::string(aux_command_received));

                response = command.execute(command_received);

                std::cout
                        << command_received
                        << " -> "
                        << response
                        << std::endl;
            } else {
                std::string ex = "La conexión del socket ";
                ex.append(std::to_string(client.get_socket()));
                ex.append(" se cerró.");

                throw ex;
            }
        } catch (std::string ex) {
            response = ex;
            std::cerr
                    << command_received
                    << " -> "
                    << response
                    << std::endl;
        } catch (...) {
            std::cerr
                    << "Error desconocido procesando el request."
                    << std::endl;
        }

        client.send(response.c_str(), response.length());

        is_data_received = client.receive(aux_op, 1) == 1;
    }
}

