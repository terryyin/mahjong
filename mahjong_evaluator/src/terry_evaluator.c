
#include "tiles.h"
#include "terry_evaluator.h"
#include "memory.h"

#define PN   4
#define P1   (2*PN+1)
#define P11  (3*P1+2)
#define P12  (P11)
#define P13  (P12-4)
#define PNN  (P11-7)
#define P133 (4*P1+7)
#define P134 (P133-6)
#define P135 (P133 -6)
#define P112 (P133+1)
#define P123 54
#define P111 (P123+7)
#define PNNN (41)
tiles_pattern_t g_patterns[] = {
		{PATTERNN(any, 		P1, 	5.0, 1,PATTERN_VERB_ANY){}},
		{PATTERN(xxx, 		PN, 	     1,PATTERN_VERB_XXX){}},
		{PATTERNN(22, 		P11, 	0.0, 2,PATTERN_VERB_ANY){0}},
		{PATTERN(nn, 		PNN,        2,PATTERN_VERB_XXX){0}},
		{PATTERNN(56, 		P12, 	7.0, 2,PATTERN_VERB_ANY){1}},
		{PATTERNN(13, 		P13,	3.0, 2,PATTERN_VERB_ANY){2}},

		{PATTERNN(135, 		P135,	1.0, 3,PATTERN_VERB_ANY){2,4}},
		{PATTERNN(134, 		P134,	1.0, 3,PATTERN_VERB_ANY){2,3}},
		{PATTERNN(124, 		P134,	1.0, 3,PATTERN_VERB_ANY){1,3}},
		{PATTERNN(123, 		P123,	1.0, 3,PATTERN_VERB_ANY){1,2}},
		{PATTERNN(111, 		P111,	1.0, 3,PATTERN_VERB_ANY){0,0}},
		{PATTERNN(NNN, 		PNNN,	0.0, 3,PATTERN_VERB_XXX){0,0}},
		{PATTERNN(112, 		P112,	2.0, 3,PATTERN_VERB_ANY){0,1}},
		{PATTERNN(122, 		P112,	2.0, 3,PATTERN_VERB_ANY){1,1}},
		{PATTERNN(113, 		P133,	2.0, 3,PATTERN_VERB_ANY){0,2}},
		{PATTERNN(133, 		P133,	2.0, 3,PATTERN_VERB_ANY){2,2}},

		{PATTERNN(1234, 	66,	2.0, 4,PATTERN_VERB_ANY){1,2,3}},
		{PATTERNN(1123, 	64,	2.0, 4,PATTERN_VERB_ANY){0,1,2}},
		{PATTERNN(1233, 	64,	2.0, 4,PATTERN_VERB_ANY){1,2,2}},
		{PATTERNN(1112, 	73,	2.0, 4,PATTERN_VERB_ANY){0,0,1}},
		{PATTERNN(1222, 	73,	2.0, 4,PATTERN_VERB_ANY){1,1,1}},
//		{PATTERNN(1357, 	41.0,	1.0, 4,PATTERN_VERB_ANY){2,4,6}},
//		{PATTERNN(2457, 	54.0,	1.0, 4,PATTERN_VERB_ANY){2,3,5}},
//		{PATTERNN(3446, 	60.0,	1.0, 4,PATTERN_VERB_ANY){1,1,3}},
//		{PATTERNN(3556, 	60.0,	1.0, 4,PATTERN_VERB_ANY){2,2,3}},
//		{PATTERNN(2346, 	50.0,	2.0, 4,PATTERN_VERB_ANY){1,2,4}},
//		{PATTERNN(2456, 	50.0,	2.0, 4,PATTERN_VERB_ANY){2,3,4}}

};

#define tile_get_number(tile) \
	 ((tile < MJ_CIRCLE_BASE) ?	(tile - MJ_CHARACTOR_BASE) :\
		(tile < MJ_BAMBOO_BASE) ? tile - MJ_CIRCLE_BASE: \
				(tile < MJ_EAST) ?	tile - MJ_BAMBOO_BASE : 0)

#define match_verb_and_tile(verb, base) \
	(((verb == PATTERN_VERB_ANY) && tile_get_number(base)) || ((verb == PATTERN_VERB_XXX) && (!tile_get_number(base))))

static int pattern_match_from_beginning(tiles_pattern_t * pattern, tile_t tiles[], int n)
{
	if (pattern->pattern_item_count <= n) {
		tile_t base = tiles[0];

		if (match_verb_and_tile(pattern->verb, base)) {
			int i;
			for (i = 1; i < pattern->pattern_item_count; i++) {
				int tile_index = tiles_get_index_of_tile(++tiles, --n, base + pattern->following_items[i - 1]);
				if (tile_index == -1)
					return 0;
				tiles+= tile_index;
				n -= tile_index;
			}
			return 1;
		}
	}

	return 0;
}

int pattern_get_score(tiles_pattern_t * pattern, tile_t tile)
{
	int number = tile_get_number(tile);
	if (number == 1 || number == 9)
		return pattern->base_score - pattern->negative_score_of_1_9;
	if (pattern->pattern_item_count>1) {
		number = tile_get_number(tile + pattern->following_items[pattern->pattern_item_count-2]);
		if (number == 9)
			return pattern->base_score - pattern->negative_score_of_1_9;
	}
	return pattern->base_score;
}

