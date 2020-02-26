#ifndef DISPLAY_H
#define DISPLAY_H

#include "dir.h"

void
screen_setup(void);

void
display_files(struct dir_display dir_display, int cursor);

void
preview_display_files(struct dir_display *main_display,
		      struct dir_display *preview_display,
		      int cursor);
#endif /* DISPLAY_H */
