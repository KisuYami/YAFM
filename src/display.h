#include <ncurses.h>
void screen_setup();
void display_path(struct working_dir *changing_dir, WINDOW *screen);
void display_files(struct working_dir *changing_dir, WINDOW *screen,
                   int cursor);
int display_confirm(WINDOW *screen, char *phrase, char *object);
