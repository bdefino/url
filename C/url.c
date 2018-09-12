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

#include "url.h"

/* comprehensive URL operations */

/* bind */
struct URL *url_bind(struct URL *dest, struct URL *a, struct URL *b) {
  char *adirname;
  unsigned int i, malloced, pathlen;
  struct URL *src;
  
  malloced = dest == NULL;
  
  if ((a == NULL && b == NULL)
      || (dest == NULL && (dest = url_init(NULL, NULL)) == NULL))
    return NULL;
  src = b;
  
  if (b == NULL) /* use a */
    src = a;
  
  if (url_copy(dest, src) == NULL) {
    if (malloced)
      url_del(dest, 1);
    return NULL;
  }
  
  /* a + b -> dest */
  
  if (dest->scheme == NULL) {
    if (a->scheme != NULL) {
      if ((dest->scheme = (char *) malloc(strlen(a->scheme) + 1)) == NULL) {
        if (malloced)
          url_del(dest, 1);
        return NULL;
      }
      strcpy(dest->scheme, a->scheme);
    }
    dest->scheme_is_protocol = a->scheme_is_protocol | b->scheme_is_protocol;
    
    if (dest->domains == NULL) {
      if (a->domains != NULL) {
        if ((dest->domains = (char *) malloc(strlen(a->domains) + 1))
            == NULL) {
          if (malloced)
           url_del(dest, 1);
          return NULL;
        }
        strcpy(dest->domains, a->domains);
      }
      
      if (!dest->port)
        dest->port = a->port;
      
      if (dest->path == NULL || *dest->path != '/') {
        free(dest->path);
        
        adirname = (a->path != NULL ? dirname(NULL, a->path) : NULL);
        pathlen = (a->path != NULL ? strlen(adirname) : 0)
          + (b->path != NULL ? strlen(b->path) : 0)
          + (a->path != NULL && b->path != NULL
            && strlen(a->path) > 0 && strlen(b->path) > 0 ? 1 : 0);
        
        if ((dest->path = (char *) malloc(pathlen + 1)) == NULL) {
          if (malloced)
            url_del(dest, 1);
          return NULL;
        }
        
        if (adirname != NULL) {
          strcat(dest->path, adirname);
          
          if (b->path != NULL)
            strcat(dest->path, "/");
        }
        
        if (b->path != NULL)
          strcat(dest->path, b->path);
      }
    }
  }
  return dest;
}

/* deep copy */
struct URL *url_copy(struct URL *dest, struct URL *url) {
  char **dest_attribs[8], **url_attribs[8];
  unsigned int i, malloced;
  
  malloced = dest == NULL;
  
  if (url == NULL || (dest == NULL && (dest = url_init(NULL, NULL)) == NULL))
    return NULL;
  memcpy(dest_attribs,
    (char **[]) {&dest->domains, &dest->fragment, &dest->parameters,
      &dest->password, &dest->path, &dest->query, &dest->scheme,
      &dest->username},
    8 * sizeof(char **));
  
  dest->ipv = url->ipv;
  dest->port = url->port;
  dest->scheme_is_protocol = url->scheme_is_protocol;
  memcpy(url_attribs,
    (char **[]) {&url->domains, &url->fragment, &url->parameters,
      &url->password, &url->path, &url->query, &url->scheme, &url->username},
    8 * sizeof(char **));
  
  for (i = 0; i < 8; i++) {
    *dest_attribs[i] = NULL;
    
    if (*url_attribs[i] != NULL) {
      if ((*dest_attribs[i] = (char *) malloc(strlen(*url_attribs[i]) + 1))
          == NULL) {
        if (malloced)
          url_del(dest, 1);
        return NULL;
      }
      strcpy(*dest_attribs[i], *url_attribs[i]);
    }
  }
  return dest;
}
/* deletion */
void url_del(struct URL *url, unsigned int free_attribs) {
  if (url != NULL) {
    if (free_attribs) {
      free(url->domains);
      free(url->fragment);
      free(url->parameters);
      free(url->password);
      free(url->path);
      free(url->query);
      free(url->scheme);
      free(url->username);
    }
    free(url);
  }
}

/* initialization */
struct URL *url_init(struct URL *dest, const char *string) {
  char *delim, *outer_delim, **primary_attribs[3], primary_delims[3], *temp;
  unsigned int i, malloced, size, start, stop; /* size is reused */
  
  malloced = dest == NULL;
  
  if ((dest == NULL
      && (dest = (struct URL *) malloc(sizeof(struct URL))) == NULL))
    return NULL;
  dest->domains = dest->fragment = dest->parameters = dest->password
    = dest->path = dest->query = dest->scheme = dest->username = NULL;
  dest->ipv = 4;
  dest->port = dest->scheme_is_protocol = 0;
  
  if (string == NULL)
    return dest;
  
  /* outer_delim is for parsing nested expressions */
  
