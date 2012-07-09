#include "agent.h"
#include "tile_pool.h"
#include "stdlib.h"
#include "string.h"
#include "mj_table.h"
#include "assert.h"

static void _mj_table_destroy(mj_table_t * self) {
	int i = 0;
	for (; i < self->player_count; i++)
		self->players[i]->destroy(self->players[i]);
}

static agent_t * get_player_of_distance(mj_table_t * self, int i) {
	return self->players[(self->current_player + i)%self->player_count];
}

void mj_table_add_player(mj_table_t * self,
		agent_t * player) {
	self->players[self->player_count++] = player;
}
static int _mj_table_get_player_count(mj_table_t * self) {
	return self->player_count;
}
static void _mj_table_pick(struct mj_table_t * self, tile_t tile) {
	int i = 0;
	for (i = 0; i < self->player_count ; i++) {
		agent_t * agent = get_player_of_distance(self, i);
		agent->pick(agent, tile, i);
	}
}
static void _change_host(mj_table_t * self) {
	self->host = (self->host + 1) % self->player_count;
	self->current_player = self->host;
}
static void _change_current_player(mj_table_t * self, int distance) {
	self->current_player = (self->current_player + distance) % self->player_count;
}
static void _mj_table_win(struct mj_table_t * self, int score) {
	int i = 0;
	for (i = 0; i < self->player_count ; i++) {
		agent_t * agent = get_player_of_distance(self, i);
		agent->win(agent, score, i);
	}
	_change_host(self);
}

static void _mj_table_deal(struct mj_table_t * self, tile_t tiles[], int n) {
	int i = 0;
	for (i = 0; i < self->player_count ; i++) {
		agent_t * agent = get_player_of_distance(self, i);
		agent->deal(agent, tiles, n, i);
	}
	_change_current_player(self, 1);
}

static void _mj_table_throw_tile(struct mj_table_t * self, tile_t tile){
	int i = 0;
	for (i = 0; i < self->player_count ; i++) {
		agent_t * agent = get_player_of_distance(self, i);
		agent->discard_tile(agent, tile, i);
	}
	self->last_tile = tile;
	_change_current_player(self, 1);
}
static void _mj_table_pong(struct mj_table_t * self){
	int i = 0;
	for (i = 0; i < self->player_count ; i++) {
		agent_t * agent = get_player_of_distance(self, i);
		agent->pong(agent, self->last_tile, i);
	}
}
static int _mj_table_chow(struct mj_table_t * self, tile_t with){
	int i = 0;
	agent_t * agent = get_player_of_distance(self, 0);
	if (!agent->chow(agent, self->last_tile, with, 0))
		return 0;
	for (i = 1; i < self->player_count ; i++) {
		agent = get_player_of_distance(self, i);
		agent->chow(agent, self->last_tile, with, i);
	}
	return 1;
}

mj_table_t * create_mj_table(tile_pool_ptr_t pool)
{
	mj_table_t * gf = (mj_table_t *) malloc(sizeof(mj_table_t));
	gf->tile_pool = pool;
	gf->state = GAME_END;
	gf->player_count = 0;
	gf->current_player = 0;
	gf->host = 0;

	return gf;
}

void mj_table_destroy(mj_table_t * self)
{
	tile_pool_destroy(self->tile_pool);
	_mj_table_destroy(self);
	free(self);
}

game_state_t mj_table_get_state(mj_table_t * mj_table)
{
	return mj_table->state;
}

void mj_table_update_state(mj_table_t * self)
{
	int i;
	tile_t action_tile;
	agent_t * agent = get_player_of_distance(self, 0);
	action_t player_action = agent->get_action(agent, &action_tile);

	/**********
	 * get current player.
	 * *******************/
	if (player_action == ACTION_RESTART) {
		int i = 1;
		for (; i < self->player_count; i++) {
			agent_t * agent = get_player_of_distance(self, i);
			if(ACTION_RESTART != agent->get_action(agent, NULL)){
				player_action = NO_ACTION;
				break;
			}
		}
	}

	if(self->state == GAME_END) {
		if (ACTION_RESTART == player_action){
			tile_pool_shuffle(self->tile_pool);
			tile_t tiles[MAX_HOLDING_COUNT];

			int cnt = _mj_table_get_player_count(self);
			for (; cnt >0; cnt --) {
				for (i = 0; i < MAX_HOLDING_COUNT; i++) {
					tiles[i] = tile_pool_pop_a_tile(self->tile_pool);
				}
				_mj_table_deal(self, tiles, MAX_HOLDING_COUNT);
			}

			_mj_table_pick(self, tile_pool_pop_a_tile(self->tile_pool));

			self->state = GAME_PICKED;
		}
	}
	else if (self->state == GAME_WAITING) {
		if (tile_pool_is_end(self->tile_pool)) {
			_mj_table_win(self, 0);
			self->state = GAME_END;
		}
		else {
			self->state = GAME_PICKING;
		}
	}
	else if (self->state == GAME_PICKED) {
		if (NO_ACTION == player_action){
		}
		else if (ACTION_DISCARD == player_action) {
			_mj_table_throw_tile(self, action_tile);
			self->state = GAME_WAITING;
		} else if (ACTION_WIN == player_action) {
			_mj_table_win(self, 1);
			self->state = GAME_END;
		}
	}
	else if (self->state ==GAME_PICKING) {
		if (ACTION_PICK == player_action) {
			_mj_table_pick(self, tile_pool_pop_a_tile(self->tile_pool));
			self->state = GAME_PICKED;
		}
		else if (ACTION_WIN == player_action) {
			_mj_table_win(self, 1);
			self->state = GAME_END;
		}
		else if (ACTION_PONG == player_action) {
			_mj_table_pong(self);
			self->state = GAME_PICKED;
		}
		else if (ACTION_CHOW == player_action) {
			if(_mj_table_chow(self, action_tile))
				self->state = GAME_PICKED;
		}
	}
}
