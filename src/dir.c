#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <magic.h>
#include <fcntl.h>
#include <dirent.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "dir.h"
#include "display.h"

static int compare(const void *p1, const void *p2) {
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

int file_list(struct working_dir *changing_dir)
{

    int i = 0;
    DIR *d = NULL;
    struct dirent *dir;

    getcwd(changing_dir->path, PATH_MAX);
    d = opendir(changing_dir->path);

    while ((dir = readdir(d)) != NULL) {
		if(changing_dir->config.hidden_files) {
            changing_dir->file[i] = dir->d_name;
            i++;
		}
        else if (*dir->d_name != '.') { // Don't Show hidden files
            changing_dir->file[i] = dir->d_name;
            i++;
        }
    }

    closedir(d);

    changing_dir->num_files = i;
    qsort(&changing_dir->file[0], i, sizeof(char *), compare);

    return i;
}

int is_file(char *path) {
    struct stat path_to_file;
    stat(path, &path_to_file);
    return S_ISREG(path_to_file.st_mode);
}

int cd_enter(struct working_dir *changing_dir)
{

    if(changing_dir->cursor >= changing_dir->num_files)
        return 1;

    if(!is_file(changing_dir->file[changing_dir->cursor]))
        chdir(changing_dir->file[changing_dir->cursor]);

    file_list(changing_dir);

    return 0;
}

void cd_back(struct working_dir *changing_dir) {

    size_t i;

	for(i = strlen(changing_dir->path); i >= 0; --i) {

        if (changing_dir->path[i] == '/') {

            changing_dir->path[i] = '\0';
            break;

        }

    }

    if(!is_file(changing_dir->path))
        chdir(changing_dir->path);

    file_list(changing_dir);
}

void file_open(struct working_dir *dir)
{
	const char *mime;
	char string[1024];
	magic_t magic;

	magic = magic_open(MAGIC_MIME_TYPE);
	magic_load(magic, NULL);
	mime = magic_file(magic, dir->file[dir->cursor]);

	if(strncmp(mime, "image", 5) == 0)
		sprintf(string, "%s %s &> /dev/null", dir->config.env[0],
				dir->file[dir->cursor]);

	else if(strncmp(mime, "video", 5) == 0)
		sprintf(string, "%s %s &> /dev/null", dir->config.env[1],
				dir->file[dir->cursor]);

	else if(strncmp(mime+12, "pdf", 5) == 0)
		sprintf(string, "%s %s &> /dev/null", dir->config.env[2],
				dir->file[dir->cursor]);

	if(*string != '\0') {
		if(fork()) {
			system(string);
			exit(0);
		}
	}
	magic_close(magic);
	return;
}
