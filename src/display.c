#include <ncurses.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../config.h"
#include "dir.h"

void screen_setup()
{
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

void display_path(dir_t *changing_dir)
{
    mvwprintw(changing_dir->screen, changing_dir->config.y - 1, 1, changing_dir->path);
}

void display_files(dir_t *changing_dir)
{
    int factor;
    static int old_factor;

    factor = (changing_dir->cursor >= changing_dir->config.y - 2) ?
        changing_dir->cursor - changing_dir->config.y + 3 : 0;

    if(changing_dir->cursor == 0)
        old_factor = 0;

    if(old_factor > factor && changing_dir->cursor > old_factor)
        factor = old_factor;

    else if(old_factor > factor)
    {
        factor = old_factor;
        factor--;
    }

    old_factor = factor;

    for (int i = factor; i < changing_dir->num_files; i++)
    {
        if(i - factor == changing_dir->config.y - 1) // only draw in the space of the window
            break;

        if(i == changing_dir->cursor)
        {
            wattron(changing_dir->screen, A_STANDOUT | COLOR_PAIR(1));
            mvwprintw(changing_dir->screen, i - factor, 3, "%s", changing_dir->file[i]);
            wattroff(changing_dir->screen, A_STANDOUT | COLOR_PAIR(1));
        }
        else if(is_file(changing_dir->file[i]))
        {
            wattron(changing_dir->screen, A_BOLD | COLOR_PAIR(3));
            mvwprintw(changing_dir->screen, i - factor, 3, "%s", changing_dir->file[i]);
            wattroff(changing_dir->screen, A_BOLD | COLOR_PAIR(3));
        }
        else
            mvwprintw(changing_dir->screen, i - factor, 3, "%s", changing_dir->file[i]);
    }
}

int
display_confirm(WINDOW *screen, int phrase_argc, char *phrase_input, ...)
{
  char phrase_final[1024];
  va_list phrase_list;

  strcpy(phrase_final, phrase_input);
  va_start(phrase_list, phrase_input);
  for(int x = 0; x < phrase_argc; x++) {
    char *tmp = va_arg(phrase_list, char *);
    strcat(phrase_final, tmp);
  }
  va_end(phrase_list);

  strcat(phrase_final, " (y/n)");
  int y = getmaxy(screen);
  wmove(screen, y - 1, 0); // move to begining of line
  wclrtoeol(screen);       // Clean displayed path

  mvwprintw(screen, y - 1, 1, phrase_final);
  wrefresh(screen);

  char key = getchar();

  if (key == 'y')
    return 0; // Yes

  if (key == 'n')
    return 1; // no

  return -1;
}
