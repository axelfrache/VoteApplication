#include "../../common/include/votechain.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>

#define V if (verbose)
#define ID_SIZE 10

int main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;

    gboolean verbose = FALSE;
    GOptionEntry entries[] = {
            {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
            {NULL}};

    context = g_option_context_new("- test pour la création de la base de donnée");
    g_option_context_add_main_entries(context, entries, NULL);

    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }

    if (verbose)
    {
        g_print("Mode verbeux activé\n");
    }

    const char *dbPath = "../data_base/base_de_donnees.db";
    sqlite3 *db;

    db = database_open(dbPath);
    if (!db) {
        fprintf(stderr, "Impossible d'ouvrir ou de créer la base de données.\n");
        return 1;
    }

    if (clear_database(db) != 0) {
        fprintf(stderr, "Erreur lors du vidage de la base de données.\n");
        sqlite3_close(db);
        return 1;
    }

    if (database_init(db) != 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de la base de données.\n");
        sqlite3_close(db);
        return 1;
    }

    // Test: Création d'une élection
    char id[ID_SIZE] = "EXXXXXXXXX";
    const char *question = "This is a binary question";
    createElection(db, id, ID_SIZE, question, "01/01/2023", "02/01/2023", "active");
    printf("Élection créée avec succès.\n");

    // Test: Vérification de l'existence de l'élection
    int localid = Election_getIdFromNumeroID(db, id, ID_SIZE);
    g_assert(localid == 1);
    printf("L'élection existe et son ID local est correct (ID attendu: 1, ID obtenu: %d).\n", localid);

    sqlite3_close(db);
    g_option_context_free(context);
    return 0;
}
