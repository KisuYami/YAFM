#ifndef INTERACTION_H_
#define INTERACTION_H_

#include <cstddef>
#include <vector>
#include <string>

#include <stdio.h>
#include <ncurses.h>

class act_list {
    public:
	std::vector<std::string> list;
	std::vector<int> list_marks;

	void display_mark(WINDOW *screen, size_t ammount, size_t cursor)
	{
		if (list.size() <= 0 || list_marks.size() <= 0)
			return;

		for (int i = cursor; i < ammount; i++) {
			if ((i - cursor) == list_marks[i])
				mvwprintw(screen, i + 1, 1, "->");
		}

		wrefresh(screen);
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
