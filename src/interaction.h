#ifndef INTERACTION_H
#define INTERACTION_H

#include <stddef.h>

#include "dir.h"
#include "./display.h"

enum selection_type {
	SEC_CLIP_YANK = 1,
	SEC_CLIP_CUT = 2,
	SEC_FILE_DEL = 4,
};

struct selection {
	int type;
	size_t size;
	char files[100][MAXNAMLEN];
} file_selection;

void
handle_input(display_t *main_display, int *cursor, char key);

void
selection_yank(display_t *dir_display);

void
selection_cut(display_t *dir_display);

int
selection_del(display_t *dir_display);

void
selection_paste(void);


#endif /* INTERACTION_H */
