#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "dir.h"
#include "display.h"
#include "clipboard.h"
#include "../config.h"

/* MATH
   config.size.x / 2 is used to get half of
the screen size
 */

int
main(void)
{
	screen_setup();

	config.envp[0] = getenv("IMAGE");
	config.envp[1] = getenv("VIDEO");
	config.envp[2] = getenv("DOCUMENTS");

	display_t main_display, preview_display;

	init_displays(&main_display, &preview_display);
	list_files(&main_display, NULL);

	DRAW_PATH();

	preview_display_files(&main_display, &preview_display, 0);
	main_display_files(main_display, 0);

	int cursor = 0;
	int main_redraw = 0;

	move(cursor, DISPLAY_M_LIST);

	for(char key = getch(); key != KEY_QUIT; key = getch())
	{
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

				if(chdir(config.path) != 0) // XXX, so we can cd '/'
					chdir("/");

				cursor = 0;

				list_files(&main_display, NULL);

				DRAW_PATH();
				main_display_files(main_display, cursor);
			}
			break;
		}
		case KEY_MOV_RIGHT:
			if(chdir(main_display.files.list[cursor]) == 0)
			{
				cursor = 0;

				list_files(&main_display, NULL);

				DRAW_PATH();
				main_display_files(main_display, cursor);
			}

			break;

		case KEY_FILE_OPEN:
			file_open(&main_display, cursor);
			break;

		case KEY_FILE_MARK:
			main_display.files.marked[cursor] =
				!main_display.files.marked[cursor];

			DRAW_PATH();
			main_display_files(main_display, cursor);
			refresh();
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

			DRAW_PATH();
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

		preview_display_files(&main_display, &preview_display, cursor);

		// If terminal size has changed, update the window size
		if(is_term_resized(config.size.y, config.size.x) == 1)
		{
			getmaxyx(stdscr, config.size.y, config.size.x);

			delwin(main_display.screen);
			delwin(preview_display.screen);

			init_displays(&main_display, &preview_display);

			DRAW_PATH();
			main_display_files(main_display, cursor);
		}

		move(cursor, DISPLAY_M_LIST);

		if(cursor >= config.size.y - DISPLAY_M_CURS || main_redraw == 1)
		{
			main_display_files(main_display, cursor);
			main_redraw = 1;

			if(cursor == 0)
				main_redraw = 0;
		}
	}

	endwin();
	delwin(main_display.screen);
	delwin(preview_display.screen);
	return 0;
}
