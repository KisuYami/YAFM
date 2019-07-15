#include "../config.h"
#include "dir.h"
#include "display.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  struct working_dir new_dir;
  list_files(&new_dir);

  printf("%s\n", new_dir.path);
  for (int i = 0; i < new_dir.nm_files; i++) {
    printf("%s\n", new_dir.file[i]);
  }

  cd_enter(&new_dir, 4);

  printf("\n\n%s\n", new_dir.path);
  for (int i = 0; i < new_dir.nm_files; i++) {
    printf("%s\n", new_dir.file[i]);
  }

  cd_back(&new_dir);

  printf("\n\n%s\n", new_dir.path);
  for (int i = 0; i < new_dir.nm_files; i++) {
    printf("%s\n", new_dir.file[i]);
  }
  return 0;
}
