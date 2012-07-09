#include "CppUTest/TestHarness.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "tiles.h"
}
/*
TEST_GROUP(is_able_to_chow) {

};

TEST(is_able_to_chow, not_able_to_chow_when_there_is_only_one_tile)
{
	tile_t tiles[] = {C(1)};
	LONGS_EQUAL(0, tiles_is_able_to_chow(tiles, 1, C(3)));
}

TEST(is_able_to_chow, able_to_chow_4_with_3_5)
{
	tile_t tiles[] = {C(1),C(2),C(3),C(5)};
	LONGS_EQUAL(1, tiles_is_able_to_chow(tiles, 4, C(4)));
}

TEST(is_able_to_chow, not_able_to_chow_when_there_are_more_than_one_tiles)
{
	tile_t tiles[] = {C(1),C(3),C(5),C(7)};
	LONGS_EQUAL(0, tiles_is_able_to_chow(tiles, 4, C(3)));
}

TEST(is_able_to_chow, able_to_chow_1_with_2_3)
{
	tile_t tiles[] = {C(2),C(3),C(5),C(7)};
	LONGS_EQUAL(1, tiles_is_able_to_chow(tiles, 4, C(1)));
}

TEST(is_able_to_chow, able_to_chow_3_with_1_2)
{
	tile_t tiles[] = {C(1),C(2),C(5),C(7)};
	LONGS_EQUAL(1, tiles_is_able_to_chow(tiles, 4, C(3)));
}

TEST(is_able_to_chow, able_to_chow_3_with_2_3_4)
{
	tile_t tiles[] = {C(2),C(3),C(4),C(7)};
	LONGS_EQUAL(1, tiles_is_able_to_chow(tiles, 4, C(3)));
}

TEST(is_able_to_chow, able_to_chow_3_with_2_3_3_4)
{
	tile_t tiles[] = {C(2),C(3),C(3),C(4)};
	LONGS_EQUAL(1, tiles_is_able_to_chow(tiles, 4, C(3)));
}

TEST(is_able_to_chow, not_able_to_chow_3_with_2_3_3_5)
{
	tile_t tiles[] = {C(2),C(3),C(3),C(5)};
	LONGS_EQUAL(0, tiles_is_able_to_chow(tiles, 4, C(3)));
}

*/
