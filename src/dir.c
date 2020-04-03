#include <sys/stat.h>
#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#include "dir.h"

static int
compare(const void *p1, const void *p2)
{
	return strcmp(*(char *const *)p1, *(char *const *)p2);
}

int
is_file(char *path)
{
	struct stat path_to_file;
	stat(path, &path_to_file);
	return S_ISDIR(path_to_file.st_mode);
}

int
list_files(display_t *dir_display, char *path)
{
	DIR *d = NULL;

	if(!path)
		getcwd(config.path, PATH_MAX);
		
	d = opendir((!path) ? config.path : path);

	if(!d)
		return -1;

	if(dir_display->files.mem_count == 0)
	{
		dir_display->files.list = calloc(FILE_LIST_SZ, sizeof(char **));
		dir_display->files.mem_count = FILE_LIST_SZ;
	}

	dir_display->files.size = 0;

	for(struct dirent *dir = readdir(d); dir != NULL; dir = readdir(d))
	{
		if(dir_display->files.size >= dir_display->files.mem_count)
		{
			dir_display->files.mem_count += FILE_LIST_SZ;
			dir_display->files.list = realloc(dir_display->files.list,
							  dir_display->files.mem_count *
							  sizeof(char **));
		}
		// Don't Show hidden files
		if(config.hidden || *dir->d_name != '.')
		{
			if(dir_display->files.size >= dir_display->files.mem_alloc)
			{
				dir_display->files.list[dir_display->files.size] =
					malloc(MAXNAMLEN);

				dir_display->files.mem_alloc++;
			}

			strcpy(dir_display->files.list[dir_display->files.size],
			       dir->d_name);

			dir_display->files.size++;
		}
	}

	closedir(d);

	if(dir_display->files.size == 0)
	{
		if(dir_display->files.marked)
			free(dir_display->files.marked);

		dir_display->files.marked = NULL;

		return -1;
	}

	qsort(&dir_display->files.list[0], dir_display->files.size,
	      sizeof(char *), compare);

	if(dir_display->files.marked)
		free(dir_display->files.marked);

	dir_display->files.marked = calloc(dir_display->files.size, sizeof(short int));

	return 0;
}

static const char *
file_extension(const char *filename)
{
	const char *dot = strrchr(filename, '.');

	if(!dot || dot == filename)
		return NULL;

	return dot + 1;
}

void
file_open(display_t *dir, int cursor)
{
	// Get the extension
	const char *extension = file_extension(dir->files.list[cursor]);

	if(!extension)
		return;

	int file_type = -1;

	if((strncmp(extension, "jpg", 3) == 0) ||
	   (strncmp(extension, "png", 3) == 0) ||
	   (strncmp(extension, "jpeg", 4) == 0))
		file_type = 0;

	else if((strncmp(extension, "mkv", 3) == 0) ||
		(strncmp(extension, "mp4", 3) == 0))
		file_type = 1;

	else if(strncmp(extension, "pdf", 3) == 0)
		file_type = 2;

	else if(file_type == -1)
		return; // Non listed file type found

	if(!config.envp[file_type])
	{
		fprintf(stderr, "YAFM: enviroment variable missing");
		return;
	}

	pid_t child = fork();

	if(child == 0)
	{
		int fd = open("/dev/null", O_WRONLY);
		// No output or input
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDERR_FILENO);
		close(fd);

		execlp(config.envp[file_type],
		       config.envp[file_type],
		       dir->files.list[cursor], (char *)NULL);
		exit(0);
	}
}
