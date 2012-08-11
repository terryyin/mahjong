/*
 * test_terry_evaluator.c
 *
 *  Created on: Jan 26, 2011
 *      Author: terry
 */
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "memory.h"
#include "stdio.h"
extern "C" {
#include "tiles.h"
#include "terry_evaluator.h"
extern tile_hash_t g_hash;
}
#define FT 0.001
TEST_GROUP(tiles_pattern)
{
	tiles_pattern_t pattern;
	tile_t tiles[14];
	void setup() {
		tile_hash_init(&g_hash);
		for(int i = 0; i < 14; i++)
			tiles[i] = MJ_CHARACTOR(5);
		pattern.base_score = 1.0;
	}
};

TEST(tiles_pattern, match_one_specific_tile_failed) {
	pattern.verb = PATTERN_VERB_IS;
	pattern.pattern_item_count = 1;
	DOUBLES_EQUAL(0.0, terry_ai_evaluate_array_by_patterns(tiles, 1, &pattern, 1), FT);
}
IGNORE_TEST(tiles_pattern, match_one_specific_tile_successful) {
	pattern.verb = PATTERN_VERB_IS;
	pattern.pattern_item_count = 1;
	pattern.tile = MJ_NORTH;
	tiles[0] = MJ_NORTH;
	DOUBLES_EQUAL(1.0, terry_ai_evaluate_array_by_patterns(tiles, 13, &pattern, 1), FT);
}

IGNORE_TEST(tiles_pattern, match_one_specific_tile_twice) {
	pattern.verb = PATTERN_VERB_IS;
	pattern.tile = MJ_CHARACTOR(2);
	pattern.pattern_item_count = 1;
	tiles[0] = MJ_CHARACTOR(2);
	tiles[1] = MJ_CHARACTOR(2);
	DOUBLES_EQUAL(2.0, terry_ai_evaluate_array_by_patterns(tiles, 13, &pattern, 1), FT);
}

TEST(tiles_pattern, match_one_any_tile) {
	pattern.verb = PATTERN_VERB_ANY;
	pattern.pattern_item_count = 1;
	DOUBLES_EQUAL(13.0, terry_ai_evaluate_array_by_patterns(tiles, 13, &pattern, 1), FT);
}

TEST(tiles_pattern, match_one_any_tile_with_one_XXX) {
	pattern.verb = PATTERN_VERB_ANY;
	pattern.pattern_item_count = 1;
	tiles[12] = MJ_NORTH;
	DOUBLES_EQUAL(12.0, terry_ai_evaluate_array_by_patterns(tiles, 13, &pattern, 1), FT);
}

TEST(tiles_pattern, match_one_any_tile_and_one_relative) {
	pattern.verb = PATTERN_VERB_ANY;
	pattern.following_items[0] = 0;
	pattern.pattern_item_count = 2;
	DOUBLES_EQUAL(6.0, terry_ai_evaluate_array_by_patterns(tiles, 13, &pattern, 1), FT);
}

TEST(tiles_pattern, match_one_any_tile_and_one_relative_fail) {
	pattern.verb = PATTERN_VERB_ANY;
	pattern.following_items[0] = 1;
	pattern.pattern_item_count = 2;
	DOUBLES_EQUAL(0.0, terry_ai_evaluate_array_by_patterns(tiles, 13, &pattern, 1), FT);
}

TEST(tiles_pattern, match_relative_with_repeat) {
	pattern.verb = PATTERN_VERB_ANY;
	pattern.following_items[0] = 2;
	pattern.pattern_item_count = 2;
	tiles[0] = MJ_BAMBOO(3);
	tiles[1] = MJ_BAMBOO(3);
	tiles[2] = MJ_BAMBOO(5);

	DOUBLES_EQUAL(1.0, terry_ai_evaluate_array_by_patterns(tiles, 13, &pattern, 1), FT);
}

TEST_GROUP(evaluator_values)
{
	void setup() {
	}
	tile_t c2t(char c) {
		if (c>='1'&& c <='9')
			return MJ_CHARACTOR(c-'0');
		if (c>='A'&& c <='I')
			return MJ_CIRCLE(c-'A'+1);
		if (c>='a'&& c <='i')
			return MJ_BAMBOO(c-'a'+1);
		if (c == 'T')
			return MJ_EAST;
		if (c == 'U')
			return MJ_SOUTH;

		return NO_TILE;
	}
	int e(const char * ts) {
		tile_t tiles[14];
		int i= 0;
		for (; ts[i]; i++) tiles[i] = c2t(ts[i]);
		return terry_ai_evaluate_array(tiles, i);
	}
	void compare_tile_strings_bigger(const char *t1, const char *t2, const char * file, int line) {
		int s1 = e(t1);
		int s2 = e(t2);
		char temp[200];
		sprintf(temp, "t1(%d) > t2(%d)", s1, s2);
		CHECK_C_LOCATION(s1 > s2, temp, file, line);
	}
};
#define T1_BIGGER_THAN_T2(t1, t2)\
		compare_tile_strings_bigger(t1, t2, __FILE__, __LINE__)
