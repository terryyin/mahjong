#ifndef AGEND_H_
#define AGEND_H_

#include "tile.h"

typedef enum {
	NO_ACTION,
	ACTION_DISCARD,
	ACTION_PICK,
	ACTION_PONG,
	ACTION_CHOW,
	ACTION_KONG,
	ACTION_WIN,
	ACTION_RESTART
}action_t;

typedef struct agent_t {
	void (*destroy)(struct agent_t * self);
	void (*deal)(struct agent_t * self, tile_t tiles[], int n, int distance);
	void (*pick)(struct agent_t * self, tile_t tile, int distance);
	void (*pong)(struct agent_t * self, tile_t tile, int distance);
	int (*chow)(struct agent_t * self, tile_t tile, tile_t with, int distance);
	void (*win)(struct agent_t * self, int score, int distance);
	action_t (*get_action)(struct agent_t * self, tile_t* tile);
	void (*set_action)(struct agent_t * self, action_t action, tile_t tile);
	void (*discard_tile)(struct agent_t *self, tile_t tile, int distance);
} agent_t;

#endif /* AGEND_H_ */
