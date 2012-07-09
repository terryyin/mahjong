#include "player.h"
#include "memory.h"
#include <stdlib.h>
#include "agent.h"

struct player_t {
	tile_t holdings[MAX_HOLDING_COUNT];
	eaten_t eaten[MAX_EATEN_COUNT];
	tile_t current;
};

void player_destroy(player_t *self) {
	free(self);
}
void player_pick(player_t *self, tile_t tile) {
	self->current = tile;
}

static void player_sort(player_t * self) {
	tiles_sort(self->holdings, MAX_HOLDING_COUNT);
}

int player_is_able_to_win(player_t * self, tile_t discard) {
	if (discard == NO_TILE)
		discard = self->current;
	return tiles_plus_one_is_winning(self->holdings, MAX_HOLDING_COUNT, discard);
}

int player_get_holdings(struct player_t *self, tile_t * tiles_buffer,
		int buffer_size) {
	int tile_count = tiles_get_count(self->holdings, MAX_HOLDING_COUNT);
	memcpy(tiles_buffer, self->holdings, tile_count * sizeof(tile_t));
	return tile_count;
}
int player_get_eaten(struct player_t *self, eaten_t * tiles_buffer,
		int buffer_size) {
	int tile_count = 0;
	for (; tile_count < MAX_EATEN_COUNT; tile_count++) {
		if (self->eaten[tile_count])
			tiles_buffer[tile_count] = self->eaten[tile_count];
		else
			break;
	}
	return tile_count;
}
tile_t player_get_current(struct player_t *self) {
	return self->current;
}

void player_deal(struct player_t *self, tile_t tiles[], int buffer_size) {
	int i = 0;
	for (i = 0; i < MAX_HOLDING_COUNT; i++)
		self->holdings[i] = NO_TILE;
	for (i = 0; i < MAX_EATEN_COUNT; i++)
		self->eaten[i] = 0;
	for (i = 0; i < buffer_size; i++)
		self->holdings[i] = tiles[i];
	player_sort(self);
}

tile_t player_discard_tile(player_t * self, tile_t tile) {
	if (tile == self->current) {
		self->current = NO_TILE;
		return tile;
	} else {
		int i;
		for (i = 0; i < MAX_HOLDING_COUNT; i++) {
			if (self->holdings[i] == tile)
				break;
		}
		if (i < MAX_HOLDING_COUNT) {
			self->holdings[i] = self->current;
			self->current = NO_TILE;
			player_sort(self);
			return tile;
		}
	}

	return NO_TILE;
}
int player_is_able_to_pong(struct player_t *self, tile_t tile) {
	return tiles_is_able_to_pong(self->holdings, MAX_HOLDING_COUNT, tile);
}
int player_is_able_to_chew(struct player_t *self, tile_t tile) {
	return tiles_is_able_to_chow(self->holdings, MAX_HOLDING_COUNT, tile);
}
static void _rearrange_after_eat(player_t *self, eaten_t eaten) {
	int i, cnt;

	player_sort(self);
	cnt = tiles_get_count(self->holdings, MAX_HOLDING_COUNT);
	self->current = self->holdings[cnt - 1];
	self->holdings[cnt - 1] = NO_TILE;

	for (i = 0; i < MAX_EATEN_COUNT; i++) {
		if (self->eaten[i] == 0)
			break;
	}
	self->eaten[i] = eaten;
}

void player_pong(struct player_t *self, tile_t tile) {
	int i, cnt = 0;
	for (i = 0; i < MAX_HOLDING_COUNT; i++) {
		if (self->holdings[i] == tile && cnt < 2) {
			self->holdings[i] = NO_TILE;
			cnt++;
		}
	}
	_rearrange_after_eat(self, PONG(tile));
}

static int _is_able_to_chow(struct player_t *self, tile_t tile, tile_t smallest)
{
	int i, cnt;
	if (tile > smallest + 2)
		return 0;
	cnt = tiles_get_count(self->holdings, MAX_HOLDING_COUNT);
	for (i = 0; i < 3; i++)
		if (tile != smallest + i)
			if (tiles_get_index_of_tile(self->holdings, cnt, smallest + i)
					== -1)
				return 0;
	return 1;
}
static void _chow(struct player_t *self, tile_t tile, tile_t smallest)
{
	int i, cnt = 0;
	cnt = tiles_get_count(self->holdings, MAX_HOLDING_COUNT);
	for (i = 0; i < 3; i++)
		if (tile != smallest + i)
			self->holdings[tiles_get_index_of_tile(self->holdings, cnt,
					smallest + i)] = NO_TILE;

	_rearrange_after_eat(self, CHOW(smallest));
}
int player_chow(struct player_t *self, tile_t tile, tile_t with) {
	tile_t smallest = tile < with ? tile : with;
	if (!_is_able_to_chow(self, tile, smallest))
		return 0;
	_chow(self, tile, smallest);
	return 1;
}
static void player_private_init(player_t * player) {
	int i;
	player->current = NO_TILE;
	for (i = 0; i < MAX_HOLDING_COUNT; i++) {
		player->holdings[i] = NO_TILE;
	}
	for (i = 0; i < MAX_EATEN_COUNT; i++) {
		player->eaten[i] = 0;
	}
}

player_t * create_player_data() {
	player_t * player = (player_t *) malloc(sizeof(player_t));

	player_private_init(player);
	return player;
}
