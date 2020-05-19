#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "dir.h"
#include "display.h"
#include "../config.h"

static int compare(const void *p1, const void *p2)
{
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int listFiles(char *l[LIST_LENGHT], char *cwd) {

	int i = 0;
	DIR *d;
	struct dirent *dir;
	memset(l, 0, LIST_LENGHT);

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

	qsort(&l[0], i, sizeof(char *), compare);

	return i;
}

int isFile(char *path) {
    struct stat path_to_file;
    stat(path, &path_to_file);
    return S_ISREG(path_to_file.st_mode);
}

void previewDir(char *path, char *text) {

	size_t size_result = sizeof(char) * (256 + strlen(path) + strlen(text));
	int Y, previewLenght;
	char *previewList[LIST_LENGHT];
	char previewPwd[size_result];

	snprintf(previewPwd, size_result, "%s/%s", path, text);

	Y = getmaxx(stdscr);
	previewLenght = listFiles(previewList, previewPwd);

	if(isFile(previewPwd) != 1)
		displayFiles(path, previewList, previewLenght, -1, (Y / 2) + 3);
}

void cdEnter(char *path, const char workingFile[]) {

	size_t size_result = sizeof(char) * (256 + strlen(path) + strlen(workingFile));
	char *result = malloc(size_result);

	snprintf(result, size_result, "%s/%s", path, workingFile);

	if(isFile(result) == 1)
		openFile(result);
	else
		memcpy(path, result, size_result);

	free(result);
}

void cdBack(char *path) {

	int i;
	i = strlen(path) - 2;

	while(i >= 0) {

		if(path[i] == '/') {
			path[i] = '\0';
			break;
		}

		else {
			path[i] = '\0';
			i--;
		}
	}

}

int openFile(char path[]) {

	int i, p;
	char file[255];
	size_t size_command = sizeof(*path) * sizeof(char);
	char *command = malloc(size_command);

	i = strlen(path);
	p = 0;

	while(i >= 0) {

		if(path[i] == '.')
			break;

		i--;
		p++; // Get the size of th extension
	}

	i = strlen(path);
	while(p >= 0) {
		file[p] = path[i]; // Get the extension as decrement the p.
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
			} else if(outProgram >= 1) {
				free(command);
				break;
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
