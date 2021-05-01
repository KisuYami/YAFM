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

	class display d_primary, d_secondary;

	//WINDOW *win = newwin(y, x / 2, 0, 0);
	d_primary.cursor = 0;
	d_primary.hidden = false;
	d_primary.path = get_current_dir_name();

	d_primary.create(y - 3, x / 2, 0, 0);
	d_primary.list_files(NULL);
	d_primary.update();

	d_secondary.cursor = 0;
	d_secondary.hidden = false;
	d_secondary.path = get_current_dir_name();
	d_secondary.path.append("/");
	d_secondary.path.append(d_primary.file_list[0].c_str());

	d_secondary.create(y - 3, x / 2, 0, x / 2);
	d_secondary.list_files(NULL);
	d_secondary.update();

	class path cpath;

	cpath.create(3, x, y - 3, 0);
	cpath.prefix = "user@test";

	cpath.prefix = getenv("USER");
	cpath.prefix.append("@");
	cpath.prefix.append(getenv("HOSTNAME"));

	cpath.path = get_current_dir_name();
	cpath.update();

	char key;
	while ((key = wgetch(d_primary.screen)) != 'q') {
		switch (key) {
		case 'k':
			d_primary.cursor -= (d_primary.cursor <= 0) ? 0 : 1;
			break;

		case 'j':
			d_primary.cursor += (d_primary.cursor >= y - 4) ? 0 : 1;
			break;
		case 'h':
			chdir("../");

			d_primary.cursor = 0;
			d_primary.path = get_current_dir_name();
			d_primary.list_files(NULL);

			cpath.path = get_current_dir_name();
			cpath.update();
			break;
		case 'l':
			chdir(d_primary.file_list[d_primary.cursor].c_str());

			d_primary.cursor = 0;
			d_primary.path = get_current_dir_name();
			d_primary.list_files(NULL);

			cpath.path = get_current_dir_name();
			cpath.update();
			break;
		case '.':
			d_primary.hidden = !d_primary.hidden;
			d_primary.list_files(NULL);
			break;
		}
		d_primary.update();

		d_secondary.path = get_current_dir_name();
		d_secondary.path.append("/");
		d_secondary.path.append(
			d_primary.file_list[d_primary.cursor].c_str());

		d_secondary.list_files(NULL);
		d_secondary.update();
	}

	delwin(d_primary.screen);
	delwin(d_secondary.screen);
	delwin(cpath.screen);

	endwin();

	return 0;
}
