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
char *cdBack(const char path[]) { // Don't do that, is wrong, but work

	char *newPath = cdEnter(path, "../");
	return newPath;
}
/*
char *cdBack(const char path[]) { // Don't work

	int i, f, z;
	char *newPath = calloc(25, 255);

	while(path[i] != '\0') {

		if(path[i] == '/')
			f = 0;

		newPath[z] = path[i];
		z++;
		f++;
		i++;
	}

	newPath = realloc(newPath, (sizeof(*path)) + 1);
	//newPath[sizeof(newPath)] = '\0';

	return newPath;
}
*/
