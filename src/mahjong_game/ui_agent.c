#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ui_agent.h"
#include "player.h"
#include "assert.h"
#include "mj_table.h"

#define UI_EVENT_QUEUE_SIZE 10
typedef struct ui_agent_t {
	agent_t agent;
	ui_event_t event_queue[UI_EVENT_QUEUE_SIZE];
	int event_queue_head;
	int event_queue_tail;
	action_t action;
	tile_t last_tile;
	tile_t action_tile;
	player_t * players[MAX_NUMBER_OF_PLAYER];
	mj_table_t * game_flow;
} ui_agent_t;

static void _remove_players(agent_t * self) {
	int i;
	ui_agent_t * ui_self = (ui_agent_t*) self;
	for (i = 0; i < MAX_NUMBER_OF_PLAYER; i++) {
		if (NULL != ui_self->players[i])
			player_destroy(ui_self->players[i]);
		ui_self->players[i] = NULL;
	}
}
static void ui_agent_destroy(agent_t * self) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	if (ui_self->game_flow) {
		mj_table_t * flow = ui_self->game_flow;
		ui_self->game_flow = NULL;
		mj_table_destroy(flow);
	} else {
		_remove_players(self);
		free(self);
	}
}

#define _QUEUE_NEXT_INDEX(i) (i) = (((i)+1)%(UI_EVENT_QUEUE_SIZE))
int ui_agent_pop_event(agent_t * self, ui_event_t * output_event) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	if (ui_self->event_queue_head == ui_self->event_queue_tail)
		return 0;

	ui_event_t * event = &ui_self->event_queue[ui_self->event_queue_head];
	_QUEUE_NEXT_INDEX(ui_self->event_queue_head);
	memcpy(output_event, event, sizeof(ui_event_t));

	return 1;
}

void ui_agent_deal(agent_t * self, tile_t tiles[], int n, int distance) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	player_t * player_data = create_player_data();
	player_deal(player_data, tiles, n);
	assert(MAX_NUMBER_OF_PLAYER > distance && ui_self->players[distance]== NULL);
	ui_self->players[distance] = player_data;
}

static void _ui_agent_add_event(agent_t * self, ui_event_name_t event_name,
		tile_t tiles[], int n, int score, int distance, const char * msg) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	ui_event_t * event = &ui_self->event_queue[ui_self->event_queue_tail];
	_QUEUE_NEXT_INDEX(ui_self->event_queue_tail);
	if (ui_self->event_queue_tail == ui_self->event_queue_head)
		_QUEUE_NEXT_INDEX(ui_self->event_queue_head);
	event->event = event_name;
	event->score = score;
	event->player_distance_to_me_clockwise = distance;
	memcpy(event->tiles, tiles, n * sizeof(tile_t));
	memset(event->message, 0, sizeof(event->message));
	if (msg) {
		strncpy(event->message, msg, UI_EVENT_MSG_LEN);
	}
}

static void react_after_pick(ui_agent_t * self, int distance) {
	self->last_tile = NO_TILE;
	if (distance == 0) {
		player_t * player = self->players[0];
		if (player_is_able_to_win(player, NO_TILE))
			_ui_agent_add_event((agent_t*) self, UI_EVENT_WIN_ABLE, NULL, 0, 0,
					0, NULL);
	}
}
static void ui_agent_pick(agent_t * self, tile_t tile, int distance) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	player_t * player_data = ui_self->players[distance];
	assert(player_data);
	player_pick(player_data, tile);

	_ui_agent_add_event(self, UI_EVENT_PICK, &tile, 1, 0, distance, NULL);

	react_after_pick(ui_self, distance);
}
static void ui_agent_pong(agent_t * self, tile_t tile, int distance) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	player_t * player_data = ui_self->players[distance];
	assert(player_data);
	player_pong(player_data, tile);
	ui_self->action = NO_ACTION;
	react_after_pick(ui_self, distance);
}

static int ui_agent_chow(agent_t * self, tile_t tile, tile_t with, int distance) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	player_t * player_data = ui_self->players[distance];
	assert(player_data);
	if (!player_chow(player_data, tile, with))
		return 0;
	ui_self->action = NO_ACTION;
	react_after_pick(ui_self, distance);
	return 1;
}

static void ui_agent_win(agent_t * self, int score, int distance) {
	_ui_agent_add_event(self, UI_EVENT_WIN, NULL, 0, score, distance, NULL);
}
static action_t ui_agent_get_action(struct agent_t * self, tile_t* tile) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	if (tile != NULL)
		*tile = ui_self->action_tile;
	return ui_self->action;
}

