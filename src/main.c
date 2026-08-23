#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "search.h"
#include "shared.h"

int main(int argc, char **argv)
{
    Arguments arguments = {
        .paths = argv,
    };

    const int parse_status = parse_arguments(argc, argv, &arguments);
    if (parse_status > 0) {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }
    if (parse_status < 0) {
        fprintf(stderr, "Try '%s --help' for usage.\n", argv[0]);
        return 2;
    }

    color_init(arguments.color);

    SearchStats stats = {0};
    for (size_t index = 0;
         index < arguments.path_count && !stats.stopped;
         ++index) {
        search_path(arguments.paths[index], &arguments.search, &stats);
    }

    if (stats.errors > 0) {
        return 2;
    }
    return stats.matches > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
