
extern "C" {
#include "mj_evaluator.h"
#include "terry_evaluator.h"
}

class Evaluator {
public:
	Evaluator() {}
	~Evaluator() {}
};


evaluator_ptr_t LIB_create_evaluator(void)
{
	return new Evaluator;
}
void LIB_evaluator_destroy(evaluator_ptr_t e)
{
	delete e;
}

int LIB_evaluator_evaluate_array(evaluator_ptr_t self, tile_t tiles[], int array_size)
{
	return terry_ai_evaluate_array(tiles, array_size);
}
