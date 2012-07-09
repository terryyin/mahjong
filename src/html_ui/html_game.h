/*
 * html_game.h
 *
 *  Created on: Feb 7, 2011
 *      Author: terry
 */

#ifndef HTML_GAME_H_
#define HTML_GAME_H_

int execute_game_command(const char * command, const char *parameters, char * buffer, int buffer_size);
void (*shutdown_server)(void);

#endif /* HTML_GAME_H_ */
