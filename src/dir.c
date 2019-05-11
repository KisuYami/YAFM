#include <dirent.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "dir.h"
#include "../config.h"

int listFiles(char *l[LIST_LENGHT], char *cwd) {

	int i = 0;
	DIR *d;
	struct dirent *dir;

	d = opendir(cwd);

	if(d) {
		while((dir = readdir(d)) != NULL) {
			if(*dir->d_name != '.') { // Don't Show hidden files
				l[i] = dir->d_name;
				i++;
			}
		}

		closedir(d);
	}
	return i;
}

int isFile(char *path) {
    struct stat path_to_file;
    stat(path, &path_to_file);
    return S_ISREG(path_to_file.st_mode);
}

char *cdEnter(char state[], const char text[]) {

	char *result = malloc(strlen(state) + strlen(text) + CD_ENTER_BUFFER_SIZE);

	strcpy(result, state);
	strcat(result, "/");
	strcat(result, text);

	if(isFile(result) == 1) {
		openFile(result);
		return state; // A real free one
	}

	return result;
}

char *cdBack(char path[]) {

	int i;
	char *newPath = malloc(sizeof(*path) + 1);

	newPath = path;
	i = strlen(newPath) - 2;

	while(i >= 0) {

		if(newPath[i] == '/') {
			newPath[i] = '\0';
			break;
		}

		else {
			newPath[i] = '\0';
			i--;
		}
	}

	newPath = realloc(newPath, 256);
	return newPath;
}

int openFile(char path[]) {

	int i, p;
	char *file = malloc(256);

	i = strlen(path);
	p = 0;

	while(i >= 0) {

		if(path[i] == '.')
			break;

		i--;
		p++;
	}

	i = strlen(path);
	while(p >= 0) {
		file[p] = path[i];
		p--;
		i--;
	}

	file = realloc(file, (sizeof(*path) + 1));
	for(p = 0; p < TYPES; p++) {
		if(strcmp(file, files_extensions[p].extension) == 0) {
			shellActions(path, "", files_extensions[p].program, "");
			return 1;
		}
	}

	return 0;
}

void shellActions(char path[], char file[], const char shellCommand[], char special[]) {

	char *command = malloc(sizeof(*path) +
			sizeof(*file) + sizeof(*special) + sizeof(*command) + OPEN_FILE_BUFFER_SIZE);

	strcpy(command, shellCommand); //Feio
	strcat(command, " ");
	strcat(command, special);
	strcat(command, " \"");
	strcat(command, path);
	strcat(command, file);
	strcat(command, "\"");
	if(strcmp(shellCommand, EDITOR))
		strcat(command, " &>/dev/null");
	system(command);
	free(command);
}
