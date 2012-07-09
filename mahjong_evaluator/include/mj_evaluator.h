/*
 * mj_evaluator.h
 *
 *  Created on: Feb 10, 2011
 *      Author: terry
 */

#ifndef MJ_EVALUATOR_H_
#define MJ_EVALUATOR_H_

#include "tile.h"
typedef struct Evaluator * evaluator_ptr_t;

evaluator_ptr_t LIB_create_evaluator(void);
void LIB_evaluator_destroy(evaluator_ptr_t);
int LIB_evaluator_evaluate_array(evaluator_ptr_t self, tile_t tiles[], int array_size);

#endif /* MJ_EVALUATOR_H_ */
