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

#include "../config.h"
#include "dir.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void screen_setup() {

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  start_color();
  // Colors
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
}

void display_path(struct working_dir *changing_dir, WINDOW *screen) {
  int x;
  x = getmaxx(screen);
  mvwprintw(screen, x - 1, 1, changing_dir->path);
}

void display_files(struct working_dir *changing_dir, WINDOW *screen,
                   int cursor) {
  for (int i = 0; i < changing_dir->num_files; i++) {
    if (i == cursor) {
      wattron(screen, A_STANDOUT | COLOR_PAIR(1));
      mvwprintw(screen, i, 3, "%s", changing_dir->file[i]);
      wattroff(screen, A_STANDOUT | COLOR_PAIR(1));
    } else if (is_file(changing_dir->file[i])) {
      wattron(screen, A_BOLD | COLOR_PAIR(3));
      mvwprintw(screen, i, 3, "%s", changing_dir->file[i]);
      wattroff(screen, A_BOLD | COLOR_PAIR(3));
    } else
      mvwprintw(screen, i, 3, "%s", changing_dir->file[i]);
  }
}

int display_confirm(WINDOW *screen, char *phrase, char *object) {

  int x = getmaxx(screen);
  char key;

  wmove(screen, x - 1, 0); // move to begining of line
  wclrtoeol(screen);       // Clean displayed path

  mvwprintw(screen, x - 1, 1, phrase, object);
  wrefresh(screen);

  key = getchar();

  if (key == 'y')
    return 0; // Yes

  if (key == 'n')
    return 1; // no

  return -1;
}
