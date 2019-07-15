#include "../config.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void setup_scr() {

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

void display_path(/*TODO*/) {}
void display_files(/*TODO*/) {}
void display_preview(/*TODO*/) {}
