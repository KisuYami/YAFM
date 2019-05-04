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
			l[i] = dir->d_name;
			i++;
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

char *cdBack(const char path[]) {// Don't work

	int i, f;
	char *newPath = calloc(25, 255);

	while(path[i] != '\0') {

		if(path[i] == '/')
			f = 0;

		f++;
		i++;
	}

	newPath = realloc(newPath, (sizeof(*path) - f) + 1);
	newPath[sizeof(newPath)] = '\0';

	return newPath;
}