  start = 0;
  stop = strlen(string);
  
  if (!stop)
    return dest;
  
  /*
  [scheme:][//][user[:password]@][host[:port]]
  [/path][;parameters][?query][#fragment]
  */
  
  memcpy(primary_attribs,
    (char **[]) {&dest->fragment, &dest->query, &dest->parameters},
    3 * sizeof(char **));
  memcpy(primary_delims, "#?;", 3);
  
  for (i = 0; i < 3; i++) {
    if ((delim = (char *) memchr(string, primary_delims[i], stop)) != NULL) {
      size = string + stop - delim - 1;
      
      if ((*primary_attribs[i] = (char *) malloc(size + 1)) == NULL) {
        if (malloced)
          url_del(dest, 1);
        return NULL;
      }
      strncpy(*primary_attribs[i], delim + 1, size);
      *(*primary_attribs[i] + size) = '\0';
      stop -= size + 1;
    }
  }
  
  /* [scheme:][//][user[:password]@][host[:port]][/path] */
  
  if ((delim = (char *) memchr(string, ':', stop - start)) != NULL) {
    size = delim - string;
    
    if ((dest->scheme = (char *) malloc(size + 1)) == NULL) {
      if (malloced)
        url_del(dest, 1);
      return NULL;
    }
    strncpy(dest->scheme, string, size);
    *(dest->scheme + size) = '\0';
    start += size + 1;
  }
  
  /* [//][user[:password]@][host[:port]][/path] */
  
  if (*(string + start) == '/' && *(string + start + 1) == '/') {
    dest->scheme_is_protocol = 1;
    start += 2;
  }
  
  /* [user[:password]@][host[:port]][/path] */
  
  if ((outer_delim = (char *) memchr(string + start, '@', stop - start))
      != NULL) {
    size = outer_delim - string - start;
    
    if ((delim = memchr(string + start, ':', size)) != NULL) {
      size = delim - string - start;
      
      if ((dest->username = (char *) malloc(size + 1)) == NULL) {
        if (malloced)
          url_del(dest, 1);
        return NULL;
      }
      strncpy(dest->username, string + start, size);
      *(dest->username + size) = '\0';
      start += size + 1;
      size = outer_delim - delim - 1;
      
      if ((dest->password = (char *) malloc(size + 1)) == NULL) {
        if (malloced)
          url_del(dest, 1);
        return NULL;
      }
      strncpy(dest->password, delim + 1, size);
      *(dest->username + size) = '\0';
    } else if ((dest->username = (char *) malloc(size + 1)) == NULL) {
      if (malloced)
        url_del(dest, 1);
      return NULL;
    } else {
      strncpy(dest->username, string + start, size);
      *(dest->username + size) = '\0';
    }
    start += size + 1;
  }
  
  /* [host[:port]][/path] */
  
  if ((delim = memchr(string + start, '/', stop - start)) != NULL) {
    size = string + stop - delim;
    
    if ((dest->path = (char *) malloc(size + 1)) == NULL) {
      if (malloced)
        url_del(dest, 1);
      return NULL;
    }
    strncpy(dest->path, delim, size);
    *(dest->path + size) = '\0';
    stop = delim - string;
  }
  
  /* [host[:port]] */
  
  if (*(string + start) == '[' /* IPv6 */
      && (delim = memchr(string + start, ']', stop - start)) != NULL) {
    dest->ipv = 6;
    size = delim - string - start - 1;
    
    if ((dest->domains = (char *) malloc(size + 1)) == NULL) {
      if (malloced)
        url_del(dest, 1);
      return NULL;
    }
    strncpy(dest->domains, string + start + 1, size);
    *(dest->domains + size) = '\0';
    start += size + 2;
    
    if (*(string + start) == ':') {
      outer_delim = memchr(string + start, '/', stop - start);
      size = (outer_delim != NULL ? outer_delim - string : stop) - start - 1;
      
      char buffer[size];
      
      strncpy(buffer, string + start, size);
      *(buffer + size) = '\0';
      
      dest->port = atoi(buffer);
      start += size + 1;
    }
  } else if ((delim = memchr(string + start, '.', stop - start)) != NULL
      && ((outer_delim = memchr(string + start, ':', stop - start)) == NULL
        || delim < outer_delim)) { /* IPv4 */
    size = (outer_delim != NULL ? outer_delim - string : stop) - start;
    
    if ((dest->domains = (char *) malloc(size + 1)) == NULL) {
      if (malloced)
        url_del(dest, 1);
      return NULL;
    }
    strncpy(dest->domains, string + start, size);
    *(dest->domains + size) = '\0';
    start += size;
    
    if (*(string + start) == ':') {
      outer_delim = memchr(string + start, '/', stop - start);
      size = (outer_delim != NULL ? outer_delim - string: stop) - start - 1;
      
      char buffer[size];
      
      strncpy(buffer, string + start + 1, size);
      *(buffer + size) = '\0';
      
      dest->port = atoi(buffer);
      start += size + 1;
    }
  }
  
