#ifndef SCREEN_H_
#define SCREEN_H_

#include <vector>
#include <string>

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <ncurses/ncurses.h>

class screen {
    public:
	WINDOW *screen;

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
};

class display {
    public:
	int cursor;
	bool hidden;

	std::string name;
	std::string path;

	std::vector<std::string> file_list;

	class screen screen;

	display(std::string n_name, char *n_path)
	{
		cursor = 0;
		hidden = false;

		name = n_name;
		path = n_path;
	}

	void update()
	{
		wclear(screen.screen);
		box(screen.screen, 0, 0);

		for (int i = cursor;
		     (i < file_list.size()) && ((i - cursor + 1) < screen.x[1]);
		     ++i) {
			if (i == cursor) {
				wattron(screen.screen, A_REVERSE);
				mvwprintw(screen.screen, i - cursor + 1, 4,
					  file_list[i].c_str());
				wattroff(screen.screen, A_REVERSE);
			} else
				mvwprintw(screen.screen, i - cursor + 1, 4,
					  file_list[i].c_str());
		}

		mvwprintw(screen.screen, 0, 3, name.c_str());
		wrefresh(screen.screen);
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

class cpath {
    public:
	std::string prefix;
	std::string path;

	class screen screen;
	cpath(std::string n_path, std::string username, std::string hostname)
	{
		path = n_path;

		prefix = username;
		prefix.append("@");
		prefix.append(hostname);
	}

	void update(void)
	{
		box(screen.screen, 0, 0);

		path = get_current_dir_name();
		mvwprintw(screen.screen, 0, 3, "Path");
		mvwprintw(screen.screen, 1, 2, "%s: %s", prefix.c_str(),
			  path.c_str());
		wrefresh(screen.screen);
	}
};

class counter {
    public:
	int cursor;
	int ammount;

	class screen screen;
	counter(int n_cursor, int n_ammount)
	{
		cursor = n_cursor;
		ammount = n_ammount;
	}

	void update(void)
	{
		box(screen.screen, 0, 0);

		mvwprintw(screen.screen, 0, 3, "File Counter");

		// +1 so the counter starts at 1 not 0
		mvwprintw(screen.screen, 1, 2, "%d/%d", cursor + 1, ammount);
		wrefresh(screen.screen);
	}
};
#endif // SCREEN_H_
