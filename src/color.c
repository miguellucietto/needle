#include <stdlib.h>
#include <unistd.h>

#include "color.h"

static bool use_color;

void color_init(ColorMode mode)
{
    const bool disabled_by_environment = getenv("NO_COLOR") != NULL;

    switch (mode) {
    case COLOR_ALWAYS:
        use_color = !disabled_by_environment;
        break;
    case COLOR_NEVER:
        use_color = false;
        break;
    case COLOR_AUTO:
        use_color = isatty(STDOUT_FILENO) && !disabled_by_environment;
        break;
    }
}

const char *color_bold(void)
{
    return use_color ? "\033[1m" : "";
}

const char *color_cyan(void)
{
    return use_color ? "\033[36m" : "";
}

const char *color_green(void)
{
    return use_color ? "\033[32m" : "";
}

const char *color_red(void)
{
    return use_color ? "\033[31m" : "";
}

const char *color_reset(void)
{
    return use_color ? "\033[0m" : "";
}
