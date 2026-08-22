#include "chunks_defs.h"

__attribute__((section("chunk_data")))
struct args_chunk args_chunk = {
    .version = 0,
    .args = 0,
    .n_args = 0,
    .args_size = 0,
};
