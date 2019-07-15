#include "../config.h"
#include "dir.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int compare(const void *p1, const void *p2) {
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

int list_files(struct working_dir *new_dir) {
  int i = 0;
  DIR *d;
  struct dirent *dir;
  getcwd(new_dir->path, PATH_MAX);

  d = opendir(new_dir->path);

  while ((dir = readdir(d)) != NULL) {
    if (*dir->d_name != '.') { // Don't Show hidden files
      new_dir->file[i] = dir->d_name;
      i++;
    }
  }

  closedir(d);
  new_dir->nm_files = i;
  qsort(&new_dir->file[0], i, sizeof(char *), compare);

  return i;
}

int is_file(char *path) {
  struct stat path_to_file;
  stat(path, &path_to_file);
  return S_ISREG(path_to_file.st_mode);
}

void cd_enter(struct working_dir *new_dir, int cursor) {

  char path[PATH_MAX];

  if (cursor >= new_dir->nm_files)
    return;

  snprintf(path, PATH_MAX + 2, "%s/%s", new_dir->path, new_dir->file[cursor]);
  strcpy(new_dir->path, path);

  chdir(new_dir->path);
  list_files(new_dir);
}

void cd_back(struct working_dir *new_dir) {

  int i = strlen(new_dir->path);

  while (i >= 0) {

    if (new_dir->path[i] == '/') {
      new_dir->path[i] = '\0';
      break;
    }

    else {
      new_dir->path[i] = '\0';
      i--;
    }
  }

  if (!is_file(new_dir->path)) // No need to make the string path to the old val
    chdir(new_dir->path);

  list_files(new_dir);
}

void file_open(/*TODO*/) {}
void file_move(/*TODO*/) {}
void file_copy(/*TODO*/) {}
void file_delete(/*TODO*/) {}
