#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <log.h>

#include "find.h"
#include "shared.h"
#include "search.h"


#define FLAGS (const char *) "r n f"


/*****************Flags*****************/

/* -n */ bool show_lines = false;   // Show the line numbers
/* -r */ bool recursive  = false;  // TODO: Search for the entire directory recursively and try to find the needle in every single file
/* -f */ bool only_first = false; // Only show the first line with the found needle

/***************************************/


void update_flags(int c, char **a) {
  get_flags(c, a);

  show_lines = has_flag('n');
  recursive = has_flag('r');
}


int main(int argc, char **argv) {
  if (argc < 3) {
    WARN("needle needs at least 2 arguments");
    return EXIT_SUCCESS;
  }
  update_flags(argc, argv);
  char *needle = argv[1];

  if (recursive) {
    int res = rec_search(".", needle);
    warn_invalid_flags(FLAGS);
    return res;
  }


  char **file_paths = &argv[2];

  for (int i = 0; i < argc - 2; i++) {
    if (is_flag(file_paths[i])) continue;

    char *path = file_paths[i];

    FILE *f = fopen(path, "r");
    if (!f) {
      ERROR("Could not open file '%s': %s", path, strerror(errno));
      return EXIT_FAILURE;
    }

    char **lines = get_file_lines(f);
    if (!lines)
      return 1;
    printf("----------------------------------------------------");
    printf("\n%s: \n\n", path);

    for (int i = 0; lines[i] != NULL; i++) {
      if (find_in(needle, lines[i]) != EXIT_SUCCESS)
        continue;


      if (!show_lines) {
	printf("    %s\n", lines[i]);
      } else {
	printf("%d    %s\n", i + 1, lines[i]);
      }

      if (only_first) exit(0);

    }

    fclose(f);
  }


  warn_invalid_flags(FLAGS);
  return 0;
}
