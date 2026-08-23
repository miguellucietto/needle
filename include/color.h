#ifndef NEEDLE_COLOR_H
#define NEEDLE_COLOR_H

#include <stdbool.h>

typedef enum ColorMode {
    COLOR_AUTO,
    COLOR_ALWAYS,
    COLOR_NEVER,
} ColorMode;

void color_init(ColorMode mode);
const char *color_bold(void);
const char *color_cyan(void);
const char *color_green(void);
const char *color_red(void);
const char *color_reset(void);

#endif
