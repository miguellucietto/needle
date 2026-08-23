#define _POSIX_C_SOURCE 200809L

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "color.h"
#include "search.h"

static void print_match(const char *line, size_t length, const char *pattern)
{
    const size_t pattern_length = strlen(pattern);
    const char *cursor = line;
    const char *end = line + length;

    while (cursor < end) {
        const char *match = strstr(cursor, pattern);
        if (match == NULL || match >= end) {
            fwrite(cursor, 1, (size_t)(end - cursor), stdout);
            break;
        }

        fwrite(cursor, 1, (size_t)(match - cursor), stdout);
        printf("%s%s", color_bold(), color_red());
        fwrite(match, 1, pattern_length, stdout);
        printf("%s", color_reset());
        cursor = match + pattern_length;
    }

    if (length == 0 || line[length - 1] != '\n') {
        putchar('\n');
    }
}

static int search_file(const char *path, const SearchOptions *options,
                       SearchStats *stats)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "needle: %s: %s\n", path, strerror(errno));
        ++stats->errors;
        return -1;
    }

    char *line = NULL;
    size_t capacity = 0;
    size_t line_number = 0;
    bool printed_header = false;

    ++stats->files;
    while (!stats->stopped) {
        const ssize_t length = getline(&line, &capacity, file);
        if (length < 0) {
            break;
        }

        ++line_number;
        if (memchr(line, '\0', (size_t)length) != NULL ||
            strstr(line, options->pattern) == NULL) {
            continue;
        }

        if (!printed_header) {
            printf("%s%s%s%s:\n", color_bold(), color_cyan(), path,
                   color_reset());
            printed_header = true;
        }
        if (options->show_line_numbers) {
            printf("%s%zu%s:", color_green(), line_number, color_reset());
        }
        print_match(line, (size_t)length, options->pattern);
        ++stats->matches;

        if (options->first_match) {
            stats->stopped = true;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "needle: %s: read error\n", path);
        ++stats->errors;
    }

    free(line);
    fclose(file);
    return 0;
}

static int search_directory(const char *path, const SearchOptions *options,
                            SearchStats *stats)
{
    DIR *directory = opendir(path);
    if (directory == NULL) {
        fprintf(stderr, "needle: %s: %s\n", path, strerror(errno));
        ++stats->errors;
        return -1;
    }

    struct dirent *entry;
    while (!stats->stopped && (entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0 ||
            strcmp(entry->d_name, ".git") == 0) {
            continue;
        }

        const size_t size = strlen(path) + strlen(entry->d_name) + 2;
        char *child = malloc(size);
        if (child == NULL) {
            fprintf(stderr, "needle: out of memory\n");
            ++stats->errors;
            break;
        }
        snprintf(child, size, "%s/%s", path, entry->d_name);

        struct stat status;
        if (lstat(child, &status) != 0) {
            fprintf(stderr, "needle: %s: %s\n", child, strerror(errno));
            ++stats->errors;
        } else if (S_ISDIR(status.st_mode)) {
            search_directory(child, options, stats);
        } else if (S_ISREG(status.st_mode)) {
            search_file(child, options, stats);
        }

        free(child);
    }

    closedir(directory);
    return stats->errors == 0 ? 0 : -1;
}

int search_path(const char *path, const SearchOptions *options,
                SearchStats *stats)
{
    struct stat status;
    if (lstat(path, &status) != 0) {
        fprintf(stderr, "needle: %s: %s\n", path, strerror(errno));
        ++stats->errors;
        return -1;
    }

    if (S_ISDIR(status.st_mode)) {
        if (!options->recursive) {
            fprintf(stderr, "needle: %s: is a directory (use -r)\n", path);
            ++stats->errors;
            return -1;
        }
        return search_directory(path, options, stats);
    }
    if (!S_ISREG(status.st_mode)) {
        return 0;
    }
    return search_file(path, options, stats);
}
