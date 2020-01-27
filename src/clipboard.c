#include <string.h>
#include <stddef.h>
#include <unistd.h>

#include "dir.h"
#include "clipboard.h"

static void
selection_copy(struct dir_display *dir_display)
{
	file_selection.size = 0;

	for(size_t i = 0; i < dir_display->files.size; ++i)
	{
		if(dir_display->files.marked[i] == 1)
		{
			strcpy(file_selection.files[file_selection.size],
			       config.path);
			strcat(file_selection.files[file_selection.size], "/");
			strcat(file_selection.files[file_selection.size++],
			       dir_display->files.list[i]);
		}
	}
}

void
selection_yank(struct dir_display *dir_display)
{
	selection_copy(dir_display);
	file_selection.type = SEC_CLIP_YANK;
}

void
selection_cut(struct dir_display *dir_display)
{
	selection_copy(dir_display);
	file_selection.type = SEC_CLIP_YANK;
}

void
selection_del(struct dir_display *dir_display)
{
	selection_copy(dir_display);
	for(size_t i = 0; i < file_selection.size; ++i)
	{
		if(fork() == 0)
			execlp("rm", "rm", "-rf",
			       file_selection.files[i],
			       (char *)NULL);
	}
}

void
selection_paste(void)
{
	for(size_t i = 0; i < file_selection.size; ++i)
	{
		if(fork() == 0)
		{

			if(file_selection.type & SEC_CLIP_YANK)
				execlp("cp", "cp", "-r",
				       file_selection.files[i],
				       config.path, (char *)NULL);
			else
				execlp("mv", "mv", file_selection.files[i],
				       config.path, (char *)NULL);
		}
	}
}
