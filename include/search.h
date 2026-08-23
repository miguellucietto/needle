#ifndef NEEDLE_SEARCH_H
#define NEEDLE_SEARCH_H

#include <stdbool.h>
#include <stddef.h>

typedef struct SearchOptions {
    const char *pattern;
    bool recursive;
    bool show_line_numbers;
    bool first_match;
} SearchOptions;

typedef struct SearchStats {
    size_t files;
    size_t matches;
    size_t errors;
    bool stopped;
} SearchStats;

int search_path(const char *path, const SearchOptions *options,
                SearchStats *stats);

#endif
