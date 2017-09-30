#include "common_Command.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <netinet/in.h>
#include <bitset>

const std::map<char, uint16_t> Command::command_lenght = {
  { 'A', 20 },
  { 'F', 20 },
  { 'P', 10 },
  { 'R', 10 },
  { 'S', 20 },
};

const std::map<uint16_t, char> Command::command_code = {
  { 0, 'A' },
  { 1, 'F' },
  { 2, 'P' },
  { 3, 'R' },
  { 4, 'S' },
};

Command::Command(file_registry_t &fr){
    // 0000000000111000 -> 38
    uint16_t bits_operacion = (fr.metadata & 0x38) >> 3;

     // 1111100000000000 -> F800
    bits_checksum_tarjeta = (fr.metadata & 0xF800) >> 9;

    // 0000011111000000 -> 7C0
    bits_checksum_monto = (fr.metadata & 0x7C0) >> 6;

    // 0000000000111000 -> 38
    bits_operacion = (fr.metadata & 0x38) >> 3;

    std::bitset<32> bits_trj(fr.tarjeta);
    std::bitset<32> bits_mnt(fr.monto);

    this->code = command_code.at(ntohs(bits_operacion));
    this->card = ntohl(fr.tarjeta);
    this->amount = ntohl(amount);
    this->size = command_lenght.at(code);

    if (ntohl(bits_checksum_monto) != bits_mnt.count()){
        std::string fc = get_formatted_command();
        throw fc.append(" -> E00001");
    }

    if (ntohl(bits_checksum_tarjeta) != bits_trj.count()){
        std::string fc = get_formatted_command();
        throw fc.append(" -> E00001");
    }
}

std::string Command::get_formatted_command(){
    std::string formatted_command = "";
    char t[10];
    char m[10];
    std::string c(&code);


    snprintf (t, 10, "%09d", card);
    snprintf (m, 10, "%09d", amount);

    formatted_command.append(c);
    formatted_command.append(t);
    formatted_command.append(m);

    return formatted_command;
}

const uint16_t Command::get_size(){
    return this->size;
}

uint16_t Command::get_size_of(char code){
    return command_lenght.at(code);
}

Command::~Command() { }

