#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "html_game.h"
#include "tile_pool.h"
#include "evaluator.h"
evaluator_t * create_simple_evaluator_r();
void set_cheapest_tile(tile_t tile);
}
#include "mocks.h"

const int buffer_size = 1024;
TEST_GROUP(html_game_new_player)
{
	char buffer[buffer_size];
	void setup() {
	}
	char * itoa(int id) {
		static char temp[10];
		sprintf(temp, "%d", id);
		return temp;
	}
};

TEST(html_game_new_player, new_player)
{
	int id = execute_game_command("/game", "", buffer, buffer_size);
	CHECK(id);
	LONGS_EQUAL(id, execute_game_command("/bye", itoa(id*1000), buffer, buffer_size));
}

TEST(html_game_new_player, mulitple_games)
{
	int id = execute_game_command("/game", "", buffer, buffer_size);
	CHECK(id);
	LONGS_EQUAL(id + 1, execute_game_command("/game", "", buffer, buffer_size));

	LONGS_EQUAL(id, execute_game_command("/bye", itoa(id*1000), buffer, buffer_size));
	LONGS_EQUAL(id + 1, execute_game_command("/bye", itoa((id+1)*1000), buffer, buffer_size));
}

static tile_t g_current_tile;
static tile_t tile_pool_pop_a_tile_stub(tile_pool_t * self) {
	return g_current_tile++;
}
static int g_tile_pool_end;
static int tile_pool_is_end_stub(tile_pool_t * game) {
	return g_tile_pool_end;
}

#define HAS_STRING(expect, actual) HAS_STRING_LOCATION(expect, actual, __FILE__, __LINE__)
TEST_GROUP(html_game)
{
	char buffer[buffer_size];
	int player_id;
	void setup() {
		g_current_tile = 1;
		g_tile_pool_end = 0;
		UT_PTR_SET(tile_pool_pop_a_tile, tile_pool_pop_a_tile_stub);
		UT_PTR_SET(tile_pool_is_end, tile_pool_is_end_stub);
		UT_PTR_SET(create_evaluator_r, create_simple_evaluator_r);
		player_id = execute_game_command("/game", "", buffer, buffer_size);

	}
	void teardown() {
		execute_cmd("/bye", 0);
	}
	char * idtoa(int id, int tile) {
		static char temp[10];
		sprintf(temp, "%d", id * 1000 + tile);
		return temp;
	}
	void execute_cmd(const char * cmd, int tile) {
		execute_game_command(cmd, idtoa(player_id, tile), buffer, buffer_size);
	}
	void HAS_STRING_LOCATION(const char * expect, const char * actual, const char * filename, int line) {
		if (strstr(actual, expect) == NULL) {
			const int buffer_size = 500;
			char text[buffer_size];
			snprintf(text, buffer_size - 1, "The actual string <%s>\n"
					"does not include the expected string <%s>", actual, expect);
			FAIL_LOCATION(text, filename, line);
		}
	}
};

