#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "dir.h"
#include "display.h"
#include "interaction.h"
#include "../config.h"

int
main(void)
{
	screen_setup();

	// Needed to open files with externel programs
	config.envp[0] = getenv("IMAGE");
	config.envp[1] = getenv("VIDEO");
	config.envp[2] = getenv("DOCUMENTS");

	display_t main_display, preview_display;

	init_displays(&main_display, &preview_display);

	int cursor = 0;
	int main_redraw = 0;

	move(cursor, DISPLAY_M_LIST);

	for(char key = getch(); key != KEY_QUIT; key = getch())
	{
		handle_input(&main_display, &cursor, key);
		preview_display_files(&main_display, &preview_display, cursor);

		// If terminal size has changed, update the window size
		if(is_term_resized(config.size.y, config.size.x) == 1)
		{
			getmaxyx(stdscr, config.size.y, config.size.x);

			delwin(main_display.screen);
			delwin(preview_display.screen);

			init_displays(&main_display, &preview_display);
		}

		move(cursor, DISPLAY_M_LIST);

		/**************************************************************/
                /*     The window shall be redrawed after the cursor value    */
		/* surpasses the screen size, after that the window will only */
		/* stop to redraw after the cursor hits the top of the window.*/
                /**************************************************************/
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
