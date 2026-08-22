#include "chunks_defs.h"

__attribute__((section("chunk_data")))
struct env_chunk env_chunk = {
    .version = 0,
    .env = 0,
    .n_env = 0,
    .env_size = 0,
};
