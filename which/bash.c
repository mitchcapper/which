/*
 * Copyright (C) 1987 - 2002 Free Software Foundation, Inc.
 *
 * This file is based on stuff from GNU Bash 1.14.7, the Bourne Again SHell.
 * Everything that was changed is marked with the word `CHANGED'.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02110-1301, USA.
 */

#include "sys.h"
#include <sys/stat.h>
#include "bash.h"

/* Use the type that was determined by configure. */
#define GID_T GETGROUPS_T

/*
 * CHANGED:
 * Perhaps these need new configure.in entries.
 * The following macro's are used in bash, and below:
 */
#define HAVE_GETGROUPS
#undef SHELL
#undef AFS
#undef NOGROUP

/*
 * CHANGED:
 * - Added prototypes,
 * - used ANSI function arguments,
 * - made all functions static and
 * - changed all occurences of 'char *' into 'const char *' where possible.
 * - changed all occurences of 'gid_t' into 'GID_T'.
 * - exported functions needed in which.c
 */
static int group_member (GID_T gid);
static char* extract_colon_unit (char const* string, int *p_index);

/*===========================================================================
 *
 * Everything below is from bash-2.0.5b.
 *
 */

/* From bash-2.05b / shell.h / line 104 */
/* Information about the current user. */
struct user_info {
  uid_t uid, euid;
  GID_T gid, egid;
  char *user_name;
  char *shell;          /* shell from the password file */
  char *home_dir;
};

/* From bash-2.05b / shell.c / line 108 */
/* Information about the current user. */
struct user_info current_user =
{
  (uid_t)-1, (uid_t)-1, (GID_T)-1, (GID_T)-1,
  (char *)NULL, (char *)NULL, (char *)NULL
};

/* From bash-2.05b / general.h / line 152 */
#define FREE(s)  do { if (s) free (s); } while (0)

/* From bash-2.05b / shell.c / line 1097 */
/* Fetch the current set of uids and gids and return 1 if we're running
   setuid or setgid. */
int
uidget ()
{
  uid_t u;

  u = getuid ();
  if (current_user.uid != u)
    {
      FREE (current_user.user_name);
      FREE (current_user.shell);
      FREE (current_user.home_dir);
      current_user.user_name = current_user.shell = current_user.home_dir = (char *)NULL;
    }
  current_user.uid = u;
  current_user.gid = getgid ();
  current_user.euid = geteuid ();
  current_user.egid = getegid ();

  /* See whether or not we are running setuid or setgid. */
  return (current_user.uid != current_user.euid) ||
           (current_user.gid != current_user.egid);
}

/* From bash-2.05b / general.c / line 744 */
static int ngroups, maxgroups;

/* From bash-2.05b / general.c / line 746 */
/* The set of groups that this user is a member of. */
static GETGROUPS_T *group_array = (GETGROUPS_T *)NULL;

/* From bash-2.05b / general.c / line 749 */
#if !defined (NOGROUP)
#  define NOGROUP (GID_T) -1
#endif

/* From bash-2.05b / lib/sh/oslib.c / line 245 */
#define DEFAULT_MAXGROUPS 64

/* From bash-2.05b / lib/sh/oslib.c / line 247 */
int
getmaxgroups ()
{
  static int maxgroups = -1;

  if (maxgroups > 0)
    return maxgroups;

#if defined (HAVE_SYSCONF) && defined (_SC_NGROUPS_MAX)
  maxgroups = sysconf (_SC_NGROUPS_MAX);
#else
#  if defined (NGROUPS_MAX)
  maxgroups = NGROUPS_MAX;
#  else /* !NGROUPS_MAX */
#    if defined (NGROUPS)
  maxgroups = NGROUPS;
#    else /* !NGROUPS */
  maxgroups = DEFAULT_MAXGROUPS;
#    endif /* !NGROUPS */
#  endif /* !NGROUPS_MAX */
#endif /* !HAVE_SYSCONF || !SC_NGROUPS_MAX */

  if (maxgroups <= 0)
    maxgroups = DEFAULT_MAXGROUPS;

  return maxgroups;
}

