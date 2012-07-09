#include <stdlib.h>
#include <string.h>
#include "tile_pool.h"

struct tile_pool_t {
	tile_t uncovered_tiles[ALL_COUNT];
	int picks_count;
};

static const tile_t all_types[] = { C(1), C(2), C(3), C(4), C(5), C(6), C(7),
		C(8), C(9), R(1), R(2), R(3), R(4), R(5), R(6), R(7), R(8), R(9),
		B(1), B(2), B(3), B(4), B(5), B(6), B(7), B(8), B(9), MJ_EAST,
		MJ_SOUTH, MJ_WEST, MJ_NORTH, MJ_ZHONG, MJ_FA, MJ_BAI };
/*
static const tile_t all_types[] = {R(1), R(2), R(3), R(4), R(5), R(6), R(7), R(8), R(9),
		B(1), B(2), B(3), B(4), B(5), B(6), B(7), B(8), B(9)};
*/
#define TILE_TYPES sizeof(all_types)/sizeof(all_types[0])

static void tile_pool_init(tile_pool_t * self);

tile_pool_t * create_tile_pool() {
	tile_pool_t * pool = (tile_pool_t *) malloc(sizeof(tile_pool_t));
	tile_pool_init(pool);
	return pool;
}

static void tile_pool_init(tile_pool_t * self) {
	int cnt = 0;
	int i = 0, j;

	for (j = 0; j < 4; j++)
		for (i = 0; i < TILE_TYPES; i++)
			self->uncovered_tiles[cnt++] = all_types[i];
	self->picks_count = 0;
}

#define SHUFFLE_TIMES 1000
static int random_index() {
	return rand() % ALL_COUNT;
}

void _tile_pool_swap(tile_pool_t *self, int index1, int index2) {
	tile_t temp = self->uncovered_tiles[index1];
	self->uncovered_tiles[index1] = self->uncovered_tiles[index2];
	self->uncovered_tiles[index2] = temp;
}

static void tile_pool_shuffle_impl(tile_pool_t * self) {
	tile_pool_init(self);

	int i;
	for (i = 0; i < SHUFFLE_TIMES; i++)
		_tile_pool_swap(self, random_index(), random_index());
}

void (*tile_pool_shuffle)(tile_pool_t * tile_pool) = tile_pool_shuffle_impl;

static tile_t tile_pool_pop_a_tile_impl(tile_pool_t * game) {
	return game->uncovered_tiles[game->picks_count++];
}
tile_t (*tile_pool_pop_a_tile)(tile_pool_t * self) = tile_pool_pop_a_tile_impl;

void tile_pool_destroy(tile_pool_t * game) {
	free(game);
}

static int tile_pool_is_end_impl(tile_pool_t * self) {
	return self->picks_count >= MAX_PICKS;
}
int (*tile_pool_is_end)(tile_pool_t * tile_pool) = tile_pool_is_end_impl;

