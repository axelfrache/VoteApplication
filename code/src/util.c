#include "../../common/include/common.h"

// Fonction de gestion d'erreurs

void err_n_die(char *message, ...)
{
    va_list args;
    int errno_save = errno;

    // On montre le message d'erreur ainsi que les valeurs supplémentaires
    fprintf(stderr, "Error: ");

    va_start(args, message);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
    fflush(stderr);

    if (errno_save != 0)
    {
        fprintf(stderr, "(errno  = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stderr, "\n");
        fflush(stderr);
    }

    exit(EXIT_FAILURE);
}
/*

Cette fonction permet d'afficher des messages dans la sortie standard du terminal

IMPORTANT: si vous utiliser plusieurs arguments, veuiller mettre NULL comme dernier caractère pour définir la fin de la chaine

*/
void notif(enum Color color, char *format,  ...)
{
    va_list args;
    va_start(args, format);
    switch (color)
    {
    case RED:
        fprintf(stdout, ANSI_COLOR_RED);
        vfprintf(stdout, format, args);
        fprintf(stdout, ANSI_COLOR_RESET"\n");
        break;
    case GREEN:
        fprintf(stdout, ANSI_COLOR_GREEN);
        vfprintf(stdout, format, args);
        fprintf(stdout, ANSI_COLOR_RESET"\n");
        break;
    case YELLOW:
        fprintf(stdout, ANSI_COLOR_YELLOW);
        vfprintf(stdout, format, args);
        fprintf(stdout, ANSI_COLOR_RESET"\n");
        break;
    case BLUE:
        fprintf(stdout, ANSI_COLOR_BLUE);
        vfprintf(stdout, format, args);
        fprintf(stdout, ANSI_COLOR_RESET"\n");
        break;
    case MAGENTA:
        fprintf(stdout, ANSI_COLOR_MAGENTA);
        vfprintf(stdout, format, args);
        fprintf(stdout, ANSI_COLOR_RESET"\n");
        break;
    case CYAN:
        fprintf(stdout, ANSI_COLOR_CYAN);
        vfprintf(stdout, format, args);
        fprintf(stdout, ANSI_COLOR_RESET"\n");
        break;
    case NONE:
        vfprintf(stdout, format, args);
        fprintf(stdout, "\n");
        break;
    }
    fflush(stdout);
    va_end(args);
}
