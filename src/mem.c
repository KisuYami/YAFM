#include <stdlib.h>
#include <unistd.h>
#include "dir.h"
#include "mem.h"
#include "clippboard.h"

void *resize_ptr(void *ptr, size_t new_size)
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

struct working_dir *init_file_list()
{
    struct working_dir *new_list;

    new_list = malloc(sizeof(struct working_dir));

    if(!new_list)
    {
        printf("YAFM: Failed to allocate memory\n");
        exit(1);
    }

    *new_list = (struct working_dir)
    {
        .cursor = 0,
        .num_files = 0,
        .path = malloc(sizeof(char) * PATH_MAX + 1),
        .screen = NULL,

    };

    new_list->clippboard = (struct clippboard)
    {
        .mem[0] = CLIPPBOARD_MEM_SIZE,
                  .mem[1] = 0,
                            .clip = malloc(sizeof(char) *
                                           CLIPPBOARD_MEM_SIZE * CLIPPBOARD_STEP_SIZE),
    };

    new_list->config = (struct config)
    {
        .hidden_files = 0, // Don't show hidden files
        .env[0] = getenv("IMAGE"),
                  .env[1] = getenv("VIDEO"),
                            .env[2] = getenv("READER"),
    };

    if(!new_list->file || !new_list->path || !new_list->clippboard.clip)
    {
        printf("YAFM: Failed to allocate memory\n");
        exit(1);
    }

    getcwd(new_list->path, PATH_MAX);

    return new_list;
}

void free_file_list(struct working_dir *list)
{
    free(list->clippboard.clip);
    free(list->path);
    free(list);
}
