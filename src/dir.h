#ifndef DIR_HEADER
#define DIR_HEADER

#define PATH_MAX 4096 + 1 /* chars in a path name including nul(linux) */
#define NAME_MAX 256 + 1  /* chars in a path name including nul(linux) */

#include "../config.h"
#include <ncurses.h>

struct paste_clipboard {
  int type;
  char path[PATH_MAX];
};

struct working_dir {
  int num_files;        // A count for the files in the working folder
  char *file[NAME_MAX]; // This should be used with malloc
  char path[PATH_MAX];
  struct paste_clipboard working_clipboard[CLIPBOARD_MAX];
};

int file_list(struct working_dir *changing_dir);
int file_delete(struct working_dir *changing_dir, WINDOW *screen, int cursor);
void file_paste(struct working_dir *changing_dir);
void file_kill_yank(struct working_dir *changing_dir, int type, int cursor,
                    int clipboard_number);
void file_open(struct working_dir *changing_dir, int cursor);

int cd_enter(struct working_dir *changing_dir, int cursor);
void cd_back(struct working_dir *changing_dir);

int is_file(char *path);

#endif
