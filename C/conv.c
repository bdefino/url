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

#include "conv.h"

/* conversions */

const char *DIGITS_MAP = "0123456789"; /* digits mapped by index */

/* character array to time_t */
time_t atot(const char *a) {
  time_t t;
  memset(&t, '\0', sizeof(time_t));
  
  if (a != NULL) {
    unsigned int exp;
    int left, right;
    
    exp = 0;
    left = right = strlen(a);
    
    while (left > 0) {
      while (left > 0 && *(a + left - 1) != ':')
        left--;
      char temp[right - left + 1];
      
      strncpy(temp, a + left, right - left);
      temp[right - left] = '\0';
      t += pow(60, exp) * atoi(temp);
      exp++;
      right = left = left - 1;
    }
  }
  return t;
}

/* integer to character array; assume a is NULL or of sufficient size */
char *itoa(char *a, int i) {
  int ai, copy;
  unsigned int size;
  size = 1;
  
  if ((copy = i) < 0)
    copy *= -1, size++;
  
  while ((copy /= 10) > 0)
    size++;
  
  if (a != NULL || (a = (char *) malloc(size + 1)) != NULL) {
    *(a + (ai = size)) = '\0';
    copy = (i < 0 ? -i : i);
    
    while (ai-- >= 0) {
      *(a + ai) = *(DIGITS_MAP + copy % 10);
      copy /= 10;
    }
    
    if (i < 0)
      *a = '-';
  }
  return a;
}

/* time_t to character array; assume a is NULL or of sufficient size */
char *ttoa(char *a, time_t t) {
  if (t > 0) {
    time_t copy;
    unsigned int nsegs, size;
    copy = t, nsegs = 1;
    
    while ((copy /= 60) > 0)
      nsegs++;
    size = 3 * nsegs;
    
    if (a != NULL || (a = (char *) malloc(size)) != NULL) {
      int i;
      char seg[3];
      *(a + size - 1) = '\0';
      
      for (i = size - 3; t > 0; i -= 3) {
        itoa(seg, t % 60);
        
        if (seg[1] == '\0')
          *(a + i) = '0', *(a + i + 1) = seg[0];
        else
          *(a + i) = seg[0], *(a + i + 1) = seg[1];
        
        if (i >= 0)
          *(a + i - 1) = ':';
        t /= 60;
      }
    }
  }
  return a;
}