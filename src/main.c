#include <stdio.h>
#include <stdlib.h>
#include <log.h>
#include <unistd.h>

#include "find.h"
#include "shared.h"
#include "search.h"


bool show_lines = false;

int main(int argc, char **argv) {
  if (argc < 3) {
    WARN("needle needs at least 2 arguments");
    return EXIT_SUCCESS;
  }

  get_flags(argc - 2, argv + 2);
  show_lines = has_flag('l');
  
  char *needle = argv[1];

  char **file_paths = &argv[2];
  
  for (int i = 0; i < argc - 2; i++) {
    if (is_flag(file_paths[i])) continue;
    
    char *path = file_paths[i];

    FILE *f = fopen(path, "r");
    if (!f) {
      ERROR("Could not open file '%s'", path);
      return EXIT_FAILURE;
    }

    char **lines = get_file_lines(f);
    if (!lines)
      return 1;
    printf("----------------------------------------------------");
    printf("\n%s: \n\n", path);

    for (int i = 0; lines[i] != NULL; i++) {
      if (find_in(needle, lines[i]) == EXIT_SUCCESS) {
        if (!show_lines) {
          printf("    %s\n", lines[i]);
        } else {
	  printf("%d    %s\n", i + 1, lines[i]);
        }
      }
    }

    fclose(f);
  }


  warn_invalid_flags("l");
  return 0;
}
