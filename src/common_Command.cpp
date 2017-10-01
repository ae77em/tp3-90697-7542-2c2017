#include "common_Command.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <netinet/in.h>
#include <bitset>
#include <map>
#include <iostream>

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

Command::Command(){ }

Command::Command(file_registry_t &fr){
    // 0x38     ->  0000000000111000
    uint16_t bits_operacion = (fr.metadata & 0x38) >> 3;

    // 0xF800   ->  1111100000000000
    bits_checksum_tarjeta = (fr.metadata & 0xF800) >> 11;

    // 0x7C0    ->  0000011111000000
    bits_checksum_monto = (fr.metadata & 0x7C0) >> 6;

    // 0000000000111000 -> 38
    bits_operacion = (fr.metadata & 0x38) >> 3;

    std::bitset<32> bits_trj(fr.card);
    std::bitset<32> bits_mnt(fr.amount);

    this->code = command_code.at(bits_operacion);
    this->card = fr.card;
    this->amount = fr.amount;
    this->size = command_lenght.at(code);

    if (bits_checksum_monto != bits_mnt.count()){
        std::string fc = get_formatted_command();
        throw fc.append(" -> E00001");
    }

    if (bits_checksum_tarjeta != bits_trj.count()){
        std::string fc = get_formatted_command();
        throw fc.append(" -> E00001");
    }
}

Command::~Command() { }

std::string Command::get_formatted_command(){
    std::string formatted_command = "";
    char card[11];
    char amount[11];

    snprintf(card, sizeof(card), "%010d", this->card);
    snprintf(amount, sizeof(amount), "%010d", this->amount);

    formatted_command.push_back(code);
    formatted_command.append(card);
    formatted_command.append(amount);

    return formatted_command;
}

const uint16_t Command::get_size(){
    return this->size;
}

uint16_t Command::get_size_of(char code){
    return command_lenght.at(code);
}

std::string Command::execute(const std::string& cmd) {
    std::string op = cmd.substr(0,1);
    uint32_t card = stoi(cmd.substr(1,10));
    char c[11];
    char a[11];
    std::string to_return(cmd);

    card_data_map_t::iterator it = data_map.find(card);

    if (op.compare("A") == 0){
        if (it != data_map.end()){
            int32_t amount = stoi(cmd.substr(11,10));
            if ((it->second + amount) < 0){
                throw std::string("E00003");
            } else {
                it->second += amount;
                snprintf(c, sizeof(c), "%010d", card);
                snprintf(a, sizeof(a), "%010d", it->second);
                to_return.assign(op);
                to_return.append(c);
                to_return.append(a);
            }
        } else {
            throw std::string("E00002");
        }

    } else if (op.compare("F") == 0){
        if (it != data_map.end()){
            uint32_t amount = stoi(cmd.substr(9,10));
            it->second += amount;
            snprintf(c, sizeof(c), "%010d", card);
            snprintf(a, sizeof(a), "%010d", it->second);
            to_return.assign(op);
            to_return.append(c);
            to_return.append(a);
        } else {
            throw std::string("E00002");
        }
    } else if (op.compare("P") == 0){
        if (it != data_map.end()){
            std::cout << it->second;
        } else {
            throw std::string("E00002");
        }
    } else if (op.compare("R") == 0){
        if (it == data_map.end()){
            uint32_t amount = 0;
            data_map.insert(std::make_pair(card, amount));
        } else {
            throw std::string("E00004");
        }
    } else if (op.compare("S") == 0){
        if (it != data_map.end()){
            uint32_t amount = stoi(cmd.substr(9,10));
            it->second = amount;
        } else {
            throw std::string("E00002");
        }
    } else {
        std::cerr << "Commando desconocido";
    }

    return to_return;
}
