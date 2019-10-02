#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "../config.h"
#include "dir.h"

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

void display_path(struct working_dir *changing_dir)
{
  int x = getmaxx(changing_dir->screen);
  mvwprintw(changing_dir->screen, x - 1, 1, changing_dir->path);
}

void display_files(struct working_dir *changing_dir)
{
  for (int i = 0; i < changing_dir->num_files; i++) {
    if (i == changing_dir->cursor) {
      wattron(changing_dir->screen, A_STANDOUT | COLOR_PAIR(1));
      mvwprintw(changing_dir->screen, i, 3, "%s", changing_dir->file[i]);
      wattroff(changing_dir->screen, A_STANDOUT | COLOR_PAIR(1));
    } else if (is_file(changing_dir->file[i])) {
      wattron(changing_dir->screen, A_BOLD | COLOR_PAIR(3));
      mvwprintw(changing_dir->screen, i, 3, "%s", changing_dir->file[i]);
      wattroff(changing_dir->screen, A_BOLD | COLOR_PAIR(3));
    } else
      mvwprintw(changing_dir->screen, i, 3, "%s", changing_dir->file[i]);
  }
}

int display_confirm(WINDOW *screen, int phrase_argc, char *phrase_input, ...) {

  int x;
  char key;
  char phrase_final[255];
  char *tmp;
  va_list phrase_list;

  strcpy(phrase_final, phrase_input);
  va_start(phrase_list, phrase_input);
  for (x = 0; x < phrase_argc; x++) {
    tmp = va_arg(phrase_list, char *);
    strcat(phrase_final, tmp);
  }
  va_end(phrase_list);

  x = getmaxx(screen);
  wmove(screen, x - 1, 0); // move to begining of line
  wclrtoeol(screen);       // Clean displayed path

  mvwprintw(screen, x - 1, 1, phrase_final);
  wrefresh(screen);

  key = getchar();

  if (key == 'y')
    return 0; // Yes

  if (key == 'n')
    return 1; // no

  return -1;
}