/* From bash-2.05b / general.c / line 753 */
static void
initialize_group_array ()
{
  register int i;

  if (maxgroups == 0)
    maxgroups = getmaxgroups ();

  ngroups = 0;
  group_array = (GETGROUPS_T *)xrealloc (group_array, maxgroups * sizeof (GETGROUPS_T));

#if defined (HAVE_GETGROUPS)
  ngroups = getgroups (maxgroups, group_array);
#endif

  /* If getgroups returns nothing, or the OS does not support getgroups(),
     make sure the groups array includes at least the current gid. */
  if (ngroups == 0)
    {
      group_array[0] = current_user.gid;
      ngroups = 1;
    }

  /* If the primary group is not in the groups array, add it as group_array[0]
     and shuffle everything else up 1, if there's room. */
  for (i = 0; i < ngroups; i++)
    if (current_user.gid == (GID_T)group_array[i])
      break;
  if (i == ngroups && ngroups < maxgroups)
    {
      for (i = ngroups; i > 0; i--)
        group_array[i] = group_array[i - 1];
      group_array[0] = current_user.gid;
      ngroups++;
    }

  /* If the primary group is not group_array[0], swap group_array[0] and
     whatever the current group is.  The vast majority of systems should
     not need this; a notable exception is Linux. */
  if (group_array[0] != current_user.gid)
    {
      for (i = 0; i < ngroups; i++)
        if (group_array[i] == current_user.gid)
          break;
      if (i < ngroups)
        {
          group_array[i] = group_array[0];
          group_array[0] = current_user.gid;
        }
    }
}

/* From bash-2.05b / general.c / line 805 */
/* Return non-zero if GID is one that we have in our groups list. */
int
group_member (GID_T gid)
{
#if defined (HAVE_GETGROUPS)
  register int i;
#endif

  /* Short-circuit if possible, maybe saving a call to getgroups(). */
  if (gid == current_user.gid || gid == current_user.egid)
    return (1);

#if defined (HAVE_GETGROUPS)
  if (ngroups == 0)
    initialize_group_array ();

  /* In case of error, the user loses. */
  if (ngroups <= 0)
    return (0);

  /* Search through the list looking for GID. */
  for (i = 0; i < ngroups; i++)
    if (gid == (GID_T)group_array[i])
      return (1);
#endif

  return (0);
}

/* From bash-2.05b / findcmd.c / line 75 */
#define u_mode_bits(x) (((x) & 0000700) >> 6)
#define g_mode_bits(x) (((x) & 0000070) >> 3)
#define o_mode_bits(x) (((x) & 0000007) >> 0)
#define X_BIT(x) ((x) & 1)

/* From bash-2.05b / findcmd.c / line 80 */
/* Return some flags based on information about this file.
   The EXISTS bit is non-zero if the file is found.
   The EXECABLE bit is non-zero the file is executble.
   Zero is returned if the file is not found. */
int
file_status (char const* name)
{
  struct stat finfo;

  /* Determine whether this file exists or not. */
  if (stat (name, &finfo) < 0)
    return (0);

  /* If the file is a directory, then it is not "executable" in the
     sense of the shell. */
  if (S_ISDIR (finfo.st_mode))
    return (FS_EXISTS|FS_DIRECTORY);

#if defined (AFS)
  /* We have to use access(2) to determine access because AFS does not
     support Unix file system semantics.  This may produce wrong
     answers for non-AFS files when ruid != euid.  I hate AFS. */
  if (access (name, X_OK) == 0)
    return (FS_EXISTS | FS_EXECABLE);
  else
    return (FS_EXISTS);
#else /* !AFS */

  /* Find out if the file is actually executable.  By definition, the
     only other criteria is that the file has an execute bit set that
     we can use. */

  /* Root only requires execute permission for any of owner, group or
     others to be able to exec a file. */
  if (current_user.euid == (uid_t)0)
    {
      int bits;

      bits = (u_mode_bits (finfo.st_mode) |
              g_mode_bits (finfo.st_mode) |
              o_mode_bits (finfo.st_mode));

      if (X_BIT (bits))
        return (FS_EXISTS | FS_EXECABLE);
    }

  /* GNU which comment: The part below was changed to ONLY return FS_EXECABLE
     when the file is *really* executable.  A bug report has been filed with
     the bash maintainers and was accepted, the behaviour of the next bash
     version (after 2.0.5b) should be the same again. */
  /* If we are the owner of the file, the owner execute bit applies. */
  if (current_user.euid == finfo.st_uid)
    return (X_BIT (u_mode_bits (finfo.st_mode))) ? (FS_EXISTS | FS_EXECABLE) : FS_EXISTS;

  /* Otherwise, if we are in the owning group, the group permissions apply. */
  if (group_member (finfo.st_gid))
    return (X_BIT (g_mode_bits (finfo.st_mode))) ? (FS_EXISTS | FS_EXECABLE) : FS_EXISTS;

  /* Otherwise, if we are in the other group, the other permissions apply. */
  if (X_BIT (o_mode_bits (finfo.st_mode)))
    return (FS_EXISTS | FS_EXECABLE);

  return (FS_EXISTS);
#endif /* !AFS */
}

