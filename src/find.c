#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "find.h"
#include "shared.h"





int find_in(const char *needle, const char *haystack) {
  if (!needle || !haystack) {
    return EXIT_FAILURE;
  }

  char *found = strstr(haystack, needle);
  
  return found ? EXIT_SUCCESS : EXIT_FAILURE;
}
