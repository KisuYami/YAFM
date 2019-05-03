#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include "dir.h"
#include "main.h"
#include "display.h"

void setupScr() {

	initscr();
	cbreak();
        noecho();
}

int main() {

	setupScr();

	int listLenght;
	char key;
	char pwd[100];
	char *list[255];

	getcwd(pwd, sizeof(pwd)); /**< Get working dir */
	pwd[sizeof(pwd) - 1] = '\0';

	listLenght = listFiles(list);

	while((key = getch()) != 'q') {

		displayFiles(list, listLenght);
		displayDirPath(pwd);
	}

	endwin();
	return 0;
}
