#include "CppUTest/TestHarness.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "tiles.h"
}

const int tile_array_size = 14;
TEST_GROUP(tiles_array)
{
	tile_t tile_array[tile_array_size];
	void setup() {
		for (int i = 0; i < tile_array_size; i++)
			tile_array[i] = NO_TILE;
	}
};


TEST(tiles_array, winning_when_empty) {
	CHECK(tiles_plus_one_is_winning(tile_array, 0, NO_TILE));
}

TEST(tiles_array, NOT_winning_when_one_tile_left) {
	tile_array[0] = MJ_CHARACTOR(1);
	CHECK(!tiles_plus_one_is_winning(tile_array, 1, NO_TILE));
}

TEST(tiles_array, winning_when_a_pair) {
	tile_array[0] = MJ_CHARACTOR(1);
	CHECK(tiles_plus_one_is_winning(tile_array, 1, MJ_CHARACTOR(1)));
}

TEST(tiles_array, not_winning_when_2_different) {
	tile_array[0] = MJ_CHARACTOR(1);
	CHECK(!tiles_plus_one_is_winning(tile_array, 1, MJ_CHARACTOR(2)));
}

TEST(tiles_array, not_winning_when__3_different) {
	tile_array[0] = MJ_CHARACTOR(2);
	tile_array[1] = MJ_CHARACTOR(5);
	CHECK(!tiles_plus_one_is_winning(tile_array, 2, MJ_CHARACTOR(8)));
}
TEST(tiles_array, not_winning_when__a_pair_and_one) {
	tile_array[0] = MJ_CHARACTOR(2);
	tile_array[1] = MJ_CHARACTOR(2);
	CHECK(!tiles_plus_one_is_winning(tile_array, 2, MJ_CHARACTOR(8)));
}
TEST(tiles_array, winning_when_a_pong) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(1);
	CHECK(tiles_plus_one_is_winning(tile_array, 2, MJ_CHARACTOR(1)));
}

TEST(tiles_array, not_winning_when_4) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(1);
	tile_array[2] = MJ_CHARACTOR(1);
	CHECK(!tiles_plus_one_is_winning(tile_array, 3, MJ_CHARACTOR(8)));
}

TEST(tiles_array, winning_when_a_pong_and_a_pair) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(1);
	tile_array[2] = MJ_CHARACTOR(1);
	tile_array[3] = MJ_CHARACTOR(8);
	CHECK(tiles_plus_one_is_winning(tile_array, 4, MJ_CHARACTOR(8)));
}

TEST(tiles_array, winning_when_a_pair_and_a_pong) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(1);
	tile_array[2] = MJ_CHARACTOR(8);
	tile_array[3] = MJ_CHARACTOR(8);
	CHECK(tiles_plus_one_is_winning(tile_array, 4, MJ_CHARACTOR(8)));
}

TEST(tiles_array, winning_when_a_a_chew) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(2);
	CHECK(tiles_plus_one_is_winning(tile_array, 2, MJ_CHARACTOR(3)));
}

TEST(tiles_array, winning_when_a_a_chew_and_a_pair_within) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(2);
	tile_array[2] = MJ_CHARACTOR(2);
	tile_array[3] = MJ_CHARACTOR(2);
	CHECK(tiles_plus_one_is_winning(tile_array, 4, MJ_CHARACTOR(3)));
}

TEST(tiles_array, winning_when_a_a_chew_and_a_pair_and_a_pong) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(1);
	tile_array[2] = MJ_CHARACTOR(1);
	tile_array[3] = MJ_CHARACTOR(2);
	tile_array[4] = MJ_CHARACTOR(2);
	tile_array[5] = MJ_CHARACTOR(2);
	tile_array[6] = MJ_CHARACTOR(2);
	CHECK(tiles_plus_one_is_winning(tile_array, 7, MJ_CHARACTOR(3)));
}

TEST(tiles_array, not_winning_when_2_pairs) {
	tile_array[0] = MJ_CHARACTOR(1);
	tile_array[1] = MJ_CHARACTOR(1);
	tile_array[2] = MJ_CHARACTOR(3);

	CHECK(!tiles_plus_one_is_winning(tile_array, 3, MJ_CHARACTOR(3)));
}
TEST(tiles_array, a_winning_case) {
	tile_t t[] = {C(2), C(3), C(4), R(2), R(2), R(6), R(7), B(2), B(2), B(2), B(7),B(8), B(9)};
	memcpy(tile_array, t, sizeof(t));
	CHECK(tiles_plus_one_is_winning(tile_array, 13, R(5)));
}
