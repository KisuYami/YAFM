/** @file dir.h

void previewDir(char *path, char *text) {

	int Y, previewLenght, start;
	size_t size_result = strlen(state) + strlen(text) + CD_ENTER_BUFFER_SIZE;
	char *previewList[LIST_LENGHT];
	char *previewPwd = malloc(size_result);

	snprintf(previewPwd, size_result, "%s/%s", state, text);

	Y = getmaxy(stdscr);
	previewLenght = listFiles(previewList, previewPwd);

	start = (Y / 2) + 3;

	if(isFile(previewPw) != 1)
		displayFiles(previewList, previewLenght, -1, start);
	else
		return;
}
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
int isFile(char *path); // I see it
int listFiles(char *l[], char *cwd);
void cdEnter(char *path, const char workingFile[]);
void cdBack(char *path);
void shellActions(char path[], char file[], const char shellCommand[], char special[]);
void bulkRename(char path[]);
void previewDir(char *path, char *text);
