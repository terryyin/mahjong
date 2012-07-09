
#ifndef EXERCISE_3_H_
#define EXERCISE_3_H_

extern int user_data_find(int player_id);
extern void user_data_add(int player_id);
extern int user_data_get_score(int player_id);
extern void user_data_set_score(int player_id, int score);

void player_wins(int player_id);

#endif /* EXERCISE_3_H_ */
