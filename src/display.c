#include <ncurses.h>

void displayFiles(char *list[255], int lenght) {

	for(int i = 0; i < lenght; i++)
		mvprintw(i, 0, "%s", list[i]);
}
