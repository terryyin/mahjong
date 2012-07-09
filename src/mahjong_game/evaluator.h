#ifndef EVALUATOR_H_
#define EVALUATOR_H_

typedef struct evaluator_t {
	int (*evaluate_array)(struct evaluator_t * self, tile_t tiles[], int array_size);
	void (*destroy)(struct evaluator_t * self);
}evaluator_t;

extern evaluator_t * (*create_evaluator_r)(void);

#endif /* EVALUATOR_H_ */