TEST(html_game, start)
{
	execute_cmd("/start", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[1,2,3,4,5,6,7,8,9,10,11,12,13,27],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);"
			"App.Pick(0, 27);"
			"App.StopUpdate();"
			, buffer);
}
TEST(html_game, a_game)
{
	execute_cmd("/start", 0);
	execute_cmd("/throw", 1);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[2,3,4,5,6,7,8,9,10,11,12,13,27,0],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);"
			"App.Throw(1, 0);"
			"App.ResumeUpdate();"
			, buffer);
	set_cheapest_tile(28);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[2,3,4,5,6,7,8,9,10,11,12,13,27,0],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,28]]);"
			"App.Pick(1, 28);"
			"App.ResumeUpdate();"
			, buffer);
	execute_cmd("/update", 0);
	HAS_STRING("App.UpdateHolding("
			"[[2,3,4,5,6,7,8,9,10,11,12,13,27,0],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);"
			"App.Throw(28, 1);"
			"App.StopUpdate();"
			, buffer);
	g_current_tile = 27;
	execute_cmd("/pick", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[2,3,4,5,6,7,8,9,10,11,12,13,27,27],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);"
			"App.Pick(0, 27);"
			"App.StopUpdate();"
			"App.LightButton('win');"
			, buffer);
	execute_cmd("/win", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[2,3,4,5,6,7,8,9,10,11,12,13,27,27],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);"
			"App.WinAck(0, 1);"
			, buffer);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[2,3,4,5,6,7,8,9,10,11,12,13,27,27],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);"
			, buffer);
	g_current_tile = 1;
	set_cheapest_tile(27);
	execute_cmd("/start", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[14,15,16,17,18,19,20,21,22,23,24,25,26,0],"
			"[1,2,3,4,5,6,7,8,9,10,11,12,13,27]]);"
			"App.Pick(1, 27);"
			"App.ResumeUpdate();"
			, buffer);
	execute_cmd("/update", 0);
}
TEST(html_game, no_tile_any_more)
{
	execute_cmd("/start", 0);
	g_tile_pool_end = 1;
	execute_cmd("/throw", 1);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[2,3,4,5,6,7,8,9,10,11,12,13,27,0],"
			"[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);"
			"App.WinAck(0, 0);"
			, buffer);
	set_cheapest_tile(54);
	execute_cmd("/start", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[41,42,43,44,45,46,47,48,49,50,51,52,53,0],"
			"[28,29,30,31,32,33,34,35,36,37,38,39,40,54]]);"
			"App.Pick(1, 54);"
			"App.ResumeUpdate();"
			, buffer);
	execute_cmd("/update", 0);
	g_tile_pool_end = 1;
	execute_cmd("/pick", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[41,42,43,44,45,46,47,48,49,50,51,52,53,0],"
			"[28,29,30,31,32,33,34,35,36,37,38,39,40,0]]);"
			"App.WinAck(0, 0);"
			, buffer);
	execute_cmd("/start", 0);
	STRCMP_EQUAL("App.UpdateHolding("
			"[[55,56,57,58,59,60,61,62,63,64,65,66,67,81],"
			"[68,69,70,71,72,73,74,75,76,77,78,79,80,0]]);"
			"App.Pick(0, 81);"
			"App.StopUpdate();"
			, buffer);
}
TEST(html_game, _WIN)
{
	execute_cmd("/start", 0);
	execute_cmd("/win", 1);
	STRCMP_EQUAL("App.UpdateHolding([[1,2,3,4,5,6,7,8,9,10,11,12,13,27],[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);alert(\"Are you kidding?\");"
			, buffer);
	execute_cmd("/throw", 1);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);App.Throw(1, 0);App.ResumeUpdate();"
			, buffer);
	set_cheapest_tile(28);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,28]]);App.Pick(1, 28);App.ResumeUpdate();"
			, buffer);
	execute_cmd("/update", 0);
	HAS_STRING("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);App.Throw(28, 1);App.StopUpdate();"
			, buffer);
	execute_cmd("/win", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);alert(\"Are you kidding?\");"
			, buffer);
	execute_cmd("/pick", 0);
	execute_cmd("/throw", 29);
	g_current_tile = 27;
	set_cheapest_tile(27);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	execute_cmd("/win", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);App.WinAck(0, 1);"
			, buffer);
}
TEST(html_game, ai_WIN)
{
	execute_cmd("/start", 0);
	execute_cmd("/throw", 1);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,0]]);App.Throw(1, 0);App.ResumeUpdate();"
			, buffer);
	g_current_tile = 14;
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,14]]);App.Pick(1, 14);App.ResumeUpdate();"
			, buffer);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[14,15,16,17,18,19,20,21,22,23,24,25,26,14]]);App.WinAck(0, 1);"
			, buffer);
	execute_cmd("/start", 0);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	g_current_tile = 41;
	execute_cmd("/pick", 0);
	execute_cmd("/throw", 41);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding([[28,29,30,31,32,33,34,35,36,37,38,39,40,0],[16,17,18,19,20,21,22,23,24,25,26,27,41,0]]);App.WinAck(0, 1);"
			, buffer);
}

IGNORE_TEST(html_game, pong)
{
	execute_cmd("/start", 0);
	execute_cmd("/throw", 1);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	execute_cmd("/pong", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,3,4,5,6,7,8,9,10,11,12,13,27,0],[15,16,17,18,19,20,21,22,23,24,25,26,28,0]]);alert(\"Are you kidding?\");"
			, buffer);
	g_current_tile = 2;
	execute_cmd("/pick", 0);
	execute_cmd("/throw", 3);
	g_current_tile = 2;
	set_cheapest_tile(2);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	execute_cmd("/update", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,2,4,5,6,7,8,9,10,11,12,13,27,0],[15,16,17,18,19,20,21,22,23,24,25,26,28,0]]);App.Throw(2, 1);App.StopUpdate();App.LightButton('pong');"
			, buffer);
	execute_cmd("/chow", 0);
	STRCMP_EQUAL("App.UpdateHolding([[2,2,4,5,6,7,8,9,10,11,12,13,27,0],[15,16,17,18,19,20,21,22,23,24,25,26,28,0]]);alert(\"Are you kidding?\");"
			, buffer);
	execute_cmd("/pong", 0);
	STRCMP_EQUAL("App.UpdateHolding([[4,5,6,7,8,9,10,11,12,13,27,130],[15,16,17,18,19,20,21,22,23,24,25,26,28,0]]);"
			, buffer);
	execute_cmd("/throw", 4);
	STRCMP_EQUAL("App.UpdateHolding([[5,6,7,8,9,10,11,12,13,27,0,130],[15,16,17,18,19,20,21,22,23,24,25,26,28,0]]);App.Throw(4, 0);App.ResumeUpdate();"
			, buffer);
}

TEST_GROUP(html_game1)
{

};
