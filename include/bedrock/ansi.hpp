#pragma once

// Reset
#define ANSI_RESET "\x1b[0m"
// Regular colors
#define ANSI_BLACK "\x1b[30m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_WHITE "\x1b[37m"
// gray / debug
#define ANSI_GRAY "\x1b[90m"
// green / info
#define ANSI_GREEN "\x1b[32m"
// yellow / warn
#define ANSI_YELLOW "\x1b[33m"
// red / error
#define ANSI_RED "\x1b[31m"
// magenta / fatal
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_ORANGE "\x1b[38;5;208m"

// Bright colors (optional but commonly useful)
#define ANSI_BBLACK "\x1b[90m"
#define ANSI_BRED "\x1b[91m"
#define ANSI_BGREEN "\x1b[92m"
#define ANSI_BYELLOW "\x1b[93m"
#define ANSI_BBLUE "\x1b[94m"
#define ANSI_BMAGENTA "\x1b[95m"
#define ANSI_BCYAN "\x1b[96m"
#define ANSI_BWHITE "\x1b[97m"

// Styles
#define ANSI_BOLD "\x1b[1m"
#define ANSI_DIM "\x1b[2m"
#define ANSI_ITALIC "\x1b[3m"
#define ANSI_UNDERLINE "\x1b[4m"
#define ANSI_BLINK "\x1b[5m"
#define ANSI_REVERSE "\x1b[7m"
#define ANSI_HIDDEN "\x1b[8m"
#define ANSI_STRIKE "\x1b[9m"

#ifdef NANUS_ANSI_COLOR
#define ANSI_STR(color, str) (ANSI_##color str ANSI_RESET)
#else
#define ANSI_STR(color, str) (str)
#endif
