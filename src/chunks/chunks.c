#include "chunks.h"

#include "chunks_defs.h"
#include "dma.h"
#include "n64sys.h"
#include "scratch.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define HEADER_CHECK_CODE_2 0x10000014
#define HEADER_FLAGS 0x10000038
#define HEADER_GAME_ID 0x1000003C

#define FLAG_HAS_CHUNKS 2

uint32_t get_chunk_rom_offset(const char *name) {
    uint32_t game_id = io_read(HEADER_GAME_ID);

    if (!((game_id >> 24) == 'E' && ((game_id >> 16) & 0xFF) == 'D'))
        return 0;

    uint32_t flags = io_read(HEADER_FLAGS);
    if (!(flags & FLAG_HAS_CHUNKS))
        return 0;

    uint32_t chunks_header_offset = io_read(HEADER_CHECK_CODE_2) + 0x10000000;

    static_assert(sizeof((struct chunks_header){}.n_chunks) == 4,
                  "chunks_header.n_chunks is not 4 bytes");
    uint32_t n_chunks = io_read(chunks_header_offset +
                                offsetof(struct chunks_header, n_chunks));

    size_t sz = n_chunks * sizeof(struct chunk_header);
    struct chunk_header *chunks = scratch_malloc(sz);
    data_cache_hit_writeback_invalidate(chunks, sz);
    dma_read(chunks,
             chunks_header_offset + offsetof(struct chunks_header, chunks), sz);

    uint32_t rom_offset = 0;
    for (uint32_t i = 0; i < n_chunks; i++) {
        if (strncmp(name, chunks[i].name, 4) == 0) {
            rom_offset = chunks[i].data;
            break;
        }
    }
    scratch_free(chunks);
    return rom_offset;
}
