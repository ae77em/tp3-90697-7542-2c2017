#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include <cstdint>
#include <unordered_map>

#define MAX_BUFFER_SIZE 30

typedef struct {
    uint16_t metadata;
    uint32_t card;
    int32_t amount;   
} file_registry_t;

typedef std::unordered_map<uint32_t, int32_t> card_data_map_t;

#endif /* COMMON_STRUCTS_H */
