#pragma once

#include <stdio.h> // fprintf, stderr

/* ---------------------------------------------------------------------------
 * Print debugging
 * ------------------------------------------------------------------------- */
#define COLOR_RESET        "\033[0m"
#define COLOR_RED          "\033[0;31m"
#define COLOR_BLUE         "\033[0;34m"
#define COLOR_GREEN        "\033[0;32m"
#define COLOR_RED_BOLD     "\033[1;31m"
#define COLOR_BLUE_BOLD    "\033[1;34m"
#define COLOR_GREEN_BOLD   "\033[1;32m"
#define COLOR_RED_ITALIC   "\033[3;31m"
#define COLOR_BLUE_ITALIC  "\033[3;34m"
#define COLOR_GREEN_ITALIC "\033[3;32m"

#define title(...) (fprintf(stderr, COLOR_BLUE_BOLD), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))
#define dbg(...)   (fprintf(stderr, COLOR_BLUE), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))

/* ---------------------------------------------------------------------------
 * Error handling
 * ------------------------------------------------------------------------- */
#define error(...)                                                                                                     \
    (fprintf(stderr, COLOR_RED "Error: "), fprintf(stderr, __VA_ARGS__), fprintf(stderr, COLOR_RESET "\n"))

#define die(err, ...)                                                                                                  \
    if ((err) != 0)                                                                                                    \
    {                                                                                                                  \
        error(__VA_ARGS__);                                                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    }

/* ---------------------------------------------------------------------------
 * Math
 * ------------------------------------------------------------------------- */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
