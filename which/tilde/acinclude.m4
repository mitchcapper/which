AC_DEFUN(BASH_CHECK_GETPW_FUNCS,
[AC_MSG_CHECKING(whether programs are able to redeclare getpw functions)
AC_CACHE_VAL(bash_cv_can_redecl_getpw,
[AC_TRY_COMPILE([#include <sys/types.h>
#include <pwd.h>
extern struct passwd *getpwent();
extern struct passwd *getpwuid();
extern struct passwd *getpwnam();],
[struct passwd *z; z = getpwent(); z = getpwuid(0); z = getpwnam("root");],
  bash_cv_can_redecl_getpw=yes,bash_cv_can_redecl_getpw=no)])
AC_MSG_RESULT($bash_cv_can_redecl_getpw)
if test $bash_cv_can_redecl_getpw = no; then
AC_DEFINE(HAVE_GETPW_DECLS)
fi
])
