#ifndef BINARY_DATA_FILE_H
#define BINARY_DATA_FILE_H

#include "common_Structs.h"

#include <string>
#include <fstream>

class BinaryDataFile {
private:
    file_registry_t file_registry;
    std::fstream file;
    int file_size;
    int bytes_readed;

public:
    explicit BinaryDataFile(const std::string& fn);
    virtual ~BinaryDataFile();
    
    void read();
    void write(file_registry_t& registry);
    bool eof();
    bool good();
    bool has_data();
    file_registry_t &get_file_registry();
    
private:
    BinaryDataFile() = delete;
    BinaryDataFile(const BinaryDataFile& orig) = delete; 
};

#endif /* CLIENT_FILE_HANDLER_H */
