#ifndef INTERACTION_H_
#define INTERACTION_H_

#include <cstddef>
#include <iterator>
#include <unistd.h>
#include <vector>
#include <string>

#include <stdio.h>
#include <ncurses.h>

#include "screen.hh"

class act_list {
    public:
	std::vector<std::string> list;
	class screen screen;

	void display_mark(void)
	{
		wclear(screen.screen);

		box(screen.screen, 0, 0);
		mvwprintw(screen.screen, 0, 3, "Clipboard");

		wrefresh(screen.screen);

		if (list.size() <= 0)
			return;

		for (int i = 0; i < list.size(); ++i)
			mvwprintw(screen.screen, i + 1, 2, "%s",
				  list[i].c_str());

		wrefresh(screen.screen);
	}

	void add(std::string file_name, size_t cursor)
	{
		for (auto i = list.begin(); i < list.end(); ++i) {
			std::string buf;

			buf = get_current_dir_name();
			buf += "/";
			buf += file_name;
			if (*i == buf) {
				list.erase(i);
				return;
			}
		}
		std::string buf;
		buf = get_current_dir_name();
		buf += "/";
		buf += file_name;
		list.push_back(buf);
	}

	void yank(char *file)
	{
		printf("STUB: YANK");
	}

	void move(char *file)
	{
		printf("STUB: MOVE");
	}

	void remove(char *file)
	{
		printf("STUB: REMOVE");
	}

	void paste(char *file)
	{
		printf("STUB: PASTE");
	}
};

#endif // INTERACTION_H_
