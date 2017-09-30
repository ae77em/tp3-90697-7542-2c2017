#ifndef COMMON_COMMAND_H
#define COMMON_COMMAND_H

#include "common_Structs.h"

#include <string>
#include <map>

class Command {
private:
    static const std::map<char, uint16_t> command_lenght;
    static const std::map<uint16_t , char> command_code;
    char code;
    int card;
    int amount;
    uint16_t size;
    
    u_int16_t bits_checksum_tarjeta;
    u_int16_t bits_checksum_monto; 
    u_int16_t bits_operacion;   
    
public:
    Command(file_registry_t& r);
    virtual ~Command();
    
    std::string get_formatted_command();
    const uint16_t get_size();
    static uint16_t get_size_of(char code);
    
private:
    Command() = delete;
    Command(const Command& orig) = delete;    
};

#endif /* COMMON_COMMAND_H */

