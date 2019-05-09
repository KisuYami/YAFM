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

	while((key = getch()) != 'q') {

		if(key == BULK_RENAME)
			editorActions(newPwd, "", "-c Renamer");

		if(key == OPEN_EDITOR)
			editorActions(newPwd, list[cursor], "");

		if(key == MOV_DOWN && cursor < (listLenght - 1))
			cursor++;

		if(key == MOV_UP && cursor > 0)
			cursor--;

		if(key == MOV_RIGHT) {
			newPwd = cdEnter(newPwd, list[cursor]);
			cursor = 0;
		}

		if(key == MOV_LEFT) {
			newPwd = cdBack(newPwd);
			cursor = 0;
		}

		clear();
		displayDirPath(newPwd);
		listLenght = listFiles(list, newPwd);
		displayFiles(list, listLenght, cursor);
	}

	endwin();
	return 0;
}
