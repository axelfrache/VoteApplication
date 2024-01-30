#include "../common/include/votechain.h"
#include "../src/serveur_vote.c"
#include "../common/include/messages.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define V if (verbose)
#define ID_SIZE 10

int main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;

    // Définition des options
    gboolean verbose = FALSE;
    gchar *file_path = NULL;
    GOptionEntry entries[] = {
        {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
        {"file", 'f', 0, G_OPTION_ARG_STRING, &file_path, "Chemin du fichier", "FILE"},
        {NULL}};

    // Création d'un contexte d'option
    context = g_option_context_new("- test pour la création de la base de donnée");
    g_option_context_add_main_entries(context, entries, NULL);

    // Analyse des options de ligne de commande
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }

    // Affichage des résultats
    if (verbose)
    {
        g_print("Mode verbeux activé\n");
    }
    if (file_path != NULL)
    {
        g_print("Chemin du fichier: %s\n", file_path);
        int needInit = database_exists(file_path);
        struct stat buffer;
        g_print("File %s exists ? %d", file_path, (stat(file_path, &buffer)));

        g_print("needInit: %d\n", needInit);
        sqlite3 *db = database_open(file_path);
        g_assert_nonnull(db);
        // Creation de la BD s'il n'existe pas
        if (needInit == -1)
        {
            g_print("initilisation de la base de données");
            database_init(db);
        }

        // creation d'une election
        CreerElectionCmd *cmd1;

        int id[ID_SIZE];

        cmd1->identifiant = "XXXXXXXXX1";
        cmd1->question = "Ce test va t'il passer ?";
        cmd1->dateDebut = "2024-01-30";
        cmd1->dateFin = "2024-01-31";
        cmd1->status = "active";

        traitementCreerElection(cmd1);

        int localid = getIdFromNumeroID(db, id, ID_SIZE);
        g_print("Numero ID when returned : %d", localid);

        // createElecteur(db, id2, 10);

        //read election
        LireElectionCmd *electionCmd;
        electionCmd->idElection = id;
        traitementLireElection(electionCmd);

        int readId = getIdFromNumeroID(db, id, ID_SIZE);
        g_print("readId = %d\n", readId);

        g_assert(electeurExists(db, id, ID_SIZE) == 1);
        

        // Update election
        updateElecteur(db, id, 10, id2, ID_SIZE);
        int r = electeurExists(db, id, ID_SIZE);
        g_print("r = %d", r);
        g_assert(r == 0);
        g_assert(electeurExists(db, id2, ID_SIZE) == 1);
        
        // Delete Election
        deleteElecteur(db, id2, ID_SIZE);
        g_assert(electeurExists(db, id2, ID_SIZE) == 0);
    
        


        


        
    }
    else
    {
        g_print("Aucun chemin de fichier fourni\n");
    }

    // Libération de la mémoire
    g_free(file_path);
    g_option_context_free(context);
    return 0;
}
