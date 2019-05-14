#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
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

	size_t size_result = strlen(state) + strlen(text) + CD_ENTER_BUFFER_SIZE;
	char *result = malloc(size_result);

	snprintf(result, size_result, "%s/%s", state, text);

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
	char file[255];
	size_t sizeCommand = sizeof(*path);
	char *command = malloc(sizeof(sizeCommand) + 255);

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

	for(p = 0; p <= TYPES; p++) {
		if(strcmp(file, files_extensions[p].extension) == 0) {
			pid_t outProgram = fork();

			sprintf(command, "%s \"%s\" &>/dev/null", files_extensions[p].program, path); // Change to snprintf

			if(outProgram == 0) {
				if(system(command) == 0) { // Makes sure that the child of YAFM commit sepuko, and return their karma.
					free(command);
					exit(0);
				}
				else {
					free(command);
					exit(1);
				}
			} else if(outProgram > 0) {
				free(command);
			}

		}
	}

	return 0;
}

void shellActions(char path[], char file[], const char shellCommand[], char special[]) {

	char *command = malloc(sizeof(*path) + 279 + OPEN_FILE_BUFFER_SIZE); // 279 = Max name lenght in unix(255) plus the longest command i can think.

	sprintf(command, "%s %s \"%s%s\"", shellCommand, special, path, file);
	system(command);

	free(command);
}

void bulkRename(char path[]) {

	char *command = malloc(sizeof(*path) + 28);

	sprintf(command, "cd \"%s\" && nvim -c Renamer", path); //Lazy
	system(command);

	free(command);

}
