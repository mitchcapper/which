/*
 * which v2.0 -- print full path of executables
 * Copyright (C) 1999  Carlo Wood <carlo@runaway.xs4all.nl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <libiberty.h>
#include <sys/stat.h>
#include <readline/tilde.h>
#include "bash.h"

static void print_usage(const char *progname)
{
  fprintf(stderr, "Usage: %s [options] [--] programname [...]\n", progname);
  fprintf(stderr, "Options: --version    Print version and exit successfully.\n");
  fprintf(stderr, "         --skip-dot   Skip directories in PATH that start with a dot.\n");
  fprintf(stderr, "         --skip-tilde Skip directories in PATH that start with a tilde.\n");
  fprintf(stderr, "         --show-dot   Don't expand a dot to current directory in output.\n");
  fprintf(stderr, "         --show-tilde Output a tilde for HOME directory for non-root.\n");
  fprintf(stderr, "         --tty-only   Stop processing options on the right if not on tty.\n");
}

static void print_version(void)
{
  fprintf(stdout, "GNU which v2.0 -- by Carlo Wood\n");
}

static void print_fail(const char *progname, const char *name, const char *path_list)
{
  fprintf(stderr, "%s: no %s in (%s)\n", progname, name, path_list);
}

enum opts {
  opt_version,
  opt_skip_dot,
  opt_skip_tilde,
  opt_show_dot,
  opt_show_tilde,
  opt_tty_only
};

int skip_dot = 0, skip_tilde = 0;

/* This does the dirty work for find_path_file () and find_user_command ().
   NAME is the name of the file to search for.
   PATH_LIST is a colon separated list of directories to search.
   FLAGS contains bit fields which control the files which are eligible.
   Some values are:
   FS_EXEC_ONLY:             The file must be an executable to be found.
   FS_EXEC_PREFERRED:        If we can't find an executable, then the
   the first file matching NAME will do.
   FS_EXISTS:                The first file found will do.
 */
const char *find_user_command_in_path(const char *name, const char *path_list, int flags)
{
  char *full_path;
  char *file_to_lose_on;
  int status, path_index, name_len;
  struct stat finfo;

  name_len = strlen(name);

  /* The file name which we would try to execute, except that it isn't
     possible to execute it.  This is the first file that matches the
     name that we are looking for while we are searching $PATH for a
     suitable one to execute.  If we cannot find a suitable executable
     file, then we use this one. */
  file_to_lose_on = (char *)NULL;

  if (absolute_program(name))
  {
    full_path = (char *)xmalloc(1 + name_len);
    strcpy(full_path, name);

    status = file_status(full_path);

    /* If the file doesn't exist, quit now. */
    if (!(status & FS_EXISTS))
    {
      free(full_path);
      return ((char *)NULL);
    }

    /* If we only care about whether the file exists or not, return
       this filename. */
    if (flags & FS_EXISTS)
      return (full_path);

    /* Otherwise, maybe we care about whether this file is executable.
       If it is, and that is what we want, return it. */
    if ((flags & FS_EXEC_ONLY) && (status & FS_EXECABLE))
      return (full_path);
    else
    {
      free(full_path);
      return ((char *)NULL);
    }
  }

  /* Find out the location of the current working directory. */
  stat(".", &finfo);

  path_index = 0;
  while (path_list && path_list[path_index])
  {
    /* Allow the user to interrupt out of a lengthy path search. */
    /* CHANGED: commented out: QUIT; */

    char *path = get_next_path_element(path_list, &path_index);

    if (!path)
      break;

    /* CHANGED: Skip everything in the PATH that doesn't start with a '/' */
    if (skip_tilde && *path != '/')
    {
      free(path);
      continue;
    }

    if (*path == '~')
    {
      char *t = tilde_expand(path);

      free(path);
      path = t;
    }

    /* CHANGED: Skip everything in the PATH that doesn't start with a '/' or '~' */
    if (skip_dot && *path != '/')
    {
      free(path);
      continue;
    }

    full_path = make_full_pathname(path, name, name_len);
    free(path);

    status = file_status(full_path);

    if (!(status & FS_EXISTS))
      goto next_file;

    /* The file exists.  If the caller simply wants the first file,
       here it is. */
    if (flags & FS_EXISTS)
      return (full_path);

    /* If the file is executable, then it satisfies the cases of
       EXEC_ONLY and EXEC_PREFERRED.  Return this file unconditionally. */
    if (status & FS_EXECABLE)
    {
      if (file_to_lose_on)
	free(file_to_lose_on);

      return (full_path);
    }

    /* The file is not executable, but it does exist.  If we prefer
       an executable, then remember this one if it is the first one
       we have found. */
    if (flags & FS_EXEC_PREFERRED)
    {
      if (!file_to_lose_on)
	file_to_lose_on = savestring(full_path);
    }

  next_file:
    free(full_path);
  }

  /* We didn't find exactly what the user was looking for.  Return
     the contents of FILE_TO_LOSE_ON which is NULL when the search
     required an executable, or non-NULL if a file was found and the
     search would accept a non-executable as a last resort. */
  return (file_to_lose_on);
}

