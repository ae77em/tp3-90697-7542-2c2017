#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include <cstdint>

typedef struct {
    uint16_t metadata;
    uint32_t tarjeta;
    uint32_t monto;   
} file_registry_t;

#endif /* COMMON_STRUCTS_H */
