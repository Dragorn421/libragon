#ifndef __LIBDRAGON_CHUNKS_CHUNKS_DEFS_H
#define __LIBDRAGON_CHUNKS_CHUNKS_DEFS_H

#include <stdint.h>

struct chunks_header {
    uint32_t version; // 0
    uint32_t n_chunks;
    struct chunk_header {
        char name[4];
        uint32_t data; // rom offset
    } chunks[];
};

struct env_chunk {
    uint32_t version; // 0
    uint32_t env;     // rom offset to n_env KEY=VALUE strings
    uint32_t n_env;
    uint32_t env_size; // amount of env data in bytes
};

struct args_chunk {
    uint32_t version; // 0
    uint32_t args;    // rom offset to n_args strings
    uint32_t n_args;
    uint32_t args_size; // amount of args data in bytes
};

#endif
