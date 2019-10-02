#include <ncurses.h>
void screen_setup();
void display_path(struct working_dir *changing_dir);
void display_files(struct working_dir *changing_dir);
int display_confirm(WINDOW *screen, int phrase_argc, char *phrase_input, ...);
