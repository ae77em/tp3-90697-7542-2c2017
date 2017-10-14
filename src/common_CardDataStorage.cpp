#include "common_CardDataStorage.h"
#include "common_Structs.h"

#include <string>
#include <cstdint>
#include <stddef.h>
#include <iostream>

CardDataStorage *CardDataStorage::instance = NULL;

CardDataStorage::CardDataStorage() { }

CardDataStorage *CardDataStorage::get_instance() {
    if (instance == NULL) {
        instance = new CardDataStorage();
    }
    return instance;
}

std::string CardDataStorage::execute_query(std::string cmd) {
    m.lock();

    std::string to_return;
    to_return.assign(cmd);

    std::string op;
    op.assign(cmd.substr(0, 1));
    uint32_t card = stoul(cmd.substr(1, 10));
    char c[11];
    char a[11];

    card_data_map_t::iterator it = data_map.find(card);

    if (op.compare("A") == 0) {
        if (it != data_map.end()) {
            int32_t amount = stoi(cmd.substr(11, 10));
            if (amount < 0 && (it->second + amount) < 0) {
                throw std::string("E00003");
            } else {
                it->second += amount;
                snprintf(c, sizeof (c), "%010u", card);
                snprintf(a, sizeof (a), "%010d", it->second);
                to_return.assign(op);
                to_return.append(c);
                to_return.append(a);
            }
        } else {
            to_return.assign("E00002");
        }

    } else if (op.compare("F") == 0) {
        if (it != data_map.end()) {
            int32_t amount = stoi(cmd.substr(11, 10));
            it->second += amount;
            snprintf(c, sizeof (c), "%010u", card);
            snprintf(a, sizeof (a), "%010d", it->second);
            to_return.assign(op);
            to_return.append(c);
            to_return.append(a);
        } else {
            to_return.assign("E00002");
        }
    } else if (op.compare("P") == 0) {
        if (it != data_map.end()) {
            snprintf(a, sizeof (a), "%010d", it->second);
            to_return.append(a);
        } else {
            to_return.assign("E00002");
        }
    } else if (op.compare("R") == 0) {
        if (it == data_map.end()) {
            int32_t amount = 0;
            data_map.insert(std::make_pair(card, amount));
        } else {
            to_return.assign("E00004");
        }
    } else if (op.compare("S") == 0) {
        if (it != data_map.end()) {
            int32_t amount = stoi(cmd.substr(11, 10));
            it->second = amount;
        } else {
            to_return.assign("E00002");
        }
    } else {
        std::cerr << "Commando desconocido";
    }

    m.unlock();
    return to_return;
}
