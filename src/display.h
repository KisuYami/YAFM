#ifndef DISPLAY_H
#define DISPLAY_H

#include "dir.h"

#define DRAW_PATH(void)						\
	clear();						\
	attron(A_UNDERLINE);					\
	mvwprintw(stdscr, config.size.y-1, 0, config.path);	\
	attroff(A_UNDERLINE);					\
	refresh();						\

enum DISPLAY_MARGIN {
	DISPLAY_M_PATH = 1,
	DISPLAY_M_CURS = 2,
	DISPLAY_M_MARK = 2,
	DISPLAY_M_LIST = 4,
};

void
screen_setup(void);

void
init_displays(display_t *main_display, display_t *preview_display);

void
display_files(display_t dir_display, int factor);

void
main_display_files(display_t dir_display, int cursor);

void
preview_display_files(display_t *main_display,
		      display_t *preview_display,
		      int cursor);
#endif /* DISPLAY_H */
