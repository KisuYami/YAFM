#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <ncurses.h>

#include "screen.hh"
#include "interaction.hh"
#include <ncurses/ncurses.h>

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

	class display d_primary("Primary", get_current_dir_name());

	d_primary.screen.create(y - 3, x / 2, 0, 0);
	d_primary.list_files(NULL);
	d_primary.update();

	class display d_secondary("Secondary", get_current_dir_name());
	d_secondary.path.append("/");
	d_secondary.path.append(d_primary.file_list[0].c_str());

	d_secondary.screen.create(y - 3, x / 2, 0, x / 2);
	d_secondary.list_files(NULL);
	d_secondary.update();

	class cpath cpath(get_current_dir_name(), getenv("USER"),
			  getenv("HOSTNAME"));

	cpath.screen.create(3, (x / 100) * 140, y - 3, 0);
	cpath.update();

	class counter counter(0, d_primary.file_list.size());
	counter.screen.create(3, (x / 100) * 30, y - 3, x - (x / 100) * 30);
	counter.update();

	class act_list act_list;

	char key;
	while ((key = wgetch(d_primary.screen.screen)) != 'q') {
		switch (key) {
		case 'k':
			d_primary.cursor -= (d_primary.cursor <= 0) ? 0 : 1;

			counter.cursor = d_primary.cursor;
			counter.update();
			break;

		case 'j':
			d_primary.cursor += (d_primary.cursor >=
					     d_primary.file_list.size() - 1) ?
							  0 :
							  1;

			counter.cursor = d_primary.cursor;
			counter.update();
			break;
		case 'h':
			chdir("../");

			d_primary.cursor = 0;
			d_primary.path = get_current_dir_name();
			d_primary.list_files(NULL);

			cpath.update();

			counter.cursor = d_primary.cursor;
			counter.ammount = d_primary.file_list.size();
			counter.update();
			break;
		case 'l':
			chdir(d_primary.file_list[d_primary.cursor].c_str());

			d_primary.cursor = 0;
			d_primary.path = get_current_dir_name();
			d_primary.list_files(NULL);

			cpath.update();

			counter.cursor = d_primary.cursor;
			counter.ammount = d_primary.file_list.size();
			counter.update();
			break;
		case '.':
			d_primary.hidden = !d_primary.hidden;
			d_primary.list_files(NULL);
			break;
		case ' ':
			act_list.list.push_back(
				d_primary.file_list[d_primary.cursor]);
			act_list.list_marks.push_back(d_primary.cursor);
			break;
		}

		d_primary.update();

		d_secondary.path = get_current_dir_name();
		d_secondary.path.append("/");
		d_secondary.path.append(
			d_primary.file_list[d_primary.cursor].c_str());

		d_secondary.list_files(NULL);
		d_secondary.update();

		act_list.display_mark(d_primary.screen.screen,
				      d_primary.file_list.size(),
				      d_primary.cursor);
	}

	delwin(d_primary.screen.screen);
	delwin(d_secondary.screen.screen);
	delwin(cpath.screen.screen);

	endwin();

	return 0;
}
