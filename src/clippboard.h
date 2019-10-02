#ifndef CLIPPBOARD_HEADER
#define CLIPPBOARD_HEADER

#include "dir.h"

#define CLIPPBOARD_MEM_SIZE 5
#define CLIPPBOARD_STEP_SIZE 50

void copy_clipboard(struct working_dir *dir);
void move_clipboard(struct working_dir *dir);

void add_clipboard(struct working_dir *dir);
void rmv_clipboard(struct working_dir *dir);
void clr_clipboard(struct working_dir *dir);

#endif
