#include <dirent.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
			}
		}

		closedir(d);
	}
	return i;
}

char *cdEnter(const char state[], const char text[]) { // Works

	char *result = malloc(strlen(state) + strlen(text) + 1);

	strcpy(result, state);
	strcat(result, "/");
	strcat(result, text);

	return result;
}
/*
char *cdBack(const char path[]) { // Don't do that, is wrong, but work

	char *newPath = cdEnter(path, "../");
	return newPath;
}
*/
char *cdBack(char path[]) { // Don't work

	int i, f;
	char *newPath = calloc(1, sizeof(*path));

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

	newPath = realloc(newPath, (sizeof(*newPath)) + 2);
	return newPath;
}
