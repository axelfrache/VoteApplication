#include "../../common/include/votechain.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <sqlite3.h>

#define V if (verbose)
#define ENTITY_ID_SIZE 256

void traitementCreerElecteur(AjoutElecteurCmd *cmd);
void traitementCreerElection(CreerElectionCmd *cmd);
void traitementCreerVote(CreerVoteCmd *cmd);
void traitementLireVote(LireVoteCmd *cmd);

void afficherSeparateurTest(const char *titreTest);
void afficherResultatTest(const char *testDescription, gboolean reussi);

int main(int argc, char *argv[]) {
    GError *error = NULL;
    GOptionContext *context;
    gboolean verbose = FALSE;
    GOptionEntry entries[] = {
            {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
            {NULL}
    };

    context = g_option_context_new("- test pour la création de la base de donnée");
    g_option_context_add_main_entries(context, entries, NULL);

    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }

    V g_print("Mode verbeux activé\n");

    const char *dbPath = "../data_base/base_de_donnees.db";
    sqlite3 *db = database_open(dbPath);
    if (!db) {
        fprintf(stderr, "Impossible d'ouvrir ou de créer la base de données.\n");
        return 1;
    }

    if (clear_database(db) != 0 || database_init(db) != 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de la base de données.\n");
        sqlite3_close(db);
        return 1;
    }

    // Créer un électeur
    afficherSeparateurTest("Création d'un électeur");
    AjoutElecteurCmd creerElecteurCmd = {
            .numeroID = "ELECTEUR123"  // Assurez-vous que cet ID est unique
    };
    traitementCreerElecteur(&creerElecteurCmd);

    // Créer une élection
    afficherSeparateurTest("Création d'une élection");
    CreerElectionCmd creerElectionCmd = {
            .identifiant = "ELECT123",  // Assurez-vous que cet ID est unique
            .question = "Question 1?",
            .dateDebut = "2023-01-01",
            .dateFin = "2023-12-31",
            .status = "active"
    };
    traitementCreerElection(&creerElectionCmd);

    // Créer un vote
    afficherSeparateurTest("Création d'un vote");
    CreerVoteCmd creerVoteCmd = {
            .numeroID = "ELECTEUR123",  // Utilisez l'ID de l'électeur créé
            .identifiant = "ELECT123",  // Utilisez l'ID de l'élection créée
            .ballot = "Oui"  // ou toute autre option de vote que votre système prend en charge
    };
    traitementCreerVote(&creerVoteCmd);

    // Lire les votes
    afficherSeparateurTest("Lecture des votes");
    LireVoteCmd lireVoteCmd = {
            .identifiant = "ELECT123"  // Utilisez l'ID de l'élection pour laquelle vous voulez lire les votes
    };
    traitementLireVote(&lireVoteCmd);

    sqlite3_close(db);
    g_option_context_free(context);
    return 0;
}

void afficherSeparateurTest(const char *titreTest) {
    printf("\n================== %s ==================\n", titreTest);
}

void afficherResultatTest(const char *testDescription, gboolean reussi) {
    printf("%s: %s\n", testDescription, reussi ? "Réussi" : "Échoué");
    printf("-------------------------------------------------\n");
}
