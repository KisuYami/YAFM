#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "../config.h"
#include "dir.h"
#include "display.h"

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

	while((key = getch()) != 'q') {

		switch(key) {
			// Editor Actions
			case BULK_RENAME:
				bulkRename(newPwd);
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
				if(cursor >= listLenght || cursor < 0)
					cursor = 0;
				break;

			case MOV_LEFT:
				newPwd = cdBack(newPwd);
				break;
			// End Cursor Movements

			default:
				break;
		}

		clear();

		displayDirPath(newPwd);
		listLenght = listFiles(list, newPwd);

		if(cursor >= listLenght - 1 || cursor < 0) // This should be aways after listFiles()
				cursor = listLenght - 1;

		displayFiles(list, listLenght, cursor);
	}

	endwin();
	return 0;
}
