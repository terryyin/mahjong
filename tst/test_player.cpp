#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "tiles.h"
#include "player.h"
#include "mj_table.h"
}
#include "mocks.h"

TEST_GROUP(common_player) {
	player_t * player;
	tile_t tiles[100];
	void setup() {
		player=create_player_data();

	}
	void teardown() {
		player_destroy(player);
	}
};

TEST(common_player, get_tiles_when_not_dealt) {
	tile_t tiles[14];
	LONGS_EQUAL(0, player_get_holdings(player, tiles, 14));
	LONGS_EQUAL(NO_TILE, player_get_current(player));
}

TEST(common_player, get_tiles_when_dealt) {
	tile_t tiles_of_deal[4] = {C(3), C(4), C(1), C(2)};
	tile_t tiles[14];
	player_deal(player, tiles_of_deal, 4);
	LONGS_EQUAL(4, player_get_holdings(player, tiles, 14));
	LONGS_EQUAL(C(1), tiles[0]);
	LONGS_EQUAL(C(4), tiles[3]);
	LONGS_EQUAL(NO_TILE, player_get_current(player));
}
TEST(common_player, eaten_info_has_been_reset_after_deal) {
	eaten_t eaten;
	tile_t tiles_of_deal[] = {C(3)};
	player_pong(player, C(1));
	player_deal(player, tiles_of_deal, 1);
	LONGS_EQUAL(0, player_get_eaten(player, &eaten, 1));
}

TEST(common_player, is_able_to_pong) {
	tile_t tiles1[] = {MJ_EAST, MJ_EAST};
	player_deal(player, tiles1, 2);
	LONGS_EQUAL(1, player_is_able_to_pong(player, MJ_EAST));
	tile_t tiles2[] = {MJ_EAST, MJ_NORTH, MJ_NORTH, MJ_NORTH};
	player_deal(player, tiles2, 4);
	LONGS_EQUAL(0, player_is_able_to_pong(player, MJ_EAST));
}

TEST(common_player, pong) {
	tile_t tiles1[] = {MJ_EAST, MJ_EAST, MJ_NORTH};
	eaten_t eaten[3];
	player_deal(player, tiles1, 3);
	player_pong(player, MJ_EAST);
	LONGS_EQUAL(0,player_get_holdings(player, tiles, 3));
	LONGS_EQUAL(1,player_get_eaten(player, eaten, 3));
	LONGS_EQUAL(PONG(MJ_EAST), eaten[0]);
}
TEST(common_player, chow_fail) {
	tile_t tiles1[] = {1, 2, 3};
	eaten_t eaten[3];
	player_deal(player, tiles1, 3);
	LONGS_EQUAL(0, player_chow(player, 4, 3));
	LONGS_EQUAL(3,player_get_holdings(player, tiles, 3));
	LONGS_EQUAL(0,player_get_eaten(player, eaten, 3));
}
TEST(common_player, chow_middle) {
	tile_t tiles1[] = {1, 3};
	eaten_t eaten[3];
	player_deal(player, tiles1, 2);
	LONGS_EQUAL(1, player_chow(player, 2, 1));
	LONGS_EQUAL(0,player_get_holdings(player, tiles, 3));
	LONGS_EQUAL(1,player_get_eaten(player, eaten, 3));
	LONGS_EQUAL(CHOW(1), eaten[0]);
}
