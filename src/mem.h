#ifndef DIR_WORKING
#define DIR_WORKING

#include "dir.h"

struct working_dir *init_file_list();
void free_file_list(struct working_dir *list);
void *resize_ptr(void *ptr, size_t new_size);

#endif
