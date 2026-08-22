#include "chunks.h"
#include "chunks_defs.h"
#include "debug.h"
#include "dma.h"
#include "n64sys.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

__attribute__((constructor)) void env_init(void) {
    uint32_t env_chunk_offset = get_chunk_rom_offset("ENV") + 0x10000000;
    if (env_chunk_offset == 0)
        return;
    struct env_chunk env_chunk;
    data_cache_hit_writeback_invalidate(&env_chunk, sizeof(struct env_chunk));
    dma_read(&env_chunk, env_chunk_offset, sizeof(struct env_chunk));
    if (env_chunk.n_env == 0)
        return;
    char *env_data = malloc(env_chunk.env_size);
    assert(env_data != NULL);
    data_cache_hit_writeback_invalidate(env_data, env_chunk.env_size);
    dma_read(env_data, env_chunk.env, env_chunk.env_size);
    size_t counted_env_size = 0;
    for (uint32_t i = 0; i < env_chunk.n_env; i++) {
        putenv(env_data);
        size_t sz = strlen(env_data) + 1;
        env_data += sz;
        counted_env_size += sz;
    }
    assert(env_chunk.env_size == counted_env_size);
}

int envargs_argc = 0;
char **envargs_argv = NULL;

__attribute__((constructor)) void args_init(void) {
    uint32_t args_chunk_offset = get_chunk_rom_offset("ARGS") + 0x10000000;
    if (args_chunk_offset == 0)
        return;
    struct args_chunk args_chunk;
    data_cache_hit_writeback_invalidate(&args_chunk, sizeof(struct env_chunk));
    dma_read(&args_chunk, args_chunk_offset, sizeof(struct env_chunk));
    if (args_chunk.n_args == 0)
        return;
    char *args_data = malloc(args_chunk.args_size);
    assertf(args_data != NULL, "Cannot malloc %" PRId32 " bytes for args_data",
            args_chunk.args_size);
    char **argv = malloc(args_chunk.n_args * sizeof(char *));
    assert(argv != NULL);
    data_cache_hit_writeback_invalidate(args_data, args_chunk.args_size);
    dma_read(args_data, args_chunk.args + 0x10000000, args_chunk.args_size);
    size_t counted_args_size = 0;
    for (uint32_t i = 0; i < args_chunk.n_args; i++) {
        argv[i] = args_data;
        size_t sz = strlen(args_data) + 1;
        args_data += sz;
        counted_args_size += sz;
    }
    assertf(args_chunk.args_size == counted_args_size, "%" PRId32 " %d",
            args_chunk.args_size, counted_args_size);
    envargs_argc = args_chunk.n_args;
    envargs_argv = argv;
}
