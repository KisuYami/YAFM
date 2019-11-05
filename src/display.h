#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include "dir.h"

void
screen_setup();

void
display_path(dir_t *changing_dir);

void
display_files(dir_t *changing_dir);

int
display_confirm(WINDOW *screen, int phrase_argc, char *phrase_input, ...);

#endif /* DISPLAY_H */
