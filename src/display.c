#include <ncurses.h>
#include "dir.h"
#include "../config.h"

void setupScr() {

	initscr();
	cbreak();
        noecho();
	curs_set(0); start_color();
	//Colors
	init_pair(1, COLOR_RED,  COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
}


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
