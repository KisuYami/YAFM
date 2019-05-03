#include <dirent.h>
#include <stddef.h>
int listFiles(char *l[255]) {

	int i = 0;
	DIR *d;
	struct dirent *dir;

	d = opendir(".");

	if(d) {
		while((dir = readdir(d)) != NULL) {
			l[i] = dir->d_name;
			i++;
		}

		closedir(d);
	}
	return i;
}
