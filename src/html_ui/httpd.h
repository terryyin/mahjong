#ifndef HTTPD__H
#define HTTPD__H

int server_main(void);
void set_shutdown_flag(void);

typedef int (*game_callback)(const char *path, const char *query_string, char * buffer, int buffer_size);
extern game_callback game_callback_ptr;

#endif