static int pattern_extract_from_beginning(tiles_pattern_t * pattern, tile_t tiles[], int n, tile_t new_tiles[])
{
	tile_t base = tiles[0];
	int i = 1,pindex=1, new_i=0;

	for(;i<n; i++) {
		if ((pindex <pattern->pattern_item_count) && (base + pattern->following_items[pindex-1]==tiles[i])) {
			pindex++;
		}
		else {
			new_tiles[new_i++] = tiles[i];
		}
	}
	for (; new_i < n; new_i++) {
		new_tiles[new_i] = NO_TILE;
	}

	return pattern_get_score(pattern, tiles[0]);
}

int pattern_get_tile_count(tiles_pattern_t * pattern){
	return pattern->pattern_item_count;
}

tile_hash_t g_hash;
static int evaluate_sub_array_with_hash_table(tile_t tiles[], int n, tiles_pattern_t patterns[], int pattern_count);

static int evaluate_sub_array(tile_t tiles[], int n, tiles_pattern_t patterns[], int pattern_count)
{
    int max = 0, i;
    tile_t new_tiles[n];

    for(i = 0; i < pattern_count; i++){
    	tiles_pattern_t *pattern = &patterns[i];
        if(pattern_match_from_beginning(pattern, tiles, n)){
        	int score = pattern_extract_from_beginning(pattern, tiles, n, new_tiles);
            score += evaluate_sub_array_with_hash_table(new_tiles, n - pattern_get_tile_count(pattern), patterns, pattern_count);

            if(score > max) max = score;
        }
    }

    return max;
}

int evaluate_sub_array_with_hash_table(tile_t tiles[], int n, tiles_pattern_t patterns[], int pattern_count)
{
	if (n == 0) {
		return 0;
	}

	int hashed_value = tile_hash_get(&g_hash, tiles, n);
	if (hashed_value != TILE_HASH_NO_VALUE)
		return hashed_value;

    return tile_hash_set(&g_hash, tiles, n,
    		evaluate_sub_array(tiles, n, patterns, pattern_count));
}

int terry_ai_evaluate_array_by_patterns(tile_t tiles[], int n, tiles_pattern_t patterns[], int pattern_count)
{
	if (n == 0) {
		return 0;
	}
	int score = 0;

	while(1) {
		tile_t base = tiles[0];
		int i;
		for(i=0;i< n && tiles[i]-base < 10; i++);
		score += evaluate_sub_array_with_hash_table(tiles, i, patterns, pattern_count);
		if (i==n)
			break;
		n-=i, tiles+=i;
	};

   return score;
}

int terry_ai_evaluate_array(tile_t tiles[], int n)
{
	tiles_sort(tiles, n);
	n = tiles_get_count(tiles, n);
	return (int) terry_ai_evaluate_array_by_patterns(tiles, n, g_patterns,
			sizeof(g_patterns) / sizeof(g_patterns[0]));
}

int (*evaluate_array)(tile_t tiles[], int n) = terry_ai_evaluate_array;

void tile_hash_init(tile_hash_t * hash)
{
	memset(hash->items, 0, sizeof(hash->items));
}
static tile_hash_item_t *_hash_get_item( tile_hash_t *hash, tile_t tiles[], int n)
{
    long long hash_index = 0;
    while(n) hash_index = (hash_index << 4) + (tiles[--n]);

    tile_hash_item_t * item = &hash->items[hash_index % TILE_HASH_SIZE];
    return item;
}

static void _hash_check_and_set_key(tile_hash_item_t *item, tile_t tiles[], int n)
{
	int i;
    for(i = 0;i < n;i++){
        if(item->tiles[i] != tiles[i])
            break;
    }
    if ((i != n )|| (n != item->n)) {
		item->value = TILE_HASH_NO_VALUE;
		item->n = n;
	}
    for(;i < n;i++)
        item->tiles[i] = tiles[i];
}
static tile_hash_value_t * _hash_get_ref(tile_hash_t * hash, tile_t tiles[], int n)
{
    tile_hash_item_t *item = _hash_get_item(hash, tiles, n);
    _hash_check_and_set_key(item, tiles, n);

	return &item->value;
}

tile_hash_value_t tile_hash_get(tile_hash_t * hash, tile_t tiles[], int n)
{
	return *_hash_get_ref(hash, tiles, n);
}
tile_hash_value_t tile_hash_set(tile_hash_t * hash, tile_t tiles[], int n, tile_hash_value_t value)
{
	return *_hash_get_ref(hash, tiles, n) = value;
}

//some quick fix
#define USER_DATA_FUN(name) user_data_ ## name
int USER_DATA_FUN(find)(int player_id){
	return 0;
}
void USER_DATA_FUN(add)(int player_id){
}
int USER_DATA_FUN(get_score)(int player_id){
	return 0;
}
void USER_DATA_FUN(set_score)(int player_id, int score){
}
