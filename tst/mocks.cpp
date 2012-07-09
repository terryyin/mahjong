#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
#include "tile_pool.h"
#include "player.h"
#include "agent.h"
}
#include "stdio.h"
#include "mocks.h"

extern "C" {
#include "../mahjong_evaluator/include/mj_evaluator.h"
}
/*
 * This a bad example of mocking 3rd-party code.
 * LIB_xxx are 3rd-party functions.
 * In our test, we write a stub for it, and make it as a mock.
 * The better solution is to write an adaptor for LIB_xxx functions
 * and then mock the adaptor.
 */
int LIB_evaluator_evaluate_array(evaluator_ptr_t self, tile_t tiles[], int array_size)
{
	return mock().actualCall("evaluator_evaluate_array")
			.withParameter("tiles", tiles)
			.withParameter("array_size", array_size)
			.returnValue().getIntValue();

}
evaluator_ptr_t LIB_create_evaluator(void)
{
	return NULL;
}
void LIB_evaluator_destroy(evaluator_ptr_t)
{
}


static void tile_pool_shuffle_mock(tile_pool_t * game) {
	mock().actualCall("tile_pool_shuffle");
}

static int tile_pool_is_end_mock(tile_pool_t * game) {
	return mock().actualCall("tile_pool_is_end") .onObject((game)) .returnValue().getIntValue();
}

static tile_t tile_pool_pop_a_tile_mock(tile_pool_t * self) {
	return (tile_t) mock().actualCall("tile_pool_pop_a_tile") .onObject(self) .returnValue().getIntValue();
}

/*
 * tile_pool is an ADT (Abstract DataType).
 * It doesn't support polymorphism (function pointers stored in data structure).
 * All the methods of tile_pool are global function pointers.
 * To mock an ADT object, we simply redirect the methods to our mock functions.
 */
void setup_tile_pool_mocks() {
	UT_PTR_SET(tile_pool_shuffle, tile_pool_shuffle_mock);
	UT_PTR_SET(tile_pool_is_end, tile_pool_is_end_mock);
	UT_PTR_SET(tile_pool_pop_a_tile, tile_pool_pop_a_tile_mock);
}

extern "C" {
#include "agent.h"
}
static void agent_destroy_mock(agent_t *self) {
	free(self);
}
static void agent_deal_mock(agent_t *self, tile_t tiles[],
		int buffer_size, int distance) {
	char s[100];
	mock().actualCall("agent.deal").onObject(self) .withParameter(
			"distance", distance) .withParameter("tiles", tiles_to_string(
			tiles, buffer_size, s, 100));
}
static action_t agent_get_action_mock(struct agent_t * self, tile_t* tile) {
	if (tile != NULL)
		*tile
				= mock().actualCall("agent.get_action.tile").onObject(self).returnValue().getIntValue();
	return (action_t) mock().actualCall("agent.get_action").onObject(self).returnValue().getIntValue();
}
static void agent_pick_mock(agent_t * self, tile_t tile, int distance) {
	mock().actualCall("agent.pick").onObject(self) .withParameter("tile", tile).withParameter(
			"distance", distance);
}
static void agent_win_mock(agent_t * self, int score, int distance) {
	mock().actualCall("agent.win").onObject(self) .withParameter("score", score).withParameter(
			"distance", distance);
}
static void agent_throw_tile_mock(struct agent_t *self, tile_t tile,
		int distance) {
	mock().actualCall("agent.throw_tile").onObject(self) .withParameter("tile",
			tile).withParameter("distance", distance);
}

/*
 * agent, is more OO-like class than ADT (see the above mock)
 * It stores its method (function pointers) in it's object's data structure.
 * So we can create a mock object with the same interface as other agent object
 * but with the mock implementation.
 */
agent_t * create_mock_agent(void) {
	agent_t * agent = (agent_t *) malloc(sizeof(agent_t));
	agent->destroy = agent_destroy_mock;
	agent->deal = agent_deal_mock;
	agent->get_action = agent_get_action_mock;
	agent->pick = agent_pick_mock;
	agent->discard_tile = agent_throw_tile_mock;
	agent->win = agent_win_mock;
	return agent;
}
