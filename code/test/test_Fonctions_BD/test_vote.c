#include "../../common/include/votechain.h"
#include "../../common/include/crypto.h"
#include <glib.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>

#define V if (verbose)
#define ID_SIZE 10
#define BALLOT_SIZE 1

// Vous devez définir ou inclure les fonctions 'encrypt' et 'decrypt' utilisées dans 'Election_castVote' et 'Election_processVotes'

int main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;

    // Définition des options
    gboolean verbose = FALSE;
    GOptionEntry entries[] = {
            {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
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

    // Initialisation de la base de données
    const char *dbPath = "../data_base/base_de_donnees.db";
    sqlite3 *db;

    // Ouvrir ou créer la base de données si elle n'existe pas
    db = database_open(dbPath);
    if (!db) {
        fprintf(stderr, "Impossible d'ouvrir ou de créer la base de données.\n");
        return 1;
    }

    // Vider la base de données et l'initialiser
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

    // Préparation des paramètres de cryptographie (ces valeurs doivent être définies correctement selon votre implémentation)
    mpz_t n, g, lambda, mu;
    mpz_inits(n, g, lambda, mu, NULL);
    generate_keys(n, lambda, g, mu); // Assurez-vous que cette fonction est correctement implémentée dans crypto.c



    // Début des tests pour l'élection
    char idElection[ID_SIZE] = "EXXXXXXXXX";
    const char *question = "This is a binary question";
    createElection(db, idElection, ID_SIZE, question, "01/01/2023", "02/01/2023", "active");
    int election_LocalId = Election_getIdFromNumeroID(db, idElection, ID_SIZE);
    g_assert(election_LocalId == 1);

    // Création des électeurs et soumission des votes
    char idElecteur1[ID_SIZE] = "PXXXXXXXX1";
    char idElecteur2[ID_SIZE] = "PXXXXXXXX2";
    char idElecteur3[ID_SIZE] = "PXXXXXXXX3";

    createElecteur(db, idElecteur1, ID_SIZE);
    createElecteur(db, idElecteur2, ID_SIZE);
    createElecteur(db, idElecteur3, ID_SIZE);

    int electeur_n01 = getIdFromNumeroID(db, idElecteur1, ID_SIZE);
    int electeur_n02 = getIdFromNumeroID(db, idElecteur2, ID_SIZE);
    int electeur_n03 = getIdFromNumeroID(db, idElecteur3, ID_SIZE);

    // Soumission des votes
    Election_castVote(db, electeur_n01, election_LocalId, "O", n, g);
    Election_castVote(db, electeur_n02, election_LocalId, "N", n, g);
    Election_castVote(db, electeur_n03, election_LocalId, "N", n, g);

    // Traitement et affichage des résultats de l'élection
    int no = 0;
    int yes = 0;
    int total = 0;

    Election_processVotes(db, election_LocalId, lambda, mu, n);

    // Libérer les ressources de cryptographie
    mpz_clears(n, g, lambda, mu, NULL);

    // Fermeture de la base de données
    sqlite3_close(db);

    // Libération de la mémoire
    g_option_context_free(context);
    return 0;
}
