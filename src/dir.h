#ifndef DIR_HEADER
#define DIR_HEADER

#include <ncurses.h>

#define PATH_MAX 4096

struct working_dir
{
    int cursor;
    int num_files;
    char *file[100]; // XXX
    char *path;
    WINDOW *screen;

    struct config
    {
        int y;
        int x;
        int hidden_files;
        char *env[10];
    } config;

};

typedef struct working_dir dir_t;

int
is_file(char *path);

int
file_list(dir_t *changing_dir);

void
file_open(dir_t *changing_dir);

int
cd_enter(dir_t *changing_dir);

void
cd_back(dir_t *changing_dir);

int
file_delete(dir_t *changing_dir);
#endif