/* From bash-2.05b / general.c / line 501 ; Changes: Using 'strchr' instead of 'xstrchr'. */
/* Return 1 if STRING is an absolute program name; it is absolute if it
   contains any slashes.  This is used to decide whether or not to look
   up through $PATH. */
int
absolute_program (char const* string)
{
  return ((char *)strchr (string, '/') != (char *)NULL);
}

/* From bash-2.05b / stringlib.c / line 124 */
/* Cons a new string from STRING starting at START and ending at END,
   not including END. */
char *
substring (char const* string, int start, int end)
{
  register int len;
  register char *result;

  len = end - start;
  result = (char *)xmalloc (len + 1);
  strncpy (result, string + start, len);
  result[len] = '\0';
  return (result);
}

/* From bash-2.05b / general.c / line 573 */
/* Given a string containing units of information separated by colons,
   return the next one pointed to by (P_INDEX), or NULL if there are no more.
   Advance (P_INDEX) to the character after the colon. */
char*
extract_colon_unit (char const* string, int* p_index)
{
  int i, start, len;
  char *value;

  if (string == 0)
    return NULL;

  len = strlen (string);
  if (*p_index >= len)
    return ((char *)NULL);

  i = *p_index;

  /* Each call to this routine leaves the index pointing at a colon if
     there is more to the path.  If I is > 0, then increment past the
     `:'.  If I is 0, then the path has a leading colon.  Trailing colons
     are handled OK by the `else' part of the if statement; an empty
     string is returned in that case. */
  if (i && string[i] == ':')
    i++;

  for (start = i; string[i] && string[i] != ':'; i++)
    ;

  *p_index = i;

  if (i == start)
    {
      if (string[i])
        (*p_index)++;
      /* Return "" in the case of a trailing `:'. */
      value = (char *)xmalloc (1);
      value[0] = '\0';
    }
  else
    value = substring (string, start, i);

  return (value);
}

/* From bash-2.05b / findcmd.c / line 242 */
/* Return the next element from PATH_LIST, a colon separated list of
   paths.  PATH_INDEX_POINTER is the address of an index into PATH_LIST;
   the index is modified by this function.
   Return the next element of PATH_LIST or NULL if there are no more. */
char*
get_next_path_element (char const* path_list, int* path_index_pointer)
{
  char* path;

  path = extract_colon_unit (path_list, path_index_pointer);

  if (path == 0)
    return (path);

  if (*path == '\0')
    {
      free (path);
      path = savestring (".");
    }

  return (path);
}

/* From bash-1.14.7 */
/* Turn PATH, a directory, and NAME, a filename, into a full pathname.
   This allocates new memory and returns it. */
char *
make_full_pathname (const char *path, const char *name, int name_len)
{
  char *full_path;
  int path_len;

  path_len = strlen (path);
  full_path = (char *) xmalloc (2 + path_len + name_len);
  strcpy (full_path, path);
  full_path[path_len] = '/';
  strcpy (full_path + path_len + 1, name);
  return (full_path);
}
