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
	size_t size_path;
	char key;
	char *list[LIST_LENGHT];
	char *newPwd, *tmpPwd;

	cursor = 0;
	size_path = 256 * sizeof(char);
	newPwd = malloc(size_path);
	tmpPwd = malloc(size_path);

	// Initialize dir absolute path
	getcwd(tmpPwd, size_path);
	tmpPwd[size_path - 1] = '\0';
	snprintf(newPwd, size_path, "%s/", tmpPwd);
	free(tmpPwd);

	// Start base of program
	listLenght = listFiles(list, ".");
	displayDirPath(newPwd);
	displayFiles(list, listLenght, 0, 3);

	// Main loop
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
			/*
			case FILE_DELETE:
				shellActions(newPwd, list[cursor], "rm", "-rd");
				break;
			*/

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
				cdEnter(newPwd, list[cursor]);
				if(cursor >= listLenght || cursor < 0)
					cursor = 0;
				break;

			case MOV_LEFT:
				cdBack(newPwd);
				break;
			// End Cursor Movements

			default:
				break;
		}

		clear();

		listLenght = listFiles(list, newPwd);

		if(cursor >= listLenght - 1 || cursor < 0) // This should be aways after listFiles()
				cursor = listLenght - 1;

		displayDirPath(newPwd);
		displayFiles(list, listLenght, cursor, 3);
	}

	endwin();
	free(newPwd);
	return 0;
}
