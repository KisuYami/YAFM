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
	curs_set(0);
	start_color();
	// Colors
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);

	getmaxyx(stdscr, config.size.y, config.size.x);
}

void
display_files(struct dir_display dir_display, int cursor)
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

	for(int i = factor; i < dir_display.files.size; i++)
	{
		if((i - factor) >= dir_display.position.y[1]) // only draw in the space of the window
			break;

		if(dir_display.files.marked[i])
			mvwprintw(dir_display.screen, i - factor, 2, "*",
				  dir_display.files.list[i]);
		if(i == cursor)
		{
			wattron(dir_display.screen, COLOR_PAIR(1) | A_BOLD | A_REVERSE);
			mvwprintw(dir_display.screen, i - factor, 4, "%s",
				  dir_display.files.list[i]);
			wattroff(dir_display.screen, COLOR_PAIR(1) | A_BOLD | A_REVERSE);
		}

		else if(!is_file(dir_display.files.list[i]))
		{
			wattron(dir_display.screen, COLOR_PAIR(2));
			mvwprintw(dir_display.screen, i - factor, 4, "%s",
				  dir_display.files.list[i]);
			wattroff(dir_display.screen, COLOR_PAIR(2));
		}
		else
			mvwprintw(dir_display.screen, i - factor, 4, "%s",
				  dir_display.files.list[i]);
	}
}

void
preview_display_files(struct dir_display *main_display,
		      struct dir_display *preview_display,
		      int cursor)
{
	if(preview_list_files(main_display, preview_display, cursor) != 0)
		return;

	wclear(preview_display->screen);
	for(int i = 0; i < preview_display->files.size; i++)
	{
		if(!is_file(preview_display->files.list[i]))
		{
			wattron(preview_display->screen, COLOR_PAIR(2));
			mvwprintw(preview_display->screen, i, 0, "%s",
				  preview_display->files.list[i]);
			wattroff(preview_display->screen, COLOR_PAIR(2));
		}
		else
			mvwprintw(preview_display->screen, i, 0, "%s",
				  preview_display->files.list[i]);
	}
	wrefresh(preview_display->screen);
}