TEST(evaluator_values, one_tile) {
	T1_BIGGER_THAN_T2("2","1");
}
TEST(evaluator_values, two_tiles) {
	T1_BIGGER_THAN_T2("33","38");
	T1_BIGGER_THAN_T2("34","38");
	T1_BIGGER_THAN_T2("34","35");
	T1_BIGGER_THAN_T2("35","36");
	T1_BIGGER_THAN_T2("11","35");
	T1_BIGGER_THAN_T2("35","13");
	T1_BIGGER_THAN_T2("13","25");
	T1_BIGGER_THAN_T2("25","14");
	T1_BIGGER_THAN_T2("11","TT");
	T1_BIGGER_THAN_T2("TT","25");
}
TEST(evaluator_values, 3_tiles) {
	T1_BIGGER_THAN_T2("246","247");
	T1_BIGGER_THAN_T2("246","135");
	T1_BIGGER_THAN_T2("235","246");
	T1_BIGGER_THAN_T2("245","246");
//	T1_BIGGER_THAN_T2("134","135");
	T1_BIGGER_THAN_T2("579","479");

	T1_BIGGER_THAN_T2("118","258");
	T1_BIGGER_THAN_T2("229","258");
	T1_BIGGER_THAN_T2("118","248");
	T1_BIGGER_THAN_T2("113","118");
	T1_BIGGER_THAN_T2("113","128");
	T1_BIGGER_THAN_T2("113","228");
	T1_BIGGER_THAN_T2("224","113");
	T1_BIGGER_THAN_T2("223","224");

	T1_BIGGER_THAN_T2("123","223");
	T1_BIGGER_THAN_T2("234","123");

	T1_BIGGER_THAN_T2("111","223");
	T1_BIGGER_THAN_T2("222","111");
	T1_BIGGER_THAN_T2("222","123");
	T1_BIGGER_THAN_T2("111","TTT");
}

TEST(evaluator_values, 4_tiles) {
//	T1_BIGGER_THAN_T2("1357","13BD");
	T1_BIGGER_THAN_T2("2468","1357");
	LONGS_EQUAL(e("1357"), e("3579"))
	T1_BIGGER_THAN_T2("2467","2468");
	T1_BIGGER_THAN_T2("2357","2468");
	T1_BIGGER_THAN_T2("2346","2357");
	T1_BIGGER_THAN_T2("3445","344B");
	T1_BIGGER_THAN_T2("34BC","344A");
	T1_BIGGER_THAN_T2("3446","344B");
	T1_BIGGER_THAN_T2("65777cdefghUU","35777cdefghUU");
}

/*
 * todo: use this
extern "C"{
#include "assess.h"
}
TEST_GROUP(assess) {

};

IGNORE_TEST(assess, 1) {
	srand(100);
	LONGS_EQUAL(5670, assess_evaluator(10000));
}

TEST_GROUP(tile_hash) {
	tile_hash_t h;
	tile_hash_t * hash;
	tile_hash_value_t item;
	void setup(){
		hash = &h;
		tile_hash_init(hash);
	}
};

TEST(tile_hash, get_from_empty) {
	tile_t tiles[1] = {C(1)};
	LONGS_EQUAL(TILE_HASH_NO_VALUE, tile_hash_get(hash, tiles, 1));
}
TEST(tile_hash, get_from_empty_and_set_and_get_again) {
	tile_t tiles[1] = {C(1)};
	tile_hash_set(hash, tiles, 1, 1);
	LONGS_EQUAL(1,tile_hash_get(hash, tiles, 1));
}
TEST(tile_hash, set_2) {
	tile_t t1[] = {C(1), C(2)};
	tile_t t2[] = {C(1), C(3)};
	tile_hash_set(hash, t1, 2, 1);
	tile_hash_set(hash, t2, 2, 2);
	LONGS_EQUAL(1,tile_hash_get(hash, t1, 2));
	LONGS_EQUAL(2,tile_hash_get(hash, t2, 2));
}


*/


