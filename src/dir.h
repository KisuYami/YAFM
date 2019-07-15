#ifndef DIR_HEADER
#define DIR_HEADER

#define PATH_MAX 4096 + 1 /* chars in a path name including nul(linux) */
#define NAME_MAX 256 + 1  /* chars in a path name including nul(linux) */

struct working_dir {
  int nm_files;         // A count for the files in the working folder
  char *file[NAME_MAX]; // This should be used with malloc
  char path[PATH_MAX];
};

int list_files(struct working_dir *new_dir);
void cd_enter(struct working_dir *new_dir, int cursor);
void cd_back(struct working_dir *new_dir);

#endif