int main(int argc, char *argv[])
{
  const char *progname = argv[0];
  const char *path_list = getenv("PATH");
  char cwd[256], home[256];
  size_t homelen = 0;
  int option, fail_count = 0;
  int show_dot = 0, show_tilde = 0, tty_only = 0;
  struct option longopts[] = {
    {"version", 0, &option, opt_version},
    {"skip-dot", 0, &option, opt_skip_dot},
    {"skip-tilde", 0, &option, opt_skip_tilde},
    {"show-dot", 0, &option, opt_show_dot},
    {"show-tilde", 0, &option, opt_show_tilde},
    {"tty-only", 0, &option, opt_tty_only},
    {NULL, 0, NULL, 0}
  };

  while (!getopt_long(argc, argv, "", longopts, NULL))
  {
    switch (option)
    {
      case opt_version:
	print_version();
	return 0;
      case opt_skip_dot:
	skip_dot = !tty_only;
	break;
      case opt_skip_tilde:
	skip_tilde = !tty_only;
	break;
      case opt_show_dot:
	show_dot = !tty_only;
	break;
      case opt_show_tilde:
	show_tilde = (!tty_only && geteuid() != 0);
	break;
      case opt_tty_only:
	tty_only = !isatty(1);
	break;
    }
  }

  if (!show_dot)
  {
    if (!getcwd(cwd, sizeof(cwd)))
    {
      const char *pwd = getenv("PWD");

      if (!pwd || strlen(pwd) >= sizeof(cwd))
	strcpy(cwd, ".");
      else
	strcpy(cwd, pwd);
    }
  }

  if (show_tilde)
  {
    const char *h;

    if (!(h = getenv("HOME")))
    {
      fprintf(stderr, "%s: --show-tilde: Environment variable HOME not set\n", progname);
      show_tilde = 0;
    }
    else
    {
      strncpy(home, h, sizeof(home));
      home[sizeof(home) - 1] = 0;
      homelen = strlen(home);
      if (home[homelen - 1] != '/' && homelen < sizeof(home) - 1)
      {
	strcat(home, "/");
	++homelen;
      }
    }
  }

  argv += optind;

  if (!*argv)
  {
    print_usage(progname);
    return -1;
  }

  for (; *argv; ++argv)
  {
    const char *result = NULL;

    if (path_list && *path_list != '\0')
    {
      result = find_user_command_in_path(*argv, path_list, FS_EXEC_ONLY);
      if (result)
      {
	if (!show_dot && *result == '.')
	{
	  ++result;
	  fprintf(stdout, "%s", cwd);
	}
	else if (show_tilde && *result == '/' && !strncmp(result, home, homelen))
	{
	  result += homelen;
	  fprintf(stdout, "~/");
	}
	fprintf(stdout, "%s\n", result);
      }
    }
    if (!result)
    {
      print_fail(progname, *argv, path_list);
      ++fail_count;
    }
  }

  return fail_count;
}
