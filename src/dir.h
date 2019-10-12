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

    struct clippboard
    {
        size_t mem[2]; // 0 = Total Size(unit), 1 = Used Mem
        char **clip;
    } clippboard;

    struct config
    {
        int y;
        int x;
        int hidden_files;
        char *env[10];
    } config;

};

int file_list(struct working_dir *changing_dir);
void file_open(struct working_dir *changing_dir);

int cd_enter(struct working_dir *changing_dir);
void cd_back(struct working_dir *changing_dir);

int is_file(char *path);

#endif
