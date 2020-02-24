#include <ncurses.h>
#include <stdarg.h>
#include <string.h>

#include "dir.h"
#include "display.h"

void
screen_setup(void)
{
	initscr();
	cbreak();
	noecho();
	start_color();

	// Colors
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);

	getmaxyx(stdscr, config.size.y, config.size.x);
}

void
display_path(void)
{
	attron(A_UNDERLINE);
	mvwprintw(stdscr, config.size.y-1, 0, config.path);
	attroff(A_UNDERLINE);
	refresh();
}

void
display_files(struct dir_display dir_display, int factor)
{
	wclear(dir_display.screen);
	for(int i = factor; i < dir_display.files.size; i++)
	{
		if((i - factor) >= dir_display.position.y[1]) // only draw in the space of the window
			break;

		if(dir_display.files.marked[i])
			mvwprintw(dir_display.screen, i - factor,
				  DISPLAY_MARGIN_STATUS, "*",
				  dir_display.files.list[i]);

		int file_attr = 0;

		if(!is_file(dir_display.files.list[i]))
			file_attr = COLOR_PAIR(2);

		wattron(dir_display.screen, file_attr);
		mvwprintw(dir_display.screen, i - factor, DISPLAY_MARGIN_LIST,
			  "%s", dir_display.files.list[i]);
		wattroff(dir_display.screen, file_attr);
	}
	wrefresh(dir_display.screen);
}

void
main_display_files(struct dir_display dir_display, int cursor)
{
	int factor;
	static int old_factor;

	factor = (cursor > dir_display.position.y[1] - 2) ?
		cursor - dir_display.position.y[1] + 2 : 0;

	if(cursor == 0)
		old_factor = 0;

	if(old_factor > factor && cursor > old_factor)
		factor = old_factor;

	// Scroolling up
	else if(old_factor > factor)
	{
		factor = old_factor;
		factor--;
	}

	old_factor = factor;

	display_files(dir_display, factor);
}

void
preview_display_files(struct dir_display *main_display,
		      struct dir_display *preview_display,
		      int cursor)
{
	if(preview_list_files(main_display, preview_display, cursor) != 0)
		return;

	display_files(*preview_display, 0);
}
