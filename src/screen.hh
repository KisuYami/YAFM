#ifndef SCREEN_H_
#define SCREEN_H_

#include <vector>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <ncurses/ncurses.h>

class display {
    public:
	WINDOW *screen;
	int cursor;

	std::vector<std::string> file_list;

	int y[2];
	int x[2];

	void create(int nlines, int ncols, int begin_y, int begin_x)
	{
		x[0] = begin_x;
		x[1] = nlines;

		y[0] = begin_y;
		y[1] = ncols;

		screen = newwin(x[1], y[1], y[0], x[0]);
		if (screen == NULL) {
			delwin(screen);
			perror("KYFM: Failed to create window.");

			exit(-1);
		}
		box(screen, 0, 0);
	}

	void update()
	{
		wclear(screen);
		for (int i = cursor; (i < file_list.size()) && (i < x[1]);
		     ++i) {
			if (i == cursor) {
				wattron(screen, A_REVERSE);
				mvwprintw(screen, i - cursor + 1, 2,
					  file_list[i].c_str());
				wattroff(screen, A_REVERSE);
			} else
				mvwprintw(screen, i - cursor + 1, 2,
					  file_list[i].c_str());
		}

		box(screen, 0, 0);
		wrefresh(screen);
	}

	void refresh()
	{
		wrefresh(screen);
	}

	void clean()
	{
		delwin(screen);
	}
};
#endif // SCREEN_H_
