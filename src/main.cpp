#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <ncurses.h>

#include "screen.hh"
#include <ncurses/ncurses.h>

using namespace std;

int main(int argc, char *argv[])
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	// Colors
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	int y, x;
	getmaxyx(stdscr, y, x);

	class display display_p;

	//WINDOW *win = newwin(y, x / 2, 0, 0);
	display_p.cursor = 0;
	display_p.hidden = false;
	display_p.path = get_current_dir_name();

	display_p.create(y, x / 2, 0, 0);
	display_p.list_files(NULL);
	display_p.update();

	char key;
	while ((key = wgetch(display_p.screen)) != 'q') {
		switch (key) {
		case 'k':
			display_p.cursor -= (display_p.cursor <= 0) ? 0 : 1;
			break;

		case 'j':
			display_p.cursor += (display_p.cursor >= y - 1) ? 0 : 1;
			break;
		case 'h':
			chdir("../");
			display_p.path = get_current_dir_name();
			display_p.list_files(NULL);
			break;
		case 'l':
			chdir(display_p.file_list[display_p.cursor].c_str());
			display_p.path = get_current_dir_name();
			display_p.list_files(NULL);
			break;
		case '.':
			display_p.hidden = !display_p.hidden;
			display_p.list_files(NULL);
			break;
		}
		display_p.update();
	}

	display_p.clean();
	endwin();

	return 0;
}
