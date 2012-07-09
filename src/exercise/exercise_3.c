#include "exercise_3.h"


void player_wins(int player_id)
{
	if (!user_data_find(player_id)){
		user_data_add(player_id);
	}

	int score = user_data_get_score(player_id);
	score += 1;

	user_data_set_score(player_id, score);
}
