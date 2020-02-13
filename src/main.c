#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "dir.h"
#include "display.h"
#include "clipboard.h"
#include "../config.h"

int
main(void)
{
	screen_setup();

	config.envp[0] = getenv("IMAGE");
	config.envp[1] = getenv("VIDEO");
	config.envp[2] = getenv("DOCUMENTS");

	struct dir_display main_display, preview_display;

	list_files(&main_display, NULL);
	main_display.position = (struct position) {
		.y[0] = 0,
		.y[1] = config.size.y - 1,
		.x[0] = 0,
		.x[1] = config.size.x/2,
	};

	preview_display.position = (struct position) {
		.y[0] = 0,
		.y[1] = config.size.y - 1,
		.x[0] = config.size.x/2,
		.x[1] = config.size.x/2,
	};

	main_display.screen = newwin(main_display.position.y[1],
				     main_display.position.x[1],
				     main_display.position.y[0],
				     main_display.position.x[0]);

	preview_display.screen = newwin(preview_display.position.y[1],
					preview_display.position.x[1],
					preview_display.position.y[0],
					preview_display.position.x[0]);

	clear();
	attron(A_UNDERLINE);
	mvwprintw(stdscr, config.size.y-1, 0, config.path);
	attroff(A_UNDERLINE);
	refresh();

	preview_display_files(&main_display, &preview_display, 0);

	wclear(main_display.screen);
	display_files(main_display, 0);
	wrefresh(main_display.screen);

	int cursor = 0;

	for(char key = getch(); key != KEY_QUIT; key = getch())
	{
		int redraw_flag = 0;
		switch(key)
		{
		case KEY_MOV_UP:
			if(cursor > 0) cursor--;
			break;

		case KEY_MOV_DOWN:
			// cursor+1: because i need the info about where
			// the cursor will be, not where it is.
			if(cursor+1 < main_display.files.size) cursor++;
			break;

		case KEY_MOV_LEFT:
		{
			char *tmp = strrchr(config.path, '/');

			if(tmp != NULL)
			{
				tmp[0] = '\0';

				chdir(config.path);
				list_files(&main_display, NULL);

				cursor = 0;
				redraw_flag = 1;
			}
			break;
		}
		case KEY_MOV_RIGHT:
			if(chdir(main_display.files.list[cursor]) == 0)
			{
				list_files(&main_display, NULL);
				cursor = 0;
				redraw_flag = 1;
			}
			break;

		case KEY_FILE_OPEN:
			file_open(&main_display, cursor);
			break;

		case KEY_FILE_MARK:
			main_display.files.marked[cursor] =
				!main_display.files.marked[cursor];

			redraw_flag = 1;
			break;

		case KEY_FILE_HIDDEN:
			config.hidden = !config.hidden;
			list_files(&main_display, NULL);
			break;

		case KEY_FILE_DEL:
			if(selection_del(&main_display) == 0)
			{
				list_files(&main_display, NULL);
				cursor = 0;
			}

			redraw_flag = 1;
			break;

		case KEY_FILE_YANK:
			selection_yank(&main_display);
			break;

		case KEY_FILE_CUT:
			selection_cut(&main_display);
			break;

		case KEY_FILE_PASTE:
			selection_paste();
			break;
		}

		// If terminal size has changed, update the window size
		if(is_term_resized(config.size.y, config.size.x) == true)
		{

			getmaxyx(stdscr, config.size.y, config.size.x);

			main_display.position = (struct position) {
				.y[0] = 0,
				.y[1] = config.size.y - 1,
				.x[0] = 0,
				.x[1] = config.size.x/2,
			};

			preview_display.position = (struct position) {
				.y[0] = 0,
				.y[1] = config.size.y - 1,
				.x[0] = config.size.x/2,
				.x[1] = config.size.x/2,
			};

			delwin(main_display.screen);
			delwin(preview_display.screen);

			main_display.screen = newwin(main_display.position.y[1],
						     main_display.position.x[1],
						     main_display.position.y[0],
						     main_display.position.x[0]);

			preview_display.screen = newwin(preview_display.position.y[1],
							preview_display.position.x[1],
							preview_display.position.y[0],
							preview_display.position.x[0]);

			redraw_flag = 1;
		}

		if(redraw_flag != 0)
		{
			clear();
			attron(A_UNDERLINE);
			mvwprintw(stdscr, config.size.y-1, 0, config.path);
			attroff(A_UNDERLINE);
			refresh();
		}

		wclear(main_display.screen);
		display_files(main_display, cursor);
		wrefresh(main_display.screen);

		preview_display_files(&main_display, &preview_display, cursor);
	}

	endwin();
	delwin(main_display.screen);
	delwin(preview_display.screen);
	return 0;
}
