#include "client_BinaryDataFile.h"
#include "common_Structs.h"

#include <string>
#include <fstream>
#include <netinet/in.h>
#include <iostream>

BinaryDataFile::BinaryDataFile(const std::string& fn) {
    file.open(fn.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open()){
        throw std::string("No se pudo abrir el archivo especificado.");
    }

    bytes_readed = 0;
    std::streampos begin,end;
    begin = file.tellg();
    file.seekg(0, std::ios::end);
    end = file.tellg();
    file_size = end - begin;
    file.seekg(0, std::ios::beg);

    file_registry.metadata = 0;
    file_registry.amount = 0;
    file_registry.card = 0;
}

BinaryDataFile::~BinaryDataFile() {
    file.close();
}

void BinaryDataFile::read(){
    uint16_t checksum;
    uint32_t card;
    uint32_t amount;

    file.read(reinterpret_cast<char*>(&checksum), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&card), sizeof(uint32_t));

    file_registry.metadata = ntohs(checksum);
    file_registry.card = ntohl(card);
    file_registry.amount = 0;

    // 0x38 -> 00000000 00111000
    //         00001000 00011000
    uint16_t op = (file_registry.metadata & 0x38) >> 3;
    // si corresponde leo el monto
    if ( op == 0 || op == 1 || op == 4 ){
        file.read(reinterpret_cast<char*>(&amount), sizeof(uint32_t));
        file_registry.amount = ntohl(amount);
        bytes_readed += 10;
    } else {
        bytes_readed += 6;
    }
}

bool BinaryDataFile::has_data(){
    return file.tellg() < file_size;
}

file_registry_t& BinaryDataFile::get_file_registry() {
    return file_registry;
}

