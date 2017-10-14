#ifndef COMMON_COMMAND_H
#define COMMON_COMMAND_H

#include "common_Structs.h"

#include <string>
#include <map>

class Command {
private:
    static const std::map<char, uint16_t> request_lenght;
    static const std::map<char, uint16_t> response_lenght;
    static const std::map<uint16_t , char> command_code;
    char code;
    int card;
    int amount;
    uint16_t size;
    
    u_int16_t bits_checksum_tarjeta;
    u_int16_t bits_checksum_monto;    
    
public:
    Command();
    explicit Command(file_registry_t& fr);
    virtual ~Command();
    
    std::string get_formatted_command();
    const uint16_t get_size();
    static uint16_t get_size_of_request(char code);
    static uint16_t get_size_of_response(char code);
    std::string execute(const std::string& cmd);
    
private:
    Command(const Command& orig) = delete;    
};

#endif /* COMMON_OPERATION_H */

