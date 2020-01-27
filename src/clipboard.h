#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <stddef.h>

#include "dir.h"

enum selection_type {
	SEC_CLIP_YANK = 1,
	SEC_CLIP_CUT = 2,
	SEC_FILE_DEL = 4,
};

struct selection {
	int type;
	size_t size;
	char files[100][1024]; // XXX
} file_selection;

void
selection_yank(struct dir_display *dir_display);

void
selection_cut(struct dir_display *dir_display);

void
selection_del(struct dir_display *dir_display);

void
selection_paste(void);

#endif /* CLIPBOARD_H */
