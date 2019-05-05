#include <ncurses.h>
#include "dir.h"

void displayFiles(char *list[255], int lenght, int cursor) {

	for(int i = 0; i < lenght; i++) {
		if(i == cursor) {

			attron(A_STANDOUT | COLOR_PAIR(1));
			mvprintw(i+1, 3, "%s", list[i]);
			attroff(A_STANDOUT | COLOR_PAIR(1));

		} else if(isFile(list[i]) != 0) {

			attron(A_BOLD | COLOR_PAIR(3));
			mvprintw(i+1, 3, "%s", list[i]);
			attroff(A_BOLD | COLOR_PAIR(3));

		} else
			mvprintw(i+1, 3, "%s", list[i]);
	}
}

void displayDirPath(char *path) {

	attron(A_UNDERLINE | COLOR_PAIR(2));
	mvprintw(0, 0, "%s", path);
	attroff(A_UNDERLINE | COLOR_PAIR(2));
}
