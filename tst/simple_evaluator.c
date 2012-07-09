/*
 * this is an example of Simulator.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tile.h"
#include "evaluator.h"
#include "../mahjong_evaluator/include/mj_evaluator.h"

static tile_t cheapest_tile = NO_TILE;
void set_cheapest_tile(tile_t tile) {
	cheapest_tile = tile;
}

typedef struct evaluator_imp_t {
	evaluator_t parent;
} evaluator_imp_t;
static void _evaluator_destroy(evaluator_t * self);
static int _evaluate_array(struct evaluator_t * self, tile_t tiles[], int array_size);

evaluator_t * create_simple_evaluator_r() {
	evaluator_t * evaluator = (evaluator_t *) malloc(sizeof(evaluator_imp_t));

	evaluator->destroy = _evaluator_destroy;
	evaluator->evaluate_array = _evaluate_array;

	return evaluator;
}

static void _evaluator_destroy(evaluator_t * self) {
	free(self);
}
static int _evaluate_array(struct evaluator_t * self, tile_t tiles[], int array_size){
	int i;
	for(i=0; i < array_size; i++) {
		if (tiles[i]==cheapest_tile)
			return 1;
	}
	return 2;
}