static void ui_agent_set_action(agent_t * self, action_t action, tile_t tile) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	player_t *player = ui_self->players[0];
	if (action == ACTION_WIN) {
		if (!player_is_able_to_win(player, ui_self->last_tile)) {
			_ui_agent_add_event(self, UI_EVENT_MESSAGE, NULL, 0, 0, 0,
					"Are you kidding?");
			return;
		}
	} else if (action == ACTION_PONG) {
		if (!player_is_able_to_pong(player, ui_self->last_tile)) {
			_ui_agent_add_event(self, UI_EVENT_MESSAGE, NULL, 0, 0, 0,
					"Are you kidding?");
			return;
		}
	} else if (action == ACTION_CHOW) {
		if (!player_is_able_to_chew(player, ui_self->last_tile)) {
			_ui_agent_add_event(self, UI_EVENT_MESSAGE, NULL, 0, 0, 0,
					"Are you kidding?");
			return;
		}
	}
	ui_self->action = action;
	ui_self->action_tile = tile;
	if (action == ACTION_RESTART)
		_remove_players(self);
}

static void cat_tiles_to_string(char buffer[], const tile_t * tiles, int n) {
	char tmp_tile[100];
	int i = 0;
	for (i = 0; i < n; i++) {
		sprintf(tmp_tile, "%d,", tiles[i]);
		strcat(buffer, tmp_tile);
	}
}
static void cat_eaten_to_string(char buffer[], const eaten_t * eaten, int n) {
	char tmp_tile[100];
	int i = 0;
	for (i = 0; i < n; i++) {
		sprintf(tmp_tile, "%d,", eaten[i]);
		strcat(buffer, tmp_tile);
	}
}

static void player_tiles_to_string(player_t * player, char buffer[],
		int buffer_size) {
	strcat(buffer, "[");
	tile_t tiles[MAX_HOLDING_COUNT];
	eaten_t eaten[MAX_EATEN_COUNT];
	int n = player_get_holdings(player, tiles, MAX_HOLDING_COUNT);
	cat_tiles_to_string(buffer, tiles, n);
	tiles[0] = player_get_current(player);
	cat_tiles_to_string(buffer, tiles, 1);
	n = player_get_eaten(player, eaten, MAX_EATEN_COUNT);
	cat_eaten_to_string(buffer, eaten, n);
	int len = strlen(buffer);
	if (buffer[len - 1] == ',') {
		buffer[len - 1] = '\0';
	}
	strcat(buffer, "]");
}
const char * ui_agent_get_tiles_array_string(agent_t * self, char buffer[],
		int buffer_size) {
	sprintf(buffer, "[");
	int i;
	ui_agent_t * ui_self = (ui_agent_t*) self;
	for (i = 0; i < MAX_NUMBER_OF_PLAYER; i++) {
		if (ui_self->players[i] != NULL) {
			if (i)
				strcat(buffer, ",");
			player_tiles_to_string(ui_self->players[i], buffer, buffer_size);
		}
	}
	strcat(buffer, "]");
	return buffer;
}

static void react_others_throw(ui_agent_t * self, tile_t tile, int distance) {
	if (distance != 0) {
		player_t * player = self->players[0];
		if (player_is_able_to_win(player, tile))
			_ui_agent_add_event((agent_t*) self, UI_EVENT_WIN_ABLE, NULL, 0, 0,
					0, NULL);
		if (player_is_able_to_pong(player, tile))
			_ui_agent_add_event((agent_t*) self, UI_EVENT_PONG_ABLE, NULL, 0,
					0, 0, NULL);
		if (distance == 1) {
			if (player_is_able_to_chew(player, tile))
				_ui_agent_add_event((agent_t*) self, UI_EVENT_CHOW_ABLE, NULL,
						0, 0, 0, NULL);
		}
	}
}
static void ui_agent_discard_tile(struct agent_t *self, tile_t tile,
		int distance) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	ui_self->last_tile = tile;
	player_t * player_data = ui_self->players[distance];
	if (player_data != NULL)
		player_discard_tile(player_data, tile);

	_ui_agent_add_event(self, UI_EVENT_DISCARD, &tile, 1, 0, distance, NULL);

	react_others_throw(ui_self, tile, distance);
}

void ui_agent_set_game_flow(agent_t * self, struct mj_table_t * game_flow) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	ui_self->game_flow = game_flow;
}
void ui_agent_update_game(agent_t * self) {
	ui_agent_t * ui_self = (ui_agent_t*) self;
	mj_table_update_state(ui_self->game_flow);
}

agent_t * create_ui_agent(void) {
	agent_t * agent = (agent_t *) malloc(sizeof(ui_agent_t));

	agent->destroy = ui_agent_destroy;
	agent->deal = ui_agent_deal;
	agent->pick = ui_agent_pick;
	agent->pong = ui_agent_pong;
	agent->chow = ui_agent_chow;
	agent->win = ui_agent_win;
	agent->discard_tile = ui_agent_discard_tile;
	agent->get_action = ui_agent_get_action;
	agent->set_action = ui_agent_set_action;

	ui_agent_t * self = (ui_agent_t *) agent;
	self->action = NO_ACTION;
	self->event_queue_head = self->event_queue_tail = 0;
	self->game_flow = NULL;
	self->last_tile = NO_TILE;
	self->action_tile = NO_TILE;
	int i = 0;
	for (; i < MAX_NUMBER_OF_PLAYER; i++)
		self->players[i] = NULL;

	return agent;
}
