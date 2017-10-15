#ifndef CARDDATASTORAGE_H
#define CARDDATASTORAGE_H

#include "common_Structs.h"

#include <string>
#include <mutex>

class CardDataStorage {
private:
    card_data_map_t data_map;
    std::mutex m;
    static CardDataStorage* instance;

public:
    static CardDataStorage* get_instance();
    std::string execute_query(std::string q);
    
private:
    CardDataStorage();
    CardDataStorage(CardDataStorage const&) = delete;
    void operator=(CardDataStorage const&) = delete;
};

#endif /* CARDDATASTORAGE_H */
