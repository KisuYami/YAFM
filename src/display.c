#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
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

void displayFiles(char *path, char *list[LIST_LENGHT], int lenght, int cursor, int startPoint) {

	char **tmpList = list;
	size_t size_pathFile = sizeof(char) * (256 + sizeof(*path));
	char fullFile[size_pathFile];

	for(int i = 0; i < lenght; i++) {

		snprintf(fullFile, size_pathFile, "%s/%s", path, tmpList[i]);

		if(i == cursor) {

			attron(A_STANDOUT | COLOR_PAIR(1));
			mvprintw(i+1, startPoint, "%s", tmpList[i]);
			attroff(A_STANDOUT | COLOR_PAIR(1));

		} else if(isFile(fullFile) != 0 && COLORED_FILES == 0) {

			attron(A_BOLD | COLOR_PAIR(3));
			mvprintw(i+1, startPoint, "%s", tmpList[i]);
			attroff(A_BOLD | COLOR_PAIR(3));

		} else
			mvprintw(i+1, startPoint, "%s", tmpList[i]);
	}
}

void displayDirPath(char *path) {

	attron(A_UNDERLINE | COLOR_PAIR(2));
	mvprintw(0, 0, "%s", path);
	attroff(A_UNDERLINE | COLOR_PAIR(2));
}
