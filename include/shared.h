#ifndef NEEDLE_SHARED_H
#define NEEDLE_SHARED_H

#include <stddef.h>

#include "color.h"
#include "search.h"

typedef struct Arguments {
    SearchOptions search;
    ColorMode color;
    char **paths;
    size_t path_count;
} Arguments;

int parse_arguments(int argc, char **argv, Arguments *arguments);
void print_usage(const char *program);

#endif
