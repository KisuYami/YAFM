#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../config.h"
#include "dir.h"
#include "main.h"
#include "display.h"

void setupScr() {

	initscr();
	cbreak();
        noecho();
	curs_set(0); start_color();
	//Colors
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
}

int main() {

	setupScr();

	int listLenght;
	int cursor;
	char key;
	char pwd[100];
	char *list[LIST_LENGHT];
	char *newPwd = "nada";

	getcwd(pwd, sizeof(pwd));
	pwd[sizeof(pwd) - 1] = '\0';
	cursor = 0;

	listLenght = listFiles(list, ".");
	newPwd = cdEnter(pwd, "");
	displayDirPath(newPwd);
	displayFiles(list, listLenght, 0);

	while((key = getch()) != 'q') { //Mudar para Switch.

		// Editor Actions
		if(key == BULK_RENAME)
			shellActions(newPwd, "", EDITOR, "-c Renamer");

		if(key == OPEN_EDITOR)
			shellActions(newPwd, list[cursor], EDITOR, "");
		// End Editor Actions

		// File operations
		if(key == FILE_DELETE)
			shellActions(newPwd, list[cursor], "rm", "-rd");

		// Cursor Movements
		if(key == MOV_DOWN && cursor < (listLenght - 1))
			cursor++;

		if(key == MOV_UP && cursor > 0)
			cursor--;

		if(key == MOV_BOTTOM && cursor < (listLenght - 1))
			cursor = listLenght - 1;

		if(key == MOV_TOP && cursor > 0)
			cursor = 0;

		if(key == MOV_MIDDLE) {
			cursor = listLenght / 2;
			if(cursor < 0)
				cursor = 0;
		}

		if(key == MOV_RIGHT) {
			newPwd = cdEnter(newPwd, list[cursor]);
			cursor = 0;
		}

		if(key == MOV_LEFT) {
			newPwd = cdBack(newPwd);
			cursor = 0;
		}
		// End Cursor Movements

		clear();
		displayDirPath(newPwd);
		listLenght = listFiles(list, newPwd);
		displayFiles(list, listLenght, cursor);
	}

	endwin();
	return 0;
}
