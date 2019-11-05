#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "dir.h"
#include "display.h"

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
    return S_ISREG(path_to_file.st_mode);
}

int
file_list(dir_t *changing_dir)
{

    int i = 0;
    DIR *d = NULL;
    struct dirent *dir = NULL;

    getcwd(changing_dir->path, PATH_MAX);
    d = opendir(changing_dir->path);

    while ((dir = readdir(d)) != NULL)
    {
        if(changing_dir->config.hidden_files)
        {
            changing_dir->file[i] = dir->d_name;
            i++;
        }
        else if (*dir->d_name != '.')   // Don't Show hidden files
        {
            changing_dir->file[i] = dir->d_name;
            i++;
        }
    }

    closedir(d);

    changing_dir->num_files = i;
    qsort(&changing_dir->file[0], i, sizeof(char *), compare);

    return i;
}

int
cd_enter(dir_t *changing_dir)
{

    if(changing_dir->cursor >= changing_dir->num_files)
        return 1;

    if(!is_file(changing_dir->file[changing_dir->cursor]))
        chdir(changing_dir->file[changing_dir->cursor]);

    file_list(changing_dir);

    return 0;
}

void
cd_back(dir_t *changing_dir)
{

    for(size_t i = strlen(changing_dir->path); i >= 0; --i)
    {
        if (changing_dir->path[i] == '/')
        {
            changing_dir->path[i] = '\0';
            break;
        }
    }

    if(!is_file(changing_dir->path))
        chdir(changing_dir->path);

    file_list(changing_dir);
}

const char *
file_extension(const char *filename)
{
    const char *dot = strrchr(filename, '.');

    if(!dot || dot == filename)
        return "";

    return dot + 1;
}

void
file_open(dir_t *dir)
{
    // Get the extension
    const char *extension = file_extension(dir->file[dir->cursor]);

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

	if(!dir->config.env[file_type])
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

		execlp(dir->config.env[file_type], dir->config.env[file_type], dir->file[dir->cursor]);
        exit(0);
    }

    return;
}

int
file_delete(dir_t *changing_dir)
{
    if(display_confirm(changing_dir->screen, 2, "Proceed with deletion of ",
                        changing_dir->file[changing_dir->cursor], "?") == 0)
	{
		char absolute_path[PATH_MAX];
        snprintf(absolute_path, PATH_MAX + 2, "%s/%s", changing_dir->path,
                 changing_dir->file[changing_dir->cursor]);

		remove(absolute_path); // Delete the file, see man 3 remove
        file_list(changing_dir);
    }
	return 0;
}
