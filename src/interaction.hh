#ifndef INTERACTION_H_
#define INTERACTION_H_

#include <cstddef>
#include <iterator>
#include <unistd.h>
#include <vector>
#include <string>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/wait.h>

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

	void action(char op)
	{
		switch (op) {
		case 'r':
			list.insert(list.begin(), "-r");
			list.insert(list.begin(), "rm");
			break;
		case 'c':
			list.insert(list.begin(), "-r");
			list.insert(list.begin(), "cp");
			list.push_back(get_current_dir_name());
			break;
		case 'm':
			list.insert(list.begin(), "mv");
			list.push_back(get_current_dir_name());
			break;
		}

		char **argv = act_list::argv();

		switch (fork()) {
		case 0:
			execvp(*argv, argv);
			exit(-1);
		default:
			int status;
			wait(&status);
		}

		list.clear();
		display_mark();
		act_list::free_argv(argv);
	}

    private:
	char **argv(void)
	{
		char **argv = (char **)malloc(list.size() * sizeof(char **));

		for (int i = 0; i < list.size(); ++i)
			argv[i] = strdup(list[i].c_str());

		return argv;
	}
	void free_argv(char **argv)
	{
		for (int i = 0; i < list.size(); ++i)
			free(argv[i]);

		free(argv);
	}
};

#endif // INTERACTION_H_
