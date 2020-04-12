#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <ncurses.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../config.h"
#include "dir.h"

/*************************************************************/
/* This module is used to get information about directories. */
/*************************************************************/

struct {
	short int type; // 0 = Image, 1 = Video, 2 = Document
	char *str;
} file_ext_list[] = {
	{0, "jpg"}, {0, "png"},	 {0, "jpeg"}, {1, "mkv"}, {1, "mp4"},
	{1, "avi"}, {1, "webm"}, {2, "pdf"},  {-1, NULL},
};

static int compare(const void *p1, const void *p2)
{
	return strcmp(*(char *const *)p1, *(char *const *)p2);
}

int is_file(char *path)
{
	struct stat path_to_file;
	stat(path, &path_to_file);
	return S_ISDIR(path_to_file.st_mode);
}

int list_files(struct files *files, char *path)
{
	if (!path)
		getcwd(config.path, PATH_MAX);

	DIR *d = opendir((!path) ? config.path : path);

	if (!d)
		return -1;

	if (0 == files->mem_count) {
		files->list = calloc(FILE_LIST_SZ, sizeof(char **));
		files->mem_count = FILE_LIST_SZ;
	}

	files->size = 0;

	for (struct dirent *dir = readdir(d); dir != NULL; dir = readdir(d)) {
		if (files->size >= files->mem_count) {
			files->mem_count += FILE_LIST_SZ;
			files->list = realloc(files->list, files->mem_count *
					      sizeof(char **));
		}
		// Don't Show hidden files
		if (config.hidden || *dir->d_name != '.') {
			if (files->size >= files->mem_alloc) {
				files->list[files->size] = malloc(MAXNAMLEN);
				files->mem_alloc++;
			}

			strcpy(files->list[files->size], dir->d_name);
			files->size++;
		}
	}

	closedir(d);

	if (files->size == 0) {
		if (files->marked)
			free(files->marked);

		files->marked = NULL;

		return -1;
	}

	qsort(&files->list[0], files->size, sizeof(char *), compare);

	if (files->marked)
		free(files->marked);

	files->marked = calloc(files->size, sizeof(short int));

	return 0;
}

int file_open(char *file_name)
{
	// Get the extension
	const char *extension = strrchr(file_name, '.');

	if (!extension)
		return -1;

	int file_type = -1;

	for (int i = 0; file_ext_list[i].type != -1; ++i) {
		if (extension[1] == file_ext_list[i].str[0]) {
			file_type = file_ext_list[i].type;
			break;
		}
	}

	if (file_type == -1 || !config.envp.defaults[file_type])
		return -1;

	if (fork() == 0) {
		int fd = open("/dev/null", O_WRONLY);
		// No output or input
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDERR_FILENO);
		close(fd);

		execlp(config.envp.defaults[file_type], config.envp.defaults[file_type],
		       file_name, (char *)NULL);
		exit(0);
	}

	return 0;
}
