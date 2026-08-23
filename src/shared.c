#include <stdio.h>
#include <string.h>

#include "shared.h"

static int parse_color(const char *value, ColorMode *mode)
{
    if (strcmp(value, "auto") == 0) {
        *mode = COLOR_AUTO;
    } else if (strcmp(value, "always") == 0) {
        *mode = COLOR_ALWAYS;
    } else if (strcmp(value, "never") == 0) {
        *mode = COLOR_NEVER;
    } else {
        fprintf(stderr, "needle: invalid color mode: %s\n", value);
        return -1;
    }
    return 0;
}

int parse_arguments(int argc, char **argv, Arguments *arguments)
{
    bool options_enabled = true;
    char **paths = arguments->paths;

    *arguments = (Arguments){
        .color = COLOR_AUTO,
        .paths = paths,
    };

    for (int index = 1; index < argc; ++index) {
        char *argument = argv[index];

        if (options_enabled && strcmp(argument, "--") == 0) {
            options_enabled = false;
            continue;
        }
        if (options_enabled && strncmp(argument, "--color=", 8) == 0) {
            if (parse_color(argument + 8, &arguments->color) != 0) {
                return -1;
            }
            continue;
        }
        if (options_enabled && strcmp(argument, "--help") == 0) {
            return 1;
        }
        if (options_enabled && argument[0] == '-' && argument[1] != '\0') {
            for (size_t flag = 1; argument[flag] != '\0'; ++flag) {
                switch (argument[flag]) {
                case 'n': arguments->search.show_line_numbers = true; break;
                case 'r': arguments->search.recursive = true; break;
                case 'f': arguments->search.first_match = true; break;
                case 'h': return 1;
                default:
                    fprintf(stderr, "needle: unknown option: -%c\n",
                            argument[flag]);
                    return -1;
                }
            }
            continue;
        }

        if (arguments->search.pattern == NULL) {
            arguments->search.pattern = argument;
        } else {
            arguments->paths[arguments->path_count++] = argument;
        }
    }

    if (arguments->search.pattern == NULL ||
        arguments->search.pattern[0] == '\0') {
        fprintf(stderr, "needle: a non-empty search pattern is required\n");
        return -1;
    }
    if (arguments->path_count == 0) {
        if (arguments->search.recursive) {
            arguments->paths[arguments->path_count++] = ".";
        } else {
            fprintf(stderr, "needle: at least one file is required\n");
            return -1;
        }
    }
    return 0;
}

void print_usage(const char *program)
{
    printf("Usage: %s [options] pattern [path ...]\n\n", program);
    puts("Search for a literal string in files.\n");
    puts("Options:");
    puts("  -n              show line numbers");
    puts("  -r              search directories recursively");
    puts("  -f              stop after the first match");
    puts("  -h, --help      show this help");
    puts("  --color=WHEN    colorize: auto, always, or never");
    puts("\nNO_COLOR disables ANSI colors in every mode.");
}
