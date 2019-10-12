#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "../config.h"
#include "dir.h"
#include "mem.h"
#include "display.h"
#include "clippboard.h"

int
main(void)
{

    char key;
    struct working_dir *main_dir = NULL;

    // Basic Setup
    main_dir = init_file_list();

    screen_setup();
    getmaxyx(stdscr, main_dir->config.y, main_dir->config.x);
    main_dir->screen = stdscr;

    // Setup window
    clear();

    wclear(main_dir->screen);

    file_list(main_dir);
    display_files(main_dir);
    display_path(main_dir);

    wrefresh(main_dir->screen);

    // Main loop
    while ((key = getchar()) != KEY_QUIT)
    {
        switch (key)
        {
        case KEY_MOV_UP:
            if(main_dir->cursor >= 1)
                main_dir->cursor--;
            break;

        case KEY_MOV_DOWN:
            if(main_dir->cursor <= main_dir->num_files - 2)
                main_dir->cursor++;
            break;

        case KEY_MOV_LEFT:
            cd_back(main_dir);
            break;

        case KEY_MOV_RIGHT:
            cd_enter(main_dir);
            break;

        case KEY_ACT_OPEN:
            file_open(main_dir);
            break;

        case KEY_ACT_HIDDEN:
            main_dir->config.hidden_files = !main_dir->config.hidden_files;
            file_list(main_dir);
            break;

        case KEY_CLIP_CLR:
            clr_clipboard(main_dir);
            break;

        case KEY_CLIP_ADD:
            add_clipboard(main_dir);
            break;

        case KEY_CLIP_RMV:
            rmv_clipboard(main_dir);
            break;

        case KEY_CLIP_PAS:
            copy_clipboard(main_dir);
            break;

        }
        // If terminal size has changed, update the window size
        if(is_term_resized(main_dir->config.y, main_dir->config.x) == true)
        {

            delwin(main_dir->screen);
            getmaxyx(stdscr, main_dir->config.y, main_dir->config.x);
            main_dir->screen = newwin(main_dir->config.x / 2,
                                      main_dir->config.y, 0, 0);
        }

        if(main_dir->cursor >= main_dir->num_files || main_dir->cursor < 0)
            main_dir->cursor = 0;

        // Updating Screen
        wclear(main_dir->screen);

        display_files(main_dir);
        display_path(main_dir);

        wrefresh(main_dir->screen);
    }

    // Cleaning

    free_file_list(main_dir);
    delwin(main_dir->screen);
    endwin();

    return 0;
}
