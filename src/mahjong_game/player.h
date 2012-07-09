#ifndef PLAYER_H_
#define PLAYER_H_

#define MAX_EATEN_COUNT (MAX_HOLDING_COUNT/3)

#include "tiles.h"

typedef struct player_t player_t;

player_t * create_player_data(void);
void player_destroy(player_t *self);

int player_get_holdings(player_t *self, tile_t * tiles_buffer, int buffer_size);
int player_get_eaten(player_t *self, eaten_t * tiles_buffer, int buffer_size);
tile_t player_get_current(player_t *self);

void player_pick(player_t *self, tile_t);
void player_pong(player_t *self, tile_t);
int player_chow(struct player_t *self, tile_t tile, tile_t with);
tile_t player_discard_tile(player_t *self, tile_t tile);
void player_deal(player_t *self, tile_t tiles[], int tiles_count);

int player_is_able_to_pong(player_t *self, tile_t tile);
int player_is_able_to_chew(player_t *self, tile_t tile);
int player_is_able_to_win(player_t *self, tile_t tile);


#endif /* PLAYER_H_ */
