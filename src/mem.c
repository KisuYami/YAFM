#include <stdlib.h>
#include <unistd.h>

#include "dir.h"
#include "mem.h"

void *
resize_ptr(void *ptr, size_t new_size)
{
    void *new_ptr = NULL;
    new_ptr = realloc(ptr, new_size);

    if(!new_ptr)
    {
        printf("YAFM: Failed to allocate memory\n");
        free(ptr);
        exit(1);
    }

    return new_ptr;
}

dir_t *
init_file_list()
{
    dir_t *new_list;

    new_list = malloc(sizeof(dir_t));

    if(!new_list)
    {
        printf("YAFM: Failed to allocate memory\n");
        exit(1);
    }

    *new_list = (dir_t) {
        .cursor = 0,
        .num_files = 0,
        .path = malloc(sizeof(char) * PATH_MAX + 1),
        .screen = NULL,
    };

    new_list->config = (struct config) {
        .hidden_files = 0, // Don't show hidden files
        .env[0] = getenv("IMAGE"),
        .env[1] = getenv("VIDEO"),
        .env[2] = getenv("READER"),
    };

    if(!new_list->file || !new_list->path)
    {
        printf("YAFM: Failed to allocate memory\n");
        exit(1);
    }

    getcwd(new_list->path, PATH_MAX);

    return new_list;
}

void free_file_list(dir_t *list)
{
    free(list->path);
    free(list);
}
