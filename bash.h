#define FS_EXISTS		0x1
#define FS_EXECABLE		0x2
#define FS_EXEC_PREFERRED	0x4
#define FS_EXEC_ONLY		0x8

extern const char *find_user_command_in_path (const char *name, const char *path_list, int flags);
