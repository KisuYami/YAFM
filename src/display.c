#include <ncurses.h>

void displayFiles(char *list[255], int lenght) {

	for(int i = 0; i < lenght; i++)
		mvprintw(i+1, 3, "%s", list[i]);
}

void displayDirPath(char *path) {

	mvprintw(0, 0, "%s", path);
}
