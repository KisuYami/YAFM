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

	// Main Display
	struct dir_display m_display = {
		.screen = newwin(config.size.y - 1, config.size.x / 2,
				 0, 0),

		.position = (struct position) {
			.y[0] = 0,
			.y[1] = config.size.y - 1,
			.x[0] = 0,
			.x[1] = config.size.x / 2,
		},
	};

	// Preview Display
	struct dir_display p_display = {
		.screen = newwin(config.size.y - 1, config.size.x / 2,
				 0, config.size.x / 2),

		.position = (struct position) {
			.y[0] = 0,
			.y[1] = config.size.y - 1,
			.x[0] = config.size.x / 2,
			.x[1] = config.size.x / 2,
		}
	};

	int cursor = 0;

	clear();
	refresh();

	list_files(&m_display, NULL);
	main_display_files(m_display, cursor);
	preview_display_files(&m_display, &p_display, cursor);

	display_path();

	move(cursor, DISPLAY_MARGIN_LIST);

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
			if(cursor+1 < m_display.files.size) cursor++;
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

				clear();
				refresh();

				list_files(&m_display, NULL);
				main_display_files(m_display, cursor);

				display_path();
			}
			break;
		}
		case KEY_MOV_RIGHT:
			if(chdir(m_display.files.list[cursor]) == 0)
			{
				cursor = 0;

				clear();
				refresh();

				list_files(&m_display, NULL);
				main_display_files(m_display, cursor);

				display_path();
			}

			break;

		case KEY_FILE_OPEN:
			file_open(&m_display, cursor);
			break;

		case KEY_FILE_MARK:
			m_display.files.marked[cursor] =
				!m_display.files.marked[cursor];
			break;

		case KEY_FILE_HIDDEN:
			config.hidden = !config.hidden;
			list_files(&m_display, NULL);
			main_display_files(m_display, cursor);
			break;

		case KEY_FILE_DEL:
			if(selection_del(&m_display) == 0)
			{
				cursor = 0;
				list_files(&m_display, NULL);
				main_display_files(m_display, cursor);
			}
			break;

		case KEY_FILE_YANK:
			selection_yank(&m_display);
			break;

		case KEY_FILE_CUT:
			selection_cut(&m_display);
			break;

		case KEY_FILE_PASTE:
			selection_paste();
			list_files(&m_display, NULL);
			main_display_files(m_display, cursor);
			break;
		}

		// If terminal size has changed, update the window size
		if(is_term_resized(config.size.y, config.size.x) == true)
		{
			getmaxyx(stdscr, config.size.y, config.size.x);

			delwin(m_display.screen);
			delwin(p_display.screen);

			m_display = (struct dir_display) {
				.screen = newwin(config.size.x / 2, config.size.y - 1,
						 0, 0),

				.position = (struct position) {
					.y[0] = 0,
					.y[1] = config.size.y - 1,
					.x[0] = 0,
					.x[1] = config.size.x / 2,
				},
			};

			// Preview Display
			p_display = (struct dir_display) {
				.screen = newwin(config.size.y - 1, config.size.x / 2,
						 0, config.size.x / 2),

				.position = (struct position) {
					.y[0] = 0,
					.y[1] = config.size.y - 1,
					.x[0] = config.size.x / 2,
					.x[1] = config.size.x / 2,
				}
			};

		}

		preview_display_files(&m_display, &p_display, cursor);
		move(cursor, DISPLAY_MARGIN_LIST);
	}

	endwin();
	delwin(m_display.screen);
	delwin(p_display.screen);
	return 0;
}
