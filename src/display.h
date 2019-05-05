/** @file display.h
  @brief Display In Terminal

  Use the info from functions to display in terminal.
  */
/** @fn void displayFiles(char *list[255], int lenght, int cursor);
  @brief Display Files

  @param *list[] Get list with what files will be displayed.
  @param length Who many files there is in list.
  @param cursor Where the cursor is to display accented.

  @author Reberti Soares
  @date 05-05-2019
  */
/** @fn void displayDirPath(char *path);
  @brief Display Path

  @param *path Display Working path as the first line in terminal.

  @author Reberti Soares
  @date 05-05-2019
  */
void displayFiles(char *list[255], int lenght, int cursor);
void displayDirPath(char *path);
