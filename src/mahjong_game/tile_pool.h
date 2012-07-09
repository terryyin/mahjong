#ifndef GAME__H
#define GAME__H

#include "tiles.h"

#define ALL_COUNT 134
#define MAX_PICKS 70

typedef struct tile_pool_t tile_pool_t;

tile_pool_t * create_tile_pool(void);

extern void (*tile_pool_shuffle)(tile_pool_t * self);
extern int (*tile_pool_is_end)(tile_pool_t * self);
void tile_pool_destroy(tile_pool_t * self);
extern tile_t (*tile_pool_pop_a_tile)(tile_pool_t * self);

#endif
