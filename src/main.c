/*
  BSD 3-Clause License

  Copyright (c) 2019, Reberti Carvalho Soares
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  3. Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
  TODO: Directory Preview
*/
#include "../config.h"
#include "dir.h"
#include "display.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *VIDEO_PLAYER = "mpv";
const char *IMAGE_VIEWR = "sxiv";
const char *PDF_READER = "zathura";

int main() {

    int x, y;
    int curs = 0;
    int clipboard_number = 0;
    char key;
    WINDOW *left;
    struct working_dir main_dir;

    // Basic Setup
    screen_setup();
    getmaxyx(stdscr, y, x);
    left = newwin(x / 2, y, 0, 0);

    // Setup window(s)
    clear();
    wclear(left);
    file_list(&main_dir);
    display_path(&main_dir, left);
    display_files(&main_dir, left, curs);
    wrefresh(left);

    // Main loop
    while ((key = getchar()) != KEY_QUIT) {
        switch (key) {
        case KEY_MOV_UP:
            if (curs >= 1)
                curs--;
            break;

        case KEY_MOV_DOWN:
            if (curs <= main_dir.num_files - 2) // this count the "./" & "../"
                curs++;
            break;

        case KEY_MOV_LEFT:
            cd_back(&main_dir);
            break;

        case KEY_MOV_RIGHT:
            cd_enter(&main_dir, curs);
            break;

        case KEY_FILE_DEL:
            if (file_delete(&main_dir, left, curs) == -1)
                display_confirm(left, 2, main_dir.file[curs],
                                " could not be removed!");
            break;

        case KEY_FILE_KILL:
            file_kill_yank(&main_dir, 0, curs, clipboard_number);
            clipboard_number++;
            break;

        case KEY_FILE_YANK:
            file_kill_yank(&main_dir, 1, curs, clipboard_number);
            clipboard_number++;
            break;

        case KEY_FILE_PASTE:
            file_paste(&main_dir);
            clipboard_number = 0;
            break;

        default:
            break;
        }
        // If terminal size has changed, update the window size
        if (is_term_resized(y, x) == true) {

            delwin(left);
            getmaxyx(stdscr, y, x);
            left = newwin(x / 2, y, 0, 0);
        }

        if (curs >= main_dir.num_files || curs < 0)
            curs = 0;

        // Updating Screen
        wclear(left);

        display_path(&main_dir, left);
        display_files(&main_dir, left, curs);

        wrefresh(left);
    }

    // Cleaning Shit
    delwin(left);
    endwin();
    return 0;
}
