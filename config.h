#ifndef HEADER_FILE
#define HEADER_FILE

#define MOV_LEFT 'h'
#define MOV_RIGHT 'l'
#define MOV_DOWN 'j'
#define MOV_UP 'k'
#define SHOW_HIDDEN 'z'
#define HIDE_HIDDEN 'Z'

static int hidden = 0;

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
	{ "zathura", ".zip" } // Baka (o_o)
};

#endif
