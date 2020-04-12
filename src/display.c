#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#include "../config.h"
#include "dir.h"
#include "display.h"

void screen_setup(void)
{
	initscr();
	cbreak();
	noecho();

	// Colors
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	getmaxyx(stdscr, config.size.y, config.size.x);

	config.envp = (struct envp){
		// Needed to open files with externel programs...
		.defaults[0] = getenv("IMAGE"),
		.defaults[1] = getenv("VIDEO"),
		.defaults[2] = getenv("DOCUMENTS"),

		.home_name = getenv("HOME"),
		.user_name = getenv("USER"),
	};

	gethostname(config.envp.host_name, 1024);
}

void init_displays(display_t *main_display, display_t *preview_display)
{
	*main_display = (display_t){
		.screen = newwin(config.size.y - DISPLAY_M_PATH,
				 config.size.x / 2, 0, 0),
		.position = (struct position){
			.y[0] = 0,
			.y[1] = config.size.y - DISPLAY_M_PATH,
			.x[0] = 0,
			.x[1] = config.size.x / 2,
		},
	};

	// see display_files()
	strcpy(main_display->files.dir, "./");

	*preview_display = (display_t){
		.screen = newwin(config.size.y - DISPLAY_M_PATH,
				 config.size.x / 2, 0, config.size.x / 2),
		.position = (struct position){
			.y[0] = 0,
			.y[1] = config.size.y - DISPLAY_M_PATH,
			.x[0] = config.size.x / 2,
			.x[1] = config.size.x / 2,
		},
	};

	if (!main_display->screen || !preview_display->screen) {
		if (main_display->screen)
			delwin(main_display->screen);

		endwin();

		perror("Failed to create windows");
		exit(-1);
	}

	if (list_files(&main_display->files, NULL) == -1) {
		delwin(main_display->screen);
		delwin(preview_display->screen);
		endwin();

		perror("kyfm: failed to execute init_displays()");
		exit(-1);
	}

	draw_path();
	DRAW_FMA(0, main_display->files.size);

	display_p(main_display, preview_display, 0);
	display_m(*main_display, 0);
}

void draw_path(void)
{
	char path[1024];

	if (strcmp(config.path, "/home") == 1)
		sprintf(path, "~%s", config.path+strlen(config.envp.home_name));
	else
		strcpy(path, config.path);

	clear();
	attron(A_UNDERLINE);

	mvwprintw(stdscr, config.size.y - 1, 0, "%s@%s: %s",
		  config.envp.user_name, config.envp.host_name, path);

	attroff(A_UNDERLINE);
	refresh();
}

static void display_files(display_t dir_display, int factor)
{
	wclear(dir_display.screen);

	/*    We change in the dir to get the right values when calling is_file()
	 * the main display dir is aways set to "./"
	 */
	chdir(dir_display.files.dir);

	for (int i = factor; i < dir_display.files.size && (i-factor) <= dir_display.position.y[1]; i++) {
		if (dir_display.files.marked[i])
			mvwprintw(dir_display.screen, i - factor,
				  DISPLAY_M_MARK, "*");

		if (!is_file(dir_display.files.list[i])) {
			wattron(dir_display.screen, COLOR_PAIR(1));

			mvwprintw(dir_display.screen, i - factor, DISPLAY_M_LIST,
				  dir_display.files.list[i]);

			wattroff(dir_display.screen, COLOR_PAIR(1));
		}
		else
			mvwprintw(dir_display.screen, i - factor, DISPLAY_M_LIST,
				  dir_display.files.list[i]);
	}

	chdir(config.path);
	wrefresh(dir_display.screen);
}

void display_m(display_t dir_display, int cursor)
{
	int factor;
	static int old_factor;

	/*********************************************************************/
	/* Ifthe curser has a greater value than the size of the screen	     */
	/* minus the space reserved to display the path, then the factor     */
	/* shaw have a value that will make the list files "scrool down"     */
	/* and keep the curser at it's position(DISPLAY_M_CURS).	     */
	/*********************************************************************/
	factor = (cursor > (dir_display.position.y[1] - DISPLAY_M_CURS))
		? (cursor - dir_display.position.y[1] + DISPLAY_M_CURS)
		: 0;

	/* This will reset the algorithm when changing directories.  */
	if (cursor == 0)
		old_factor = 0;

	/* Creates the effect of the cursor moving to the top of the window. */
	if (old_factor > factor && cursor > old_factor)
		factor = old_factor;

	/* Scroll up only after the cursor hits the top margin.	 */
	else if (old_factor > factor)
		factor = old_factor - 1;

	old_factor = factor;

	display_files(dir_display, factor);
	move(cursor - factor, DISPLAY_M_LIST);
}

void display_p(display_t *main_display, display_t *preview_display,
	       int cursor)
{
	char tmp[PATH_MAX+MAXNAMLEN+1];

	strcpy(preview_display->files.dir, main_display->files.list[cursor]);

	sprintf(tmp, "%s/%s", config.path, preview_display->files.dir);

	if (list_files(&preview_display->files, tmp) != 0) {
		wclear(preview_display->screen);
		wrefresh(preview_display->screen);

		return;
	}

	display_files(*preview_display, 0);
}
