#define FS_EXISTS		0x1
#define FS_EXECABLE		0x2
#define FS_EXEC_PREFERRED	0x4
#define FS_EXEC_ONLY		0x8

#define savestring(x) (char *)strcpy (xmalloc (1 + strlen (x)), (x))

extern int file_status (const char *name);
extern int absolute_program (const char *string);
extern char *get_next_path_element (const char *path_list, int *path_index_pointer);
extern int same_file (const char *path1, const char *path2, const struct stat *stp1, const struct stat *stp2);
extern char *make_full_pathname (const char *path, const char *name, int name_len);
