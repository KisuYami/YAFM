#ifndef SCREEN_H_
#define SCREEN_H_

#include <vector>
#include <string>

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <ncurses/ncurses.h>

class display {
    public:
	WINDOW *screen;

	std::string path;
	bool hidden;
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

	void list_files(char *new_path)
	{
		file_list.clear();

		DIR *d = opendir((!new_path) ? path.c_str() : new_path);

		if (!d)
			return;

		for (struct dirent *dir = readdir(d); dir != NULL;
		     dir = readdir(d)) {
			if (hidden || *dir->d_name != '.')
				file_list.push_back(dir->d_name);
		}
	}
};

class path {
    public:
	WINDOW *screen;

	std::string prefix;
	std::string path;

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

	void update(void)
	{
		box(screen, 0, 0);
		mvwprintw(screen, 1, 1, "%s: %s", prefix.c_str(), path.c_str());
		wrefresh(screen);
	}
};
#endif // SCREEN_H_
