#include "server_RequestProccessor.h"

#include "common_Command.h"

#include <string>
#include <cstring>
#include <iostream>

RequestProccessor::RequestProccessor(char o, Socket &c, Command *cmd)
: op(o), client(c), command(cmd) { }

RequestProccessor::~RequestProccessor() { }

void RequestProccessor::run(){
    char aux_command_received[MAX_BUFFER_SIZE] = {0};
    std::string command_received;
    std::string response;
    int lenght;

    try {
        lenght = Command::get_size_of_request(op) - 1;

        if (lenght != 0){
            client.receive(aux_command_received, lenght);

            aux_command_received[lenght] = '\0';

            command_received = "";
            command_received.assign(std::string(1, op));
            command_received.append(std::string(aux_command_received));

            std::cout << "comando: " << command_received << std::endl;

            response = command->execute(command_received);

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
    }

    client.send(response.c_str(), response.length());
}

