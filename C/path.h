/*
Bailey Defino 2018
<https://hiten2.github.io>

CLARIFICATIONS:
  1. "Authorship Information" regards the following:
    "Hiten," "Hiten2," the year,
    these clarifications, the disclaimer,
    "<https://hiten2.github.io>," and this license.
  2. "Illegal Activity" is relative to the end user.

DISCLAIMER:
  THIS CODE IS PROVIDED WITH NO WARRANTY NOR ANY GAURANTEES.
  THE ORIGINAL AUTHOR IS NOT RESPONSIBLE FOR ANY DAMAGE
  OR ILLEGAL ACTIVITY CAUSED BY THE USE OF THIS VERBATIM
  OR MODIFIED CODE. BY VIEWING, STORING, OR EXECUTING THIS CODE
  THE END USER ACKNOWLEDGES THESE IMPLICATIONS AND ASSUMES
  ALL RESPONSIBILITY.

LICENSE:
  This verbatim code is intellectual property and may not
  be redistributed. This code may be modified in any way
  under either of these two conditions: (1) modified
  versions are not distributed or (2) modified versions
  satisfy the distribution conditions. Distribution
  conditions are as follows and must all be met:
  (1) the distributed code is not a verbatim copy,
  (2) the distributed code retains the authorship information,
  (3) the distributed code clarifies that the original author
  had no part in the modifications.
*/
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif

/* Pythonic path manipulations *//* deprecate PATH_MAX */

#ifndef PATH_H
#define PATH_H

#undef PATH_SEP
#define PATH_SEP '/'

#ifdef _WIN32
#undef getcwd
#define getcwd(b, s) _getcwd(b, s)

#undef PATH_SEP
#define PATH_SEP '\\'
#endif

/* place an absolute path into (optional) dest */
char *abspath(char *dest, const char *src);

/* return a pointer to the base name */
char *basename(const char *src);

/* place directory (if available) in (optional) dest */
char *dirname(char *dest, const char *src);

/* return a pointer to the extension */
char *extension(const char *src);

/* return whether the path is absolute */
unsigned int isabspath(const char *path);

/* join a and b into (optional) c */
char *joinpaths(char *c, const char *a, const char *b);

/* normalize a path into (optional) dest */
char *normpath(char *dest, const char *src);

#endif
