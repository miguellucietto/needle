#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <log.h>
#include "shared.h"







static bool Flags[256];


int get_flags(int argc, char **argv){
  if (!argv || !*argv)
    return EXIT_FAILURE;

  
  for (int i = 0; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
	Flags[(unsigned char) argv[i][j]] = true;
      }
    }
  }
  
  return EXIT_SUCCESS;
}

bool is_flag(const char *arg) {
  if (!arg) return false;

  return arg[0] == '-';
}

bool has_flag(char f) {
  return Flags[(unsigned char) f];
}


int warn_invalid_flags(const char *rflags) {
  if (!rflags) return EXIT_FAILURE;
  char nflags[256];
  int count = 0;


  for (int i = 0; i < sizeof(Flags); i++) {
    if (!Flags[i])
      continue;

    bool allowed = false;
    for (int j = 0; rflags[j] != '\0'; j++) {
      if (i == rflags[j]) allowed = true;
    }
    if (!allowed) {
      nflags[count++] = (unsigned char) i;
    }
    
  }
  nflags[count] = '\0';
  if (nflags[0] != '\0') { 
    fprintf(stderr, "[WARN] The flags ");
    for (int i = 0; i < count; i++) {
      fprintf(stderr, "'%c' ", nflags[i]);
    }
    fprintf(stderr, "are not real flags\n");
  }
  return EXIT_SUCCESS;
}



char *strdup(const char *text) {
  if (!text) {
    return NULL;
  }

  int len = strlen(text) + 1;
  char *copy = malloc(len);

  memcpy(copy, text, len);
  copy[len] = '\0';

  return copy;
}
