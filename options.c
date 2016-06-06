/* -*- C -*- */
#include "options.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void processNumericArg(const char* value, uint64_t *target, const char* what) {
  char* endptr;
  uint64_t temp = strtoul(value, &endptr, 10);
  uint64_t multiplier = 1;
  if (endptr > value) {
    /* got at least one digit */
    if (*endptr == 0) {
      ; /* leave multiplier at 1 */
    } else if (endptr[1] == 0) {
      switch(*endptr) {
      case 'k':
      case 'K':
        multiplier = 1000; break;

      case 'm':
      case 'M':
        multiplier = 1000000; break;

      case 'g':
      case 'G':
        multiplier = 1000000000; break;

      default:
        fprintf(stderr, "%s has invalid multiplier; '%s' ignored, using %lu\n",
                what, value, *target);
      }
    } else {
      fprintf(stderr, "%s has junk after multiplier; '%s' ignored, using %lu\n",
              what, value, *target);
      return;
    }

    if (temp == 0) {
      fprintf(stderr, "%s may not be zero; '%s' ignored, using %lu\n",
              what, value, *target);
    } else {
      *target = temp*multiplier;
    }
  } else {
    fprintf(stderr, "invalid %s '%s' ignored, using %lu\n",
            what, value, *target);
  }
}

static const char emptyString[] = "";

void split(char* source, char delim, char** first, char** remainder)
{
  *first = source;
  char* where = strchr(source, delim);
  if (where != 0) {
    *where = 0;
    *remainder = where+1;
  } else {
    /* point to the end of the string */
    *remainder = source+strlen(source);
  }
}
