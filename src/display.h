#ifndef DISPLAY_H
#define DISPLAY_H

#include "dir.h"

enum DISPLAY_MARGIN {
	DISPLAY_MARGIN_STATUS = 2,
	DISPLAY_MARGIN_LIST = 4,
};

void
screen_setup(void);

void
display_files(struct dir_display dir_display, int factor);

void
main_display_files(struct dir_display dir_display, int cursor);

void
preview_display_files(struct dir_display *main_display,
		      struct dir_display *preview_display,
		      int cursor);

void
display_path(void);
#endif /* DISPLAY_H */
