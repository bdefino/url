/*
Bailey Defino 2018
<https://bdefino.github.io>

CLARIFICATIONS:
  1. "Authorship Information" regards the following:
    "Hiten," "Hiten2," the year,
    these clarifications, the disclaimer,
    "<https://bdefino.github.io>," and this license.
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

#include "conv.h"
#include "path.h"

/* comprehensive URL operations */

#ifndef URL_H
#define URL_H

struct URL {
  char *domains;
  char *fragment;
  unsigned int ipv; /* IP version */
  char *parameters;
  char *password;
  char *path;
  unsigned int port;
  char *query;
  char *scheme;
  unsigned int scheme_is_protocol;
  char *username;
};

/* bind */
struct URL *url_bind(struct URL *dest, struct URL *a, struct URL *b);

/* deep copy */
struct URL *url_copy(struct URL *dest, struct URL *url);

/* deletion */
void url_del(struct URL *url, unsigned int free_attribs);

/* initialization */
struct URL *url_init(struct URL *dest, const char *string);

/* convert to string */
char *url_to_string(char *dest, struct URL *url, unsigned int resolve_path);

#endif
