/*
  BSD 3-Clause License

  Copyright (c) 2019, Reberti Carvalho Soares
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  3. Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "dir.h"
#include "../config.h"
#include "display.h"
#include <dirent.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

char *get_env(char *env_name) {

	int i = 0;
    char *value;

	while(environ[i] != NULL) {

		if(strncmp(environ[i], env_name, 4) == 0) {
			value = strrchr(environ[i], '=');
			value++;
		}
		i++;
	}

    return value;
}

static int compare(const void *p1, const void *p2) {
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

const char *file_extension(const char *filename) {

    const char *dot = strrchr(filename, '.');

    if(!dot || dot == filename)
        return "";

    return dot + 1;
}

int file_list(struct working_dir *changing_dir) {
    int i = 0;
    DIR *d;
    struct dirent *dir;
    getcwd(changing_dir->path, PATH_MAX);

    d = opendir(changing_dir->path);

    while ((dir = readdir(d)) != NULL) {
        if (*dir->d_name != '.') { // Don't Show hidden files
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

int cd_enter(struct working_dir *changing_dir, int cursor) {

    if (cursor >= changing_dir->num_files)
        return 1;

    if (is_file(changing_dir->file[cursor])) {
        file_open(changing_dir, cursor);
    } else
        chdir(changing_dir->file[cursor]);

    file_list(changing_dir);
    return 0;
}

void cd_back(struct working_dir *changing_dir) {

    int i = strlen(changing_dir->path);

    while (i >= 0) {

        if (changing_dir->path[i] == '/') {
            changing_dir->path[i] = '\0';
            break;
        } else {
            changing_dir->path[i] = '\0';
            i--;
        }
    }

    if (!is_file(changing_dir->path))
        chdir(changing_dir->path);

    file_list(changing_dir);
}

void file_open(struct working_dir *changing_dir, int cursor) {

    pid_t child;
    char command[PATH_MAX + 23];

    // Env vars
    const char *doc_reader = get_env("READER");
    const char *video_viewer = get_env("VIDEO");
    const char *image_viewer = get_env("IMAGE");

    // Get the extension
    const char *extension = file_extension(changing_dir->file[cursor]);


    // TODO: found a better way
    if ((strncmp(extension, "mkv", 3) == 0) ||
        (strncmp(extension, "mp4", 3) == 0)) {
        snprintf(command, PATH_MAX + 25, "%s \"%s/%s\" &>/dev/null", video_viewer,
                 changing_dir->path, changing_dir->file[cursor]);
    }

    if (strncmp(extension, "pdf", 3) == 0) {
        snprintf(command, PATH_MAX + 25, "%s \"%s/%s\" &>/dev/null", doc_reader,
                 changing_dir->path, changing_dir->file[cursor]);
    }

    if ((strncmp(extension, "jpg", 3) == 0) ||
        (strncmp(extension, "png", 3) == 0) ||
        (strncmp(extension, "jpeg", 4) == 0)) {
        snprintf(command, PATH_MAX + 25, "%s \"%s/%s\" &>/dev/null", image_viewer,
                 changing_dir->path, changing_dir->file[cursor]);
    }

    child = fork();
    if (child == 0) {
        if (system(command) == 0)
            exit(0);
        else
            exit(1);
    } else if (child >= 1)
        return;
}

void file_kill_yank(struct working_dir *changing_dir, int type, int cursor,
                    int clipboard_number) { // type 0 = kill, 1 = yank

    if (clipboard_number > CLIPBOARD_MAX)
        return;

    changing_dir->working_clipboard[clipboard_number].type = type;
    snprintf(changing_dir->working_clipboard[clipboard_number].path, PATH_MAX + 3,
             "%s/%s", changing_dir->path, changing_dir->file[cursor]);
}

void file_paste(struct working_dir *changing_dir) {

    int i;
    char tmp_command[PATH_MAX * 2 + 25];

    for (i = 0; i < CLIPBOARD_MAX; i++) {

        if (changing_dir->working_clipboard[i].type == 0) {
            snprintf(tmp_command, PATH_MAX * 2 + 25, "mv \"%s\" \"%s\" &>/dev/null",
                     changing_dir->working_clipboard[i].path, changing_dir->path);
            changing_dir->working_clipboard[i].type = -1; // No duplicates
            system(tmp_command);
        }
        if (changing_dir->working_clipboard[i].type == 1) {
            snprintf(tmp_command, PATH_MAX * 2 + 25,
                     "cp -r \"%s\" \"%s\" &>/dev/null",
                     changing_dir->working_clipboard[i].path, changing_dir->path);

            changing_dir->working_clipboard[i].type = -1; // No duplicates
            system(tmp_command);
        }
    }
    file_list(changing_dir);
}

int file_delete(struct working_dir *changing_dir, WINDOW *screen, int cursor) {

    int return_value = 0;
    char absolute_path[PATH_MAX];

    if (display_confirm(screen, 3, "Proceed with deletion of ",
                        changing_dir->file[cursor], "?") == 0) {
        snprintf(absolute_path, PATH_MAX + 2, "%s/%s", changing_dir->path,
                 changing_dir->file[cursor]);
        return_value = remove(absolute_path); // Delete the file, see man 3 remove

        file_list(changing_dir);
    }
    return return_value;
}
