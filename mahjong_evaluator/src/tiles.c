#include <stdlib.h>
#include <memory.h>
#include "tiles.h"

static int tiles_is_winning(tile_t tile_array[], int size_of_array, int had_pair);
static int tiles_is_winning_remove_a_pair(tile_t tile_array[], int size_of_array)
{
	if (tile_array[0] == tile_array[1]) {
		if (tiles_is_winning(tile_array + 2, size_of_array - 2, 1))
			return 1;
	}
	return 0;
}
static int tiles_is_winning_remove_a_pong(tile_t tile_array[], int size_of_array, int had_pair)
{
	if (size_of_array > 2) {
		if ((tile_array[0] == tile_array[1]) && (tile_array[1] == tile_array[2]))
			if (tiles_is_winning(tile_array + 3, size_of_array - 3,
					had_pair))
				return 1;
	}
	return 0;
}
static int tiles_is_winning_remove_a_chow(tile_t tile_array[], int size_of_array, int had_pair)
{
	if (size_of_array > 2) {
		tile_t new_tile_array[size_of_array];
		int i, cnt = 0;
		tile_t first_tile = tile_array[0];
		for (i = 0; i < size_of_array; i++) {
			if (cnt < 3 && tile_array[i] == first_tile + cnt) {
				cnt++;
			} else {
				new_tile_array[i - cnt] = tile_array[i];
			}
		}

		if (cnt == 3)
			if (tiles_is_winning(new_tile_array, size_of_array - 3,
					had_pair))
				return 1;
	}
	return 0;
}

static int tiles_is_winning_more_than_one_tile(tile_t tile_array[], int size_of_array, int had_pair)
{
	return ((!had_pair && tiles_is_winning_remove_a_pair(tile_array, size_of_array))
			|| tiles_is_winning_remove_a_pong(tile_array, size_of_array, had_pair)
			|| tiles_is_winning_remove_a_chow(tile_array, size_of_array, had_pair));
}

static int tiles_is_winning(tile_t tile_array[], int size_of_array, int had_pair) {
	if (size_of_array == 0)
		return 1;

	if (size_of_array == 1)
		return 0;

	return tiles_is_winning_more_than_one_tile(tile_array, size_of_array, had_pair);
}

int tiles_get_index_of_tile(tile_t tiles[], int n, tile_t tile)
{
	int i;
	for(i = 0; i<n && tiles[i]<tile; i++);
	if (i<n &&tiles[i] == tile)
			return i;
	return -1;
}

int tiles_get_count(tile_t tiles[], int size_of_array)
{
	int i, cnt=0;
	for (i=0; i<size_of_array; i++)
		if (tiles[i] != NO_TILE)
			cnt++;
	return cnt;
}
static int tile_compare(const void * t1, const void * t2) {return (*((tile_t *)t1)) - (*((tile_t *)t2));}

void tiles_sort(tile_t tiles[], int n)
{
    int i, cnt = 0;
    for(i = 0;i < n;i++){
        if(tiles[i] != NO_TILE)
            tiles[cnt++] = tiles[i];
    }
    for(i = cnt;i < n;i++){
            tiles[i] = NO_TILE;
    }
	qsort(tiles, cnt, sizeof(tile_t), tile_compare);

}
int tiles_is_able_to_pong(tile_t tiles[], int n, tile_t tile)
{
	int i, cnt = 0;
    for(i=0; i<n; i++) {
		if (tiles[i] == tile) {
			cnt++;
		}
	}
    return cnt>=2;
}

int tiles_plus_one_is_winning(tile_t holding[], int n, tile_t discard)
{
	tile_t tiles[MAX_HOLDING_COUNT+1];

	if (n > MAX_HOLDING_COUNT) return 0;

	memset(tiles, 0, sizeof(tiles));
	memcpy(tiles, holding, n * sizeof(tile_t));
	tiles[MAX_HOLDING_COUNT] = discard;
	tiles_sort(tiles, MAX_HOLDING_COUNT + 1);

	return tiles_is_winning(tiles, tiles_get_count(tiles, MAX_HOLDING_COUNT+1), 0);
}
