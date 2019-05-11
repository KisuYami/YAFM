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
		switch(key) {
			case BULK_RENAME:
				shellActions(newPwd, "", EDITOR, "-c Renamer");
				break;

			case OPEN_EDITOR:
				shellActions(newPwd, list[cursor], EDITOR, "");
				break;
			// End Editor Actions

			// File operations
			case FILE_DELETE:
				shellActions(newPwd, list[cursor], "rm", "-rd");
				break;

			// Cursor Movements
			case MOV_DOWN:
				if(cursor < (listLenght - 1))
					cursor++;
				break;

			case MOV_UP:
				if(cursor > 0)
					cursor--;
				break;

			case MOV_BOTTOM:
				if(cursor < (listLenght - 1))
					cursor = listLenght - 1;
				break;

			case MOV_TOP:
				if(cursor > 0)
					cursor = 0;
				break;

			case MOV_MIDDLE:
				cursor = listLenght / 2;
				if(cursor < 0)
					cursor = 0;
				break;

			case MOV_RIGHT:
				newPwd = cdEnter(newPwd, list[cursor]);
				cursor = 0;
				break;

			case MOV_LEFT:
				newPwd = cdBack(newPwd);
				cursor = 0;
				break;
			default:
				break;
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