  if (start < stop) { /* we didn't previously get the entire path */
    temp = dest->path;
    size = stop - start + (temp != NULL ? strlen(temp) : 0);
    
    if ((dest->path = (char *) malloc(size + 1)) == NULL) {
      if (malloced)
        url_del(dest, 1);
      return NULL;
    }
    strncpy(dest->path, string + start, stop - start);
    
    if (temp != NULL) {
      strncpy(dest->path + stop - start, temp, size - stop + start);
      free(temp);
      temp = NULL;
    }
    *(dest->path + size) = '\0';
  }
  return dest;
}

/* convert to string */
char *url_to_string(char *dest, struct URL *url, unsigned int resolve_path) {
  unsigned int i, size; /* reused size */
  char port_arr[6], *primary_attribs[3], primary_delims[3];
  
  if (url == NULL)
    return NULL;
  memcpy(primary_attribs,
    (char *[]) {url->parameters, url->query, url->fragment},
    3 * sizeof(char *));
  memcpy(primary_delims, ";?#", 3);
  itoa(port_arr, url->port);
  
  /* determine size */
  
  size = (url->scheme != NULL ? strlen(url->scheme) : 0);
  
  size += (url->scheme_is_protocol ? 2 : 0);
  
  if (url->username != NULL)
    size += strlen(url->username)
      + (url->password != NULL ? strlen(url->password) : 0) + 1;
  
  if (url->domains != NULL)
    size += ((url->domains != NULL ? strlen(url->domains) : 0)
      + (url->ipv == 6 ? 2 : 0)
      + (url->port ? strlen(port_arr) + 1: 0)
      + (url->path != NULL && strlen(url->path) > 0 ? strlen(url->path) + 1
        : 0));
  else
    size += (url->path != NULL ? strlen(url->path): 0);
  size += (url->parameters != NULL ? strlen(url->parameters) + 1 : 0);
  size += (url->query != NULL ? strlen(url->query) + 1 : 0);
  size += (url->fragment != NULL ? strlen(url->fragment) + 1 : 0);
  
  if (dest == NULL && (dest = (char *) malloc(size + 1)) == NULL)
    return NULL;
  size = 0; /* current size written (start offset) */
  
  /*
  following code could be achieved more cleanly with strcat,
  however, it would also be inefficient,
  as each call to strcat would have to find the null terminator
  
  this approach is closer to O(n), and is therefore preferred
  */
  
  if (url->scheme != NULL) {
    strcpy(dest, url->scheme);
    size += strlen(url->scheme);
    *(dest + size) = ':';
    size++;
    
    if (url->scheme_is_protocol) {
      strcpy(dest + size, "//");
      size += 2;
    }
  }
  
  /* [scheme:][//] */
  
  if (url->username != NULL) {
    strcpy(dest + size, url->username);
    size += strlen(url->username);
    
    if (url->password != NULL) {
      *(dest + size) = ':';
      size++;
      strcpy(dest + size, url->password);
      size += strlen(url->password);
    }
    *(dest + size) = '@';
    size++;
  }
  
  /* [scheme:][//][user[:password]@] */
  
  if (url->domains != NULL) {
    if (url->ipv == 6) { /* IPv6 */
      *(dest + size) = '[';
      size++;
      strcpy(dest + size, url->domains);
      size += strlen(url->domains);
      *(dest + size) = ']';
      size++;
    } else { /* IPv4 */
      strcpy(dest + size, url->domains);
      size += strlen(url->domains);
    }
    
    if (url->port) {
      *(dest + size) = ':';
      size++;
      strcpy(dest + size, port_arr);
      size += strlen(port_arr);
    }
    
    if ((url->path == NULL || *url->path != '/') /* improper path */
        && ((url->parameters != NULL && strlen(url->parameters))
          || (url->query != NULL && strlen(url->query))
          || (url->fragment != NULL && strlen(url->fragment)))) {
      *(dest + size) = '/';
      size++;
    }
  }
  
  /* [scheme:][//][user[:password]@][host[:port]] */
  
  if (url->path != NULL) {
    if (resolve_path) {
      normpath(dest + size, url->path);
      size += strlen(dest + size);
    } else {
      strcpy(dest + size, url->path);
      size += strlen(url->path);
    }
  }
  
  /* [scheme:][//][user[:password]@][host[:port]][/path] */
  
  for (i = 0; i < 3; i++) {
    if (primary_attribs[i] != NULL) {
      *(dest + size) = primary_delims[i];
      size++;
      strcpy(dest + size, primary_attribs[i]);
      size += strlen(primary_attribs[i]);
    }
  }
  
  /*
  [scheme:][//][user[:password]@][host[:port]]
  [/path][;parameters][?query][#fragment]
  */
  
  *(dest + size) = '\0';
  return dest;
}
