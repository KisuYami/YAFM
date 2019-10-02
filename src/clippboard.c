#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "dir.h"
#include "mem.h"
#include "clippboard.h"

void copy_clipboard(struct working_dir *dir)
{
	size_t i;
	char *string = NULL;

	string = malloc(sizeof(char) * 1024);

	for(i = 0; i < dir->clippboard.mem[1]; ++i) {
		sprintf("mv %s %s", dir->clippboard.clip[i], dir->path);
		system(string);
	}

	free(string);
}

void move_clipboard(struct working_dir *dir)
{
	return; // TODO
}

void add_clipboard(struct working_dir *dir)
{

	if(dir->clippboard.mem[0] <= dir->clippboard.mem[1]) {

		dir->clippboard.mem[0] += CLIPPBOARD_MEM_SIZE;

		dir->clippboard.clip = resize_ptr(dir->clippboard.clip,
				dir->clippboard.mem[0] *
				CLIPPBOARD_STEP_SIZE * sizeof(char));
	}

#if 0
	strcpy(dir->clippboard.clip[dir->clippboard.mem[1]],
			dir->file[dir->cursor]);
#else
	dir->clippboard.clip[dir->clippboard.mem[1]] = dir->file[dir->cursor];
#endif

	dir->clippboard.mem[1]++;
}

void rmv_clipboard(struct working_dir *dir)
{
	size_t i;
	for(i = 0; i < dir->clippboard.mem[1]; ++i) {
		if(strcmp(dir->clippboard.clip[i], dir->file[dir->cursor]) == 0) {
			memset(dir->clippboard.clip[i], 0,
					strlen(dir->clippboard.clip[i]));
		}

	}
}

void clr_clipboard(struct working_dir *dir)
{
	free(dir->clippboard.clip);

	dir->clippboard = (struct clippboard) {
		.mem[0] = CLIPPBOARD_MEM_SIZE,
		.mem[1] = 0,

		.clip = malloc(sizeof(char) *
				CLIPPBOARD_MEM_SIZE * CLIPPBOARD_STEP_SIZE),
	};

	if(!dir->clippboard.clip) {
		printf("YAFM: Failed to alocate memory\n");
	}
}
