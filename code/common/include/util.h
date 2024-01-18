#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#ifndef COMMON_H
#define COMMON_H

#define PORT 1025
#define TCP 0
#define BUFFER_SIZE 1024

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN     "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum Color { RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, NONE };

void err_n_die(char *message, ...);

void notif(enum Color color, char* format, ...);

char* eliminerEntreesStardardPath(char *path);

#endif /* COMMON_H */