/** @file dir.h
  @brief Folder and File Operations

  This code make the core functions like enter and exiting folders, using
  editing files, and opening files with external source.
  */
/** @fn int openFile(char path[], const char text[])
  @brief Open file with defined programs

  This function get the path of file as argument, and test the
  extension against files_extensions, only files with extensions should
  be tested.

  @author Reberti Soares
  @date 05-05-2019
  */
/** @fn int isFile(const char *path);
  @brief Check the type of file

  @param *path Path where check is made.
  Check using POSIX compliant system call, if a file is a regular file.
  @author Frédéric Hamidi(frederic.hamidi@gmail.com)
  @date 05-05-2019
  */
/** @fn int listFiles(char *l[255], char *cwd);
  @brief Get files in path

  @param *l[] List where result well be put.
  @param *cwd Current Working Dir.

  @author Reberti Soares
  @date 05-05-2019
  */
/** @fn char *cdBack(char path[]);
  @brief Change Dir in ../

  @param path[] Path to go.
  @author Reberti Soares
  @date 05-05-2019
  */
/** @fn char *cdEnter(char state[], const char text[]);
  @brief Open dir or file

  This function enter in a dir, or open a file if the var text
  points to one. See openFile function
  @param state[] Working dir.
  @param text[] Path to go.

  @author Reberti Soares
  @date 05-05-2019
  */
/** @fn void shellActions(char path[], char file[], char shellCommand[], char special[]);
  @brief Use a editor for action

  @param path[] Where the file is.
  @param file[] The file itself(can be empty string).
  @param command[] Command executed in file(can be empty string).
  @param special[] Some statements for command executed in file(can be empty string).

  @author Reberti Soares
  @date 05-05-2019
  */
int openFile(char path[]);
int isFile(char *path);
int listFiles(char *l[255], char *cwd);
char *cdBack(char path[]);
char *cdEnter(char state[], const char text[]);
void shellActions(char path[], char file[], const char shellCommand[], char special[]);
