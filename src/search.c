#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dirs.h>
#include <threads.h>

#include <log.h>

#include "search.h"
#include "shared.h"
#include "find.h"


int rec_search(const char *path, const char *needle) {
  if (!path) {
    return EXIT_FAILURE;
  }
  putchar('\n');

  DIR *dir = opendir(path);
  for (struct dirent *ent = readdir(dir); ent; ent = readdir(dir)) {
    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".git") == 0)
      continue;


  }

  closedir(dir);

  return EXIT_SUCCESS;
}
