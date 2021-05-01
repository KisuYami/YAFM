#include <cstdio>
#include <iostream>
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
	display_p.create(y, x / 2, 0, 0);
	display_p.cursor = 0;
	display_p.update();

	char key;
	while ((key = wgetch(display_p.screen)) != 'q') {
		switch (key) {
		case 'k':
			display_p.cursor -= (display_p.cursor <= 0) ? 0 : 1;
			display_p.update();
			break;

		case 'j':
			display_p.cursor += (display_p.cursor >= y - 1) ? 0 : 1;
			display_p.update();
			break;
		}
	}

	display_p.clean();
	endwin();

	return 0;
}
