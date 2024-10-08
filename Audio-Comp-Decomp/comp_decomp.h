#ifndef MULAWAPI_H
#define MULAWAPI_H

#include <stdio.h>
#include <string.h>
#include "read_wave_file.h"
// macros

//Chage to 1 for Debugging mode and 0 otherwise
#define DEBUG 0
#define debug_print(fmt, ...)                    \
    do                                           \
    {                                            \
        if (DEBUG)                               \
            fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)

// Functions

int signum(int sample);

int magnitude(int sample);

int codeword_compression(int sample_magnitude, int sign);

int codeword_decompression(int codeWord);

int Test(int sample);

#endif