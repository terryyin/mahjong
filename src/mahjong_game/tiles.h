#ifndef TILES__H
#define TILES__H

#include "tile.h"
#define MAX_HOLDING_COUNT 13

int tiles_is_able_to_chow(tile_t tiles[], int size_of_array, tile_t tile);
int tiles_get_count(tile_t tiles[], int size_of_array);
int tiles_get_index_of_tile(tile_t tiles[], int n, tile_t tile);
void tiles_sort(tile_t tiles[], int n);
int tiles_is_able_to_pong(tile_t tiles[], int n, tile_t tile);
int tiles_plus_one_is_winning(tile_t holding[], int n, tile_t discard);
tile_t * tiles_from_string(tile_t * buffer, const char * string);
const char * tiles_to_string(tile_t tiles[], int n, char * buf, int buffer_size);

#endif
