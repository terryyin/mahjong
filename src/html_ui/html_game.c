#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mahjong_game.h"
#include "html_game.h"
#include "agent.h"
#include "ui_agent.h"

static int ui_adaptor_pool_add_ui_adaptor(agent_t * ui);
static agent_t * ui_adaptor_pool_get_ui_adaptor_by_id(int id);
static void ui_adaptor_pool_remove(int id);

const char * html_frame =
		"<html>\n"
		"<head>\n"
		"<title>Mahjong Game - For Writing Effective Unit Test Training</title> "
		"<script>game_id=%d;</script>\n"
		"<LINK REL=StyleSheet HREF=\"mj.css\" TYPE=\"text/css\">\n"
		"<script src=\"mj.js\" type=\"text/javascript\"></script>\n"
		"</head>\n"
		"<body>\n"
		"</body>\n"
		"</html>\n";

int start_new_player(char * buffer, int buffer_size)
{
	agent_t * ui = game_builder_join_new_game_with_one_ai_player(NULL);
	int player_id = ui_adaptor_pool_add_ui_adaptor(ui);
	if (player_id == 0)
		ui->destroy(ui);

	snprintf(buffer, buffer_size, html_frame, player_id);

	return player_id;
}

void do_user_do_not_exist_error(char * buffer, int buffer_size)
{
	snprintf(buffer, buffer_size, "alert('Game does not exist. Restart, please.');");
}

void script_to_update_all_holdings(int player_id, char * buffer, int buffer_size)
{
	agent_t * agent = ui_adaptor_pool_get_ui_adaptor_by_id(player_id);
	if (!agent) {
		do_user_do_not_exist_error(buffer, buffer_size);
		return;
	}
	char tmp[1024]="";
	ui_agent_get_tiles_array_string(agent, tmp, 1024);
	snprintf(buffer, buffer_size, "App.UpdateHolding(%s);", tmp);

}

void generate_ui_event_script(int player_id, char * buffer, int buffer_size)
{
	agent_t * agent = ui_adaptor_pool_get_ui_adaptor_by_id(player_id);
	if (!agent) {
		do_user_do_not_exist_error(buffer, buffer_size);
		return;
	}

	ui_agent_update_game(agent);
	script_to_update_all_holdings(player_id, buffer, buffer_size);
	ui_event_t event;
	char tmp[1024];
	while (ui_agent_pop_event(agent, &event)) {
		switch (event.event) {
		case UI_EVENT_DISCARD:
			sprintf(tmp, "App.Throw(%d, %d);", event.tiles[0], event.player_distance_to_me_clockwise);
			if (1 == event.player_distance_to_me_clockwise)
				strcat(tmp, "App.StopUpdate();");
			else
				strcat(tmp, "App.ResumeUpdate();");

			break;
		case UI_EVENT_WIN:
			sprintf(tmp, "App.WinAck(%d, %d);",0, event.score);
			break;
		case UI_EVENT_PICK:
			sprintf(tmp, "App.Pick(%d, %d);", event.player_distance_to_me_clockwise, event.tiles[0]);
			if (0 == event.player_distance_to_me_clockwise)
				strcat(tmp, "App.StopUpdate();");
			else
				strcat(tmp, "App.ResumeUpdate();");
			break;
		case UI_EVENT_MESSAGE:
			sprintf(tmp, "alert(\"%s\");",event.message);
			break;
		case UI_EVENT_CHOW_ABLE:
			sprintf(tmp, "App.LightButton('chow');");
			break;
		case UI_EVENT_PONG_ABLE:
			sprintf(tmp, "App.LightButton('pong');");
			break;
		case UI_EVENT_WIN_ABLE:
			sprintf(tmp, "App.LightButton('win');");
			break;
		default:
			sprintf(tmp, "'invalid event%d';", event.event);
		}

		strcat(buffer, tmp);
	}
}

void html_game_do_action(int player_id, char * buffer, int buffer_size, action_t action, tile_t tile)
{
	agent_t * agent = ui_adaptor_pool_get_ui_adaptor_by_id(player_id);
	if (!agent) {
		do_user_do_not_exist_error(buffer, buffer_size);
		return;
	}
	ui_agent_update_game(agent);
	agent->set_action(agent, action, tile);
}

