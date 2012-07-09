#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tile.h"
#include "evaluator.h"
#include "../mahjong_evaluator/include/mj_evaluator.h"


typedef struct evaluator_imp_t {
	evaluator_t parent;
	evaluator_ptr_t evalutor;
} evaluator_imp_t;

static void _evaluator_destroy(evaluator_t * self) {
	evaluator_imp_t * _self = (evaluator_imp_t*) self;
	LIB_evaluator_destroy(_self->evalutor);
	free(self);
}
static int _evaluate_array(struct evaluator_t * self, tile_t tiles[], int array_size){
	evaluator_imp_t * _self = (evaluator_imp_t*) self;
	return LIB_evaluator_evaluate_array(_self->evalutor, tiles, array_size);
}

static evaluator_t * create_evaluator_r_impl() {
	evaluator_t * evaluator = (evaluator_t *) malloc(sizeof(evaluator_imp_t));

	evaluator->destroy = _evaluator_destroy;
	evaluator->evaluate_array = _evaluate_array;

	evaluator_imp_t * self = (evaluator_imp_t *) evaluator;
	self->evalutor = LIB_create_evaluator();

	return evaluator;
}
evaluator_t * (*create_evaluator_r)(void) = create_evaluator_r_impl;
