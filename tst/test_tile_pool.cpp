#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "tile_pool.h"
}

TEST_BASE(base_tile_pool) {
	tile_pool_t * game;
	void setup() {
		game = create_tile_pool();
	}
	void teardown() {
		tile_pool_destroy(game);
	}
};

TEST_GROUP_BASE(tile_pool, base_tile_pool)
{};

TEST(tile_pool, not_end)
{
	LONGS_EQUAL(0, tile_pool_is_end(game));
}

TEST(tile_pool, end)
{
	for (int i = 0; i < MAX_PICKS-1; i++) {
		tile_pool_pop_a_tile(game);
		LONGS_EQUAL(0, tile_pool_is_end(game));
	}

	tile_pool_pop_a_tile(game);
	LONGS_EQUAL(1, tile_pool_is_end(game));
}

TEST_GROUP_BASE(tile_pool_shuffle, base_tile_pool)
{
	tile_t all[ALL_COUNT];
	void setup() {
		base_tile_pool::setup();
		shuffle_and_get_all_tiles();
	}
	void shuffle_and_get_all_tiles (){
		srand(100);
		int cnt = 0;
		tile_pool_shuffle(game);
		while (ALL_COUNT > cnt) all[cnt++] = tile_pool_pop_a_tile(game);
	}
	double average_of_array(tile_t tiles[], int from, int count) {
		double sum = 0;
		for (int i = from; i < from + count; i++)
			sum += tiles[i];
		return sum / count;
	}
};

TEST(tile_pool_shuffle, check_sum_of_shuffle)
{
	int checksum = 0;
	for (int i = 0; i < ALL_COUNT; i++) checksum+=all[i];
//	LONGS_EQUAL(11304, checksum);
}