void script_to_bye(int player_id, char * buffer, int buffer_size, action_t action, tile_t tile)
{
	ui_adaptor_pool_remove(player_id);
	buffer[0] = 0;
}


void show_byebye(char * buffer, int buffer_size)
{
	shutdown_server();
	snprintf(buffer, buffer_size, "<HTML><BODY>Bye!</BODY></HTML>");

}

int execute_game_command(const char * command, const char *parameters, char * buffer, int buffer_size)
{
	int player_id = 0;
	tile_t tile = NO_TILE;
	const char * cmd = strchr(command, '/');
//	printf("cmd:%s, with:%s\n", command, parameters);

	buffer[0]='\0';
	if (parameters != NULL && parameters[0]>='0' && parameters[0]<='9') {
		int param = atoi(parameters);
		player_id = param / 1000;
		tile = param % 1000;
	}
	if (strcmp(cmd, "/game")==0) {
		player_id = start_new_player(buffer, buffer_size);
	}
	else if (strcmp(cmd, "/bye")==0) {
		script_to_bye(player_id, buffer, buffer_size, ACTION_WIN, NO_TILE);
	}
	else if (strcmp(cmd, "/shutdown")==0) {
		show_byebye(buffer, buffer_size);
	}
	else {
		if (strcmp(cmd, "/start")==0) {
			char temp[1024];
			html_game_do_action(player_id, temp, 1024, ACTION_RESTART, NO_TILE);
		}
		else if (strcmp(cmd, "/update")==0) {
		}
		else if (strcmp(cmd, "/pick")==0) {
			html_game_do_action(player_id, buffer, buffer_size, ACTION_PICK, NO_TILE);
		}
		else if (strcmp(cmd, "/throw")==0) {
			html_game_do_action(player_id, buffer, buffer_size, ACTION_DISCARD, tile);
		}
		else if (strcmp(cmd, "/chow")==0) {
			html_game_do_action(player_id, buffer, buffer_size, ACTION_CHOW, tile);
		}
		else if (strcmp(cmd, "/pong")==0) {
			html_game_do_action(player_id, buffer, buffer_size, ACTION_PONG, NO_TILE);
		}
		else if (strcmp(cmd, "/kong")==0) {
			html_game_do_action(player_id, buffer, buffer_size, ACTION_KONG, tile);
		}
		else if (strcmp(cmd, "/win")==0) {
			html_game_do_action(player_id, buffer, buffer_size, ACTION_WIN, NO_TILE);
		}
		else
			return 0;
		generate_ui_event_script(player_id, buffer, buffer_size);
	}
//	printf("%s\n", buffer);
	return player_id;
}

/*************************************
 * ui_adaptor  pool */
#define UI_ADAPTOR_POOL_SIZE  100

typedef struct {
	agent_t * ui;
	int id;
} ui_pool_item_t;

static ui_pool_item_t g_ui_adaptor_pool[UI_ADAPTOR_POOL_SIZE];
static int g_ui_adaptor_id_counter = 1;

static ui_pool_item_t * ui_adaptor_pool_get_item_by_id(int id)
{
	int i;
	for (i=0; i < UI_ADAPTOR_POOL_SIZE; i++)
		if (g_ui_adaptor_pool[i].id == id)
			return &g_ui_adaptor_pool[i];
	return NULL;
}

static int ui_adaptor_pool_add_ui_adaptor(agent_t * ui)
{
	ui_pool_item_t * item = ui_adaptor_pool_get_item_by_id(0);

	if (item == NULL)
		return 0;
	item->ui = ui;
	item->id = g_ui_adaptor_id_counter++;
	return item->id;
}

static agent_t * ui_adaptor_pool_get_ui_adaptor_by_id(int id)
{
	ui_pool_item_t * item = ui_adaptor_pool_get_item_by_id(id);

	if (item == NULL)
		return NULL;
	return item->ui;
}
static void ui_adaptor_pool_remove(int id)
{
	ui_pool_item_t * item = ui_adaptor_pool_get_item_by_id(id);

	if (item != NULL){
		item->id = 0;
		item->ui->destroy(item->ui);
	}
}
