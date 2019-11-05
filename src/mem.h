#ifndef DIR_WORKING
#define DIR_WORKING

#include "dir.h"

dir_t *init_file_list();
void free_file_list(dir_t *list);
void *resize_ptr(void *ptr, size_t new_size);

#endif
