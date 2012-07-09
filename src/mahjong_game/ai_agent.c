#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ai_agent.h"
#include "player.h"
#include "evaluator.h"
#include "assert.h"

typedef struct ai_agent_t {
	agent_t agent;
	action_t action;
	tile_t tile;
	player_t * player;
	evaluator_t * evaluator;

} ai_agent_t;

static void ai_agent_destroy(agent_t * self) {
	ai_agent_t * ai_self = (ai_agent_t*) self;
	if (ai_self->player)
		player_destroy(ai_self->player);
	ai_self->evaluator->destroy(ai_self->evaluator);
	free(self);
}

void ai_agent_deal(agent_t * self, tile_t tiles[], int n, int distance) {
	if (distance == 0) {
		ai_agent_t * ai_self = (ai_agent_t*) self;
		player_t * player_data = create_player_data();
		player_deal(player_data, tiles, n);
		ai_self->player = player_data;
	}
}

static tile_t ai_which_to_discard(ai_agent_t * ai_self) {
	int i;
	int max = 0;
	int index_to_throw = 0;
	tile_t holdings[MAX_HOLDING_COUNT+1];
	tile_t tiles[MAX_HOLDING_COUNT+1];
	player_t * player = ai_self->player;
	int tile_count = player_get_holdings(player, holdings, MAX_HOLDING_COUNT);
	holdings[tile_count] = player_get_current(player);
	for (i = 0; i < tile_count+1; i++) {
		player_get_holdings(player, tiles, MAX_HOLDING_COUNT);
		tiles[i] = player_get_current(player);
		int score = ai_self->evaluator->evaluate_array(ai_self->evaluator, tiles, tile_count);
		if (score > max) {
			max = score;
			index_to_throw = i;
		}
	}

	return holdings[index_to_throw];
}
static void ai_agent_pick(agent_t * self, tile_t tile, int distance) {
	if (distance == 0) {
		ai_agent_t * ai_self = (ai_agent_t*) self;
		player_pick(ai_self->player, tile);
		if (player_is_able_to_win(ai_self->player, NO_TILE))
			ai_self->action = ACTION_WIN;
		else {
			ai_self->action = ACTION_DISCARD;
			ai_self->tile = ai_which_to_discard(ai_self);
		}
	}
}

static void ai_agent_pong(agent_t * self, tile_t tile, int distance) {
}
static int ai_agent_chow(agent_t * self, tile_t tile, tile_t with, int distance) {
	return 0;
}
static void ai_agent_win(agent_t * self, int score, int distance) {
	ai_agent_t * ai_self = (ai_agent_t*) self;
	ai_self->action = ACTION_RESTART;
	player_destroy(ai_self->player);
	ai_self->player = NULL;
}

static action_t ai_agent_get_action(struct agent_t * self, tile_t* tile) {
	ai_agent_t * ai_self = (ai_agent_t*) self;
	if (tile != NULL)
		*tile = ai_self->tile;
	return ai_self->action;
}

static void ai_agent_set_action(agent_t * self, action_t action, tile_t tile) {
	ai_agent_t * ai_self = (ai_agent_t*) self;

	ai_self->action = action;
	ai_self->tile = tile;
}

static void ai_agent_discard_tile(struct agent_t *self, tile_t tile, int distance) {
	ai_agent_t * ai_self = (ai_agent_t*) self;
	if (distance == 0)
		player_discard_tile(ai_self->player, tile);
	else {
		if (player_is_able_to_win(ai_self->player, tile))
			ai_self->action = ACTION_WIN;
		else
			ai_self->action = ACTION_PICK;
	}
}

agent_t * create_ai_agent() {
	agent_t * agent = (agent_t *) malloc(sizeof(ai_agent_t));

	agent->destroy = ai_agent_destroy;
	agent->deal = ai_agent_deal;
	agent->pick = ai_agent_pick;
	agent->pong = ai_agent_pong;
	agent->chow = ai_agent_chow;
	agent->discard_tile = ai_agent_discard_tile;
	agent->get_action = ai_agent_get_action;
	agent->set_action = ai_agent_set_action;
	agent->win = ai_agent_win;

	ai_agent_t * self = (ai_agent_t *) agent;
	self->action = ACTION_RESTART;
	self->player = NULL;
	self->evaluator = create_evaluator_r();

	return agent;
}
