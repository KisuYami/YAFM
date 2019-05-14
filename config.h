#ifndef HEADER_FILE
#define HEADER_FILE

#define CD_ENTER_BUFFER_SIZE 1024
#define OPEN_FILE_BUFFER_SIZE 1024
#define LIST_LENGHT 255
#define COLORED_FILES 1

#define MOV_LEFT 'h'
#define MOV_RIGHT 'l'
#define MOV_DOWN 'j'
#define MOV_UP 'k'
#define MOV_BOTTOM 'L'
#define MOV_MIDDLE 'M'
#define MOV_TOP 'H'
#define SHOW_HIDDEN 'z'
#define HIDE_HIDDEN 'Z'
#define BULK_RENAME 'b'
#define OPEN_EDITOR 'e'
#define FILE_DELETE 'D' // Remember No warranty!!!

static const char *EDITOR = "nvim";

#define TYPES 9
static const struct files_extensions {
	const char *program;
	const char *extension;
} files_extensions[] = {
	{ "mpv", ".mp4" },
	{ "mpv", ".avi" },
	{ "mpv", ".mkv" },
	{ "sxiv", ".jpg" },
	{ "sxiv", ".png" },
	{ "sxiv", ".jpeg" },
	{ "zathura", ".pdf" },
	{ "zathura", ".zip" } // Baka (>_<)
};

#endif
