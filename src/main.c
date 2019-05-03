#include <ncurses.h>
#include "main.h"
#include "dir.h"
#include "display.h"

void setupScr() {

	initscr();
	cbreak();
        noecho();
}

int main() {

	setupScr();

	int listLenght;
	char *list[255];

	listLenght = listFiles(list);
	displayFiles(list, listLenght);

	getch();
	endwin();
	return 0;
}
