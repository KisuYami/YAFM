#include <dirent.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dir.h"
#include "../config.h"

int listFiles(char *l[255], char *cwd) {

	int i = 0;
	DIR *d;
	struct dirent *dir;

	d = opendir(cwd);

	if(d) {
		while((dir = readdir(d)) != NULL) {
			if(*dir->d_name != '.') { // Don't Show hidden files
				l[i] = dir->d_name;
				i++;
			/*} if(hidden == 1) { // Show hidden files: don't work
				l[i] = dir->d_name;
				i++;
			*/}
		}

		closedir(d);
	}
	return i;
}

int isFile(const char *path) {
    struct stat path_to_file;
    stat(path, &path_to_file);
    return S_ISREG(path_to_file.st_mode);
}

char *cdEnter(char state[], const char text[]) {

	char *result = malloc(strlen(state) + strlen(text) + 1024);

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

int openFile(const char path[]) {

	int i, p;
	char *file = malloc(256);
	char *command = malloc(sizeof(*path) + 1024);

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
			strcpy(command, files_extensions[p].program);
			strcat(command, " '");
			strcat(command, path);
			strcat(command, "'");
			strcat(command, " &>/dev/null"); // No terminal output
			strcat(command, " &");
			system(command);
			free(command);
			return 1;
		}
	}

	return 0;
}

void editorActions(char path[], char file[], char special[]) {

	char *command = malloc(sizeof(*path) +
			sizeof(*file) + sizeof(*special) + 25);

	strcpy(command, "nvim ");
	strcat(command, special);
	strcat(command, " '");
	strcat(command, path);
	strcat(command, "/");
	strcat(command, file);
	strcat(command, "'");
	system(command);
	free(command);
}
