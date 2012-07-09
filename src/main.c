#include <stdlib.h>
#include <time.h>
#include "httpd.h"
#include "html_game.h"

int main(void)
{
	srand(time(NULL));
	game_callback_ptr = execute_game_command;
	shutdown_server = set_shutdown_flag;
	return server_main();
}
