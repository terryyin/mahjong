#ifndef UI_AGENT_H_
#define UI_AGENT_H_

#include "tiles.h"
#include "agent.h"
typedef struct user_info_t{

} user_info_t;

typedef enum {
	UI_EVENT_INVALID,
	UI_EVENT_DISCARD,
	UI_EVENT_PICK_ABLE,
	UI_EVENT_PONG_ABLE,
	UI_EVENT_CHOW_ABLE,
	UI_EVENT_WIN_ABLE,
	UI_EVENT_MESSAGE,
	UI_EVENT_WIN,
	UI_EVENT_PICK
}ui_event_name_t;

#define UI_EVENT_MSG_LEN 100
typedef struct {
	ui_event_name_t event;
	int score;
	int player_distance_to_me_clockwise;
	tile_t tiles[MAX_HOLDING_COUNT];
	char message[UI_EVENT_MSG_LEN+1];
} ui_event_t;

struct mj_table_t;

agent_t * create_ui_agent(void);
int ui_agent_pop_event(agent_t * self, ui_event_t * output_event);
const char * ui_agent_get_tiles_array_string(agent_t * self, char buffer[], int buffer_size);
void ui_agent_set_game_flow(agent_t * self, struct mj_table_t * game_flow);
void ui_agent_update_game(agent_t * self);

#endif /* UI_AGENT_H_ */
