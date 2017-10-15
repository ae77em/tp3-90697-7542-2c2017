#include "common_Command.h"
#include "common_CardDataStorage.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <netinet/in.h>
#include <bitset>
#include <map>
#include <iostream>

/*
 * Inicializo mapas con datos...
 */

const std::map<char, uint16_t> Command::request_lenght = {
    { 'A', 21},
    { 'F', 21},
    { 'P', 11},
    { 'R', 11},
    { 'S', 21},
};

const std::map<char, uint16_t> Command::response_lenght = {
    { 'A', 21},
    { 'F', 21},
    { 'P', 21},
    { 'R', 11},
    { 'S', 21},
};

const std::map<uint16_t, char> Command::command_code = {
    { 0, 'A'},
    { 1, 'F'},
    { 2, 'P'},
    { 3, 'R'},
    { 4, 'S'},
};

/*
 * Implementación de la clase propiamente dicha.
 */

Command::Command() { }

Command::Command(file_registry_t &fr) {
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
    this->size = request_lenght.at(code);

    checkshum_ok = true;
    int count = (bits_mnt.count() != 32) ? bits_mnt.count() : 0;
    if (bits_checksum_monto != count) {
        checkshum_ok = false;
    }

    count = (bits_trj.count() != 32) ? bits_trj.count() : 0;
    if (bits_checksum_tarjeta != count) {
        checkshum_ok = false;
    }
}

Command::~Command() { }

std::string Command::get_formatted_command() {
    std::string formatted_command = "";
    char card[11];

    snprintf(card, sizeof (card), "%010u", this->card);

    formatted_command.push_back(code);
    formatted_command.append(card);

    if (code == 'A' || code == 'F' || code == 'S') {
        char amount[11];
        snprintf(amount, sizeof (amount), "%010d", this->amount);
        formatted_command.append(amount);
    }

    return formatted_command;
}

const uint16_t Command::get_size() {
    return this->size;
}

uint16_t Command::get_size_of_request(char code) {
    return request_lenght.at(code);
}

uint16_t Command::get_size_of_response(char code) {
    return response_lenght.at(code);
}

std::string Command::execute(const std::string& cmd) {
    CardDataStorage *cds = CardDataStorage::get_instance();

    std::string to_return = cds->execute_query(cmd);

    return to_return;
}

bool Command::is_checksum_ok(){
    return checkshum_ok;
}