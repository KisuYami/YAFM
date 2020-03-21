#include <string.h>
#include <unistd.h>

#include "interaction.h"
#include "display.h"
#include "dir.h"
#include "../config.h"

void
handle_input(display_t *main_display, int *cursor, char key)
{
	switch(key)
	{
	case KEY_MOV_UP:
		if(*cursor > 0)
			*cursor -= 1;

		break;

	case KEY_MOV_DOWN:
		// *cursor+1: because i need the info about where
		// the *cursor will be, not where it is.
		if(*cursor+1 < main_display->files.size)
			*cursor += 1;

		break;

	case KEY_MOV_LEFT:
	{
		char *tmp = strrchr(config.path, '/');

		if(tmp[1] != config.path[1])
		{
			tmp[0] = '\0';
			chdir(config.path);
		}
		else
			chdir("/");


		list_files(main_display, NULL);

		*cursor = 0;

		DRAW_PATH();
		main_display_files(*main_display, *cursor);

		break;
	}
	case KEY_MOV_RIGHT:
		if(chdir(main_display->files.list[*cursor]) == 0)
		{
			list_files(main_display, NULL);

			*cursor = 0;

			DRAW_PATH();
			main_display_files(*main_display, *cursor);
		}

		break;

	case KEY_FILE_OPEN:
		file_open(main_display, *cursor);
		break;

	case KEY_FILE_MARK:
		main_display->files.marked[*cursor] =
			!main_display->files.marked[*cursor];

		DRAW_PATH();
		main_display_files(*main_display, *cursor);
		refresh();
		break;

	case KEY_FILE_HIDDEN:
		*cursor = 0;
		config.hidden = !config.hidden;

		list_files(main_display, NULL);
		main_display_files(*main_display, *cursor);
		break;

	case KEY_FILE_DEL:
		if(selection_del(main_display) == 0)
		{
			*cursor = 0;
			list_files(main_display, NULL);
			main_display_files(*main_display, *cursor);
		}

		DRAW_PATH();
		break;

	case KEY_FILE_YANK:
		selection_yank(main_display);
		break;

	case KEY_FILE_CUT:
		selection_cut(main_display);
		break;

	case KEY_FILE_PASTE:
		selection_paste();
		break;
	}
}

static void
selection_copy(display_t *dir_display)
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
selection_yank(display_t *dir_display)
{
	selection_copy(dir_display);
	file_selection.type = SEC_CLIP_YANK;
}

void
selection_cut(display_t *dir_display)
{
	selection_copy(dir_display);
	file_selection.type = SEC_CLIP_YANK;
}

int
selection_del(display_t *dir_display)
{
	char phrase_final[] = "Proceed with deletion of files? (y/n)";
	move(config.size.y - 1, 0); // move to begining of line
	clrtoeol();	  // Clean displayed path

	mvprintw(config.size.y - 1, 1, phrase_final);
	refresh();

	char key = getchar();

	if(key != 'y')
		return -1; // user answear wans't "Yes"

	selection_copy(dir_display);
	for(size_t i = 0; i < file_selection.size; ++i)
		remove(file_selection.files[i]);

	return 0;
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
