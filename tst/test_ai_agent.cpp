#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
extern "C" {
#include "tiles.h"
#include "ai_agent.h"
}
#include "mocks.h"

TEST_GROUP(ai_agent)
{
	agent_t * agent;
	void setup() {
		agent = create_ai_agent();
		tile_t holdings[] = { C(1) };
		agent->deal(agent, holdings, 1, 0);
	}
	void teardown() {
		agent->destroy(agent);
	}

};

TEST(ai_agent, player_creation)
{
	LONGS_EQUAL(ACTION_RESTART, agent->get_action(agent, NULL));
}

TEST(ai_agent, player_pick_when_winning)
{
	agent->pick(agent, C(1), 0);
	LONGS_EQUAL(ACTION_WIN, agent->get_action(agent, NULL));
}

TEST(ai_agent, player_react_when_winning)
{
	agent->discard_tile(agent, C(1), 1);
	LONGS_EQUAL(ACTION_WIN, agent->get_action(agent, NULL));
}

TEST(ai_agent, player_pick)
{
	mock()	.expectNCalls(2, "evaluator_evaluate_array")
			.ignoreOtherParameters()
			.andReturnValue(1);
	agent->pick(agent, C(3), 0);
	LONGS_EQUAL(ACTION_DISCARD, agent->get_action(agent, NULL));
}

TEST(ai_agent, player_win)
{
	agent->win(agent, 0, 0);
	LONGS_EQUAL(ACTION_RESTART, agent->get_action(agent, NULL));
}
