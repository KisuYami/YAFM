#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "dir.h"
#include "display.h"


void
screen_setup(void)
{
	initscr();
	cbreak();
	noecho();

	// Colors
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	getmaxyx(stdscr, config.size.y, config.size.x);
}

void
init_displays(display_t * main_display, display_t * preview_display)
{
	*main_display = (display_t) {
		.screen = newwin(config.size.y - DISPLAY_M_PATH,
				 config.size.x / 2, 0, 0),
		.position = (struct position) {
			.y[0] = 0,
			.y[1] = config.size.y - DISPLAY_M_PATH,
			.x[0] = 0,
			.x[1] = config.size.x / 2,
		},
	};

	*preview_display = (display_t) {
		.screen = newwin(config.size.y - DISPLAY_M_PATH,
				 config.size.x / 2, 0,
				 config.size.x / 2),
		.position = (struct position) {
			.y[0] = 0,
			.y[1] = config.size.y - DISPLAY_M_PATH,
			.x[0] = config.size.x / 2,
			.x[1] = config.size.x / 2,
		},
	};

	if(!main_display->screen || !preview_display->screen)
	{
		if(main_display->screen)
			delwin(main_display->screen);

		endwin();

		perror("Failed to create windows");

		exit(-1);
	}

	if(list_files(main_display, NULL) == -1)
	{
		delwin(main_display->screen);
		delwin(preview_display->screen);
		endwin();

		fprintf(stderr, "kyfm: failed to execute init_displays(),"
			" exiting with -1 status.\n");

		exit(-1);
	}

	DRAW_PATH();

	preview_display_files(main_display, preview_display, 0);
	main_display_files(*main_display, 0);
}

// TODO: Fix wrong display coloring that is caused by not having a full
/// path to the file/dir while checking forit's type.

static void
display_files(display_t dir_display, int factor)
{
	wclear(dir_display.screen);
	for(int i = factor; i < dir_display.files.size; i++)
	{
		// only draw in the space of the window
		if((i - factor) >= dir_display.position.y[1])
			break;

		if(dir_display.files.marked[i])
			mvwprintw(dir_display.screen, i - factor,
				  DISPLAY_M_MARK, "*", dir_display.files.list[i]);

		int display_attr = 0;

		char buf[1024];
		sprintf(buf, "%s/%s/%s", config.path,
			dir_display.files.dir,
			dir_display.files.list[i]);

		if(!is_file(buf))
			display_attr = COLOR_PAIR(1);

		wattron(dir_display.screen, display_attr);

		mvwprintw(dir_display.screen, i - factor, DISPLAY_M_LIST, "%s",
			  dir_display.files.list[i]);

		wattroff(dir_display.screen, display_attr);
	}
	wrefresh(dir_display.screen);
}

void
main_display_files(display_t dir_display, int cursor)
{
	int		factor;
	static int	old_factor;

	/*********************************************************************/
	/* Ifthe curser has a greater value than the size of the screen */
	/* minus the space reserved to display the path, then the factor */
	/* shaw have a value that will make the list files "scrool down" */
	/* and keep the curser at it's position(DISPLAY_M_CURS).  */
	/*********************************************************************/
	factor = (cursor > (dir_display.position.y[1] - DISPLAY_M_CURS)) ?
		(cursor - dir_display.position.y[1] + DISPLAY_M_CURS) : 0;

	/*********************************************************************/
	/* This will reset the algorithm when changing directories.  */
	/*********************************************************************/
	if(cursor == 0)
		old_factor = 0;

	/*********************************************************************/
	/* Creates the effect of the cursor moving to the top of the */
	/* window.  */
	/*********************************************************************/
	if(old_factor > factor && cursor > old_factor)
		factor = old_factor;

	/*********************************************************************/
	/* Scroll up only after the cursor hits the top margin.	 */
	/*********************************************************************/
	else if(old_factor > factor)
		factor = old_factor - 1;

	old_factor = factor;

	display_files(dir_display, factor);
	move(cursor - factor, DISPLAY_M_LIST);
}

void
preview_display_files(display_t * main_display,
		      display_t * preview_display, int cursor)
{
	if(preview_list_files(main_display, preview_display, cursor) != 0)
		return;

	display_files(*preview_display, 0);
}
