#ifndef TERRY_EVALUATOR_H_
#define TERRY_EVALUATOR_H_

#define MAX_PATTERN_ITEMS 10
typedef int (*pattern_score_callback_t)(tile_t tile);

typedef enum {
	PATTERN_VERB_IS,
	PATTERN_VERB_NUMBER_IS,
	PATTERN_VERB_ANY,
	PATTERN_VERB_19,
	PATTERN_VERB_XXX
} pattern_verb;

typedef struct {
	int base_score;
	int negative_score_of_1_9;
	pattern_verb verb;
	tile_t tile;
	int following_items[MAX_PATTERN_ITEMS];
	int pattern_item_count;
}tiles_pattern_t;

#define MAX_HASH_TILE_COUNT 14
typedef int tile_hash_value_t;
typedef struct {
	tile_hash_value_t value;
	tile_t tiles[MAX_HASH_TILE_COUNT];
	int n;
} tile_hash_item_t;
#define TILE_HASH_NO_VALUE (0)
#define TILE_HASH_SIZE 66005
typedef struct {
	tile_hash_item_t items[TILE_HASH_SIZE];

} tile_hash_t;

int terry_ai_evaluate_array(tile_t tiles[], int n);
int terry_ai_evaluate_array_by_patterns(tile_t tiles[], int n, tiles_pattern_t patterns[], int pattern_count);

void tile_hash_init(tile_hash_t * hash);
tile_hash_value_t tile_hash_get(tile_hash_t * hash, tile_t tiles[], int n);
tile_hash_value_t tile_hash_set(tile_hash_t * hash, tile_t tiles[], int n, tile_hash_value_t value);

#define PATTERN(name, base, count, v) base_score:base, negative_score_of_1_9:0.0, pattern_item_count:count, verb:v, following_items:
#define PATTERNN(name, base, n19, count, v) base_score:base, negative_score_of_1_9:n19, pattern_item_count:count, verb:v, following_items:

#endif /* TERRY_EVALUATOR_H_ */
