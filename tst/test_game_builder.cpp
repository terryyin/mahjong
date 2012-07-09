#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "game_builder.h"
#include "agent.h"
}

TEST_GROUP(system1) {

};

/*
 * This is a bad example
 * of a test case, which covers many line of code,
 * but actually checks nothing.
 */
TEST(system1, create_system) {
	agent_t * ui = game_builder_join_new_game_with_one_ai_player(NULL);
	ui->destroy(ui);
}

