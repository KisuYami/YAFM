/** @brief Current Dir Files

  This function get the file list of the current folder.
  Use a char *EXAMPLE[255] as argument, and give to the var the list.

  @author Reberti Soares
  @date 03-05-2019
  */
int openFile(const char path[]);
int is_regular_file(const char *path);
int listFiles(char *l[255], char *cwd);
char *cdBack(char path[]);
char *cdEnter(const char state[], const char text[]);
