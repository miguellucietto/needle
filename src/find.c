#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <log.h>
#include <unistd.h>

#include "find.h"
#include "shared.h"



static char *get_entire_file(FILE *f) {
  if (!f)
    return NULL;

  size_t count = 0;
  size_t cap = 2048;
  char *content = malloc(cap);
  if (!content) {
    ERROR("Failed to get file content");
    return NULL;
  }

  int c = fgetc(f);
  do {
    if (count + 1 >= cap) {
      char *tmp = realloc(content, cap * 2);
      if (!tmp) {
        ERROR("Could not realloc file content");
	free(content);
        return NULL;
      }
      cap *= 2;
      content = tmp;
    }

    content[count++] = c;
  } while ((c = fgetc(f)) != EOF);


  if (count + 1 >= cap) {
    char *tmp = realloc(content, cap + 1);
    if (!tmp) {
      ERROR("Could not realloc file content");
      free(content);
      return NULL;
    }
    cap += 1;
    content = tmp;
  }

  content[count] = '\0';
  
  return content;
}

char **get_file_lines(FILE *f) {
  if (!f)
    return NULL;
  
  size_t count = 0, cap = 128;
  char *fcontent = get_entire_file(f);
  if (!fcontent) {
    return NULL;
  }
  char **lines = calloc(cap, sizeof(char *));
  if (!lines) {
    ERROR("Could not allocate lines");
    free(fcontent);
    return NULL;
  }
  
  char *line = strtok(fcontent, "\n");
  
  do {
    if (count + 1 >= cap) {
      char **tmp = realloc(lines, sizeof(char *) * (cap * 2));
      if (!tmp) {
        ERROR("Could not reallocate lines");
	free(fcontent);
        free(lines);
	return NULL;
      }
      lines = tmp;
      cap *= 2; 
    }
    lines[count++] = line;
  } while ((line = strtok(NULL, "\n")) != NULL);
  
  if (count + 1 >= cap) {
    char **tmp = realloc(lines, sizeof(char *) * (cap + 1));
    if (!tmp) {
      ERROR("Could not realloc lines");
      free(lines);
      return NULL;
    }
    cap += 1;
    lines = tmp;
    
  }
  return lines;
}
  
int find_in(const char *needle, const char *haystack) {
  if (!needle || !haystack) {
      return EXIT_FAILURE;
    }
    
    char *found = strstr(haystack, needle);

    if (!found) {
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
