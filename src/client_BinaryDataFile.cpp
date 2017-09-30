#include "client_BinaryDataFile.h"
#include "common_Structs.h"

#include <string>
#include <fstream>
#include <netinet/in.h>

BinaryDataFile::BinaryDataFile(const std::string& fn) {
    file.open(fn.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    if (!good()){
        throw "No se pudo abrir el archivo especificado.";
    }

    file_registry.metadata = 0;
    file_registry.monto = 0;
    file_registry.tarjeta = 0;
}

BinaryDataFile::~BinaryDataFile() {
    file.close();
}

void BinaryDataFile::read(){
    file.read ((char*)&file_registry, sizeof (file_registry_t));
}

void BinaryDataFile::write(file_registry_t& fr){

}

bool BinaryDataFile::good(){
    return file.good();
}

file_registry_t& BinaryDataFile::get_file_registry() {
    return file_registry;
}

