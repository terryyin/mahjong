#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "exercise_3.h"
}

//static int g_score[2];
//#define INVALID_SCORE -1
//
//void clear_user_score(){
//	int len = sizeof(g_score)/sizeof(g_score[0]);
//	for (int i = 0; i < len; i++){
//		g_score[i] = INVALID_SCORE;
//	}
//}
//
//TEST_GROUP(scoring) {
//	int player_id;
//	void setup () {
//		player_id = 0;
//		clear_user_score();
//	}
//};
//
//TEST(scoring, one_player_win_not_exist)
//{
//	player_wins(player_id);
//	LONGS_EQUAL(1, user_data_get_score(player_id));
//}
//
//TEST(scoring, one_player_win_if_exists)
//{
//	user_data_add(player_id);
//	player_wins(player_id);
//	LONGS_EQUAL(1, user_data_get_score(player_id));
//}
//
//TEST(scoring, one_player_win_if_exists_with_score)
//{
//	user_data_add(player_id);
//	user_data_set_score(player_id, 2);
//	player_wins(player_id);
//	LONGS_EQUAL(3, user_data_get_score(player_id));
//}
//
//TEST(scoring, two_player)
//{
//	int player2 = player_id + 1;
//	user_data_add(player_id);
//	user_data_set_score(player_id, 2);
//	user_data_add(player2);
//	user_data_set_score(player2, 4);
//	player_wins(player_id);
//	LONGS_EQUAL(3, user_data_get_score(player_id));
//}
//
//
//int user_data_find(int player_id){
//	return g_score[player_id] != INVALID_SCORE;
//}
//void user_data_add(int player_id){
//	g_score[player_id] = 0;
//}
//int user_data_get_score(int player_id){
//	return g_score[player_id];
//}
//void user_data_set_score(int player_id, int score){
//	g_score[player_id] = score;
//}


/*
 * Mock tests below
 */
/*
TEST_GROUP(scoring)
{
};

TEST(scoring, one_player_win_not_exist)
{
	int player_id = 1;
	mock().expectOneCall("user_data_find").withParameter("player_id", player_id).andReturnValue(
			0);
	mock().expectOneCall("user_data_add").withParameter("player_id", player_id);
	mock().expectOneCall("user_data_get_score").withParameter("player_id",
			player_id).andReturnValue(0);
	mock().expectOneCall("user_data_set_score").withParameter("player_id",
			player_id) .withParameter("score", 1);
	player_wins(player_id);
}

TEST(scoring, one_player_win_if_exists)
{
	int player_id = 2;
	mock().expectOneCall("user_data_find").withParameter("player_id", player_id).andReturnValue(
			1);
	mock().expectOneCall("user_data_get_score").withParameter("player_id",
			player_id).andReturnValue(2);
	mock().expectOneCall("user_data_set_score").withParameter("player_id",
			player_id) .withParameter("score", 3);
	player_wins(player_id);
}

int user_data_find(int player_id) {
	return mock().actualCall("user_data_find").withParameter("player_id",
			player_id).returnValue().getIntValue();
}

void user_data_add(int player_id) {
	mock().actualCall("user_data_add").withParameter("player_id", player_id);
}

int user_data_get_score(int player_id) {
	return mock().actualCall("user_data_get_score").withParameter("player_id",
			player_id).returnValue().getIntValue();
}

void user_data_set_score(int player_id, int score) {
	mock().actualCall("user_data_set_score").withParameter("player_id",
			player_id).withParameter("score", score);
}
*/
