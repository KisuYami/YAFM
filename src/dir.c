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

static int compare(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

int file_list(struct working_dir *changing_dir)
{

    int i = 0;
    DIR *d = NULL;
    struct dirent *dir;

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

int is_file(char *path)
{
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

void cd_back(struct working_dir *changing_dir)
{

    for(unsigned int i = strlen(changing_dir->path); i >= 0; --i)
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

const char *file_extension(const char *filename)
{

    const char *dot = strrchr(filename, '.');

    if(!dot || dot == filename)
        return "";

    return dot + 1;
}

void file_open(struct working_dir *dir)
{
    char string[1024+25];

    pid_t child;

    // Get the extension
    const char *extension = file_extension(dir->file[dir->cursor]);

    if((strncmp(extension, "jpg", 3) == 0) ||
            (strncmp(extension, "png", 3) == 0) ||
            (strncmp(extension, "jpeg", 4) == 0))
    {
        if (dir->config.env[0] == NULL)
        {
            mvprintw(dir->config.x, dir->config.y, "YAFM: Cound't find ENV var");
            return;
        }
        sprintf(string, "%s \"%s/%s\" &>/dev/null",
                dir->config.env[0], dir->path, dir->file[dir->cursor]);
    }

    else if((strncmp(extension, "mkv", 3) == 0) ||
            (strncmp(extension, "mp4", 3) == 0))
    {
        if (dir->config.env[1] == NULL)
        {
            mvprintw(dir->config.x, dir->config.y, "YAFM: Cound't find ENV var");
            return;
        }

        sprintf(string, "%s \"%s/%s\" &>/dev/null",
                dir->config.env[1], dir->path, dir->file[dir->cursor]);

    }

    else if(strncmp(extension, "pdf", 3) == 0)
    {
        if (dir->config.env[2] == NULL)
        {
            mvprintw(dir->config.x, dir->config.y, "YAFM: Cound't find ENV var");
            return;
        }
        sprintf(string, "%s \"%s/%s\" &>/dev/null",
                dir->config.env[2], dir->path, dir->file[dir->cursor]);
    }

    else
        return; // Non listed file type found

    child = fork();

    if(child == 0)
    {
        int fd = open("/dev/null", O_WRONLY);
        dup2(fd, STDOUT_FILENO);
        close(fd);

        system(string);
        exit(0);
    }
    return;
}
