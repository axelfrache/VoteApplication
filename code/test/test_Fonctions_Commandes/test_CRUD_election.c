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

// Déclarations des fonctions pour les opérations CRUD sur les élections
void traitementCreerElection(CreerElectionCmd *cmd);
void traitementLireElection(LireElectionCmd *cmd);
void traitementModifierElection(ModifierElectionCmd *cmd);
void traitementSupprimerElection(SupprimerElectionCmd *cmd);

// Fonctions auxiliaires pour les tests
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

    context = g_option_context_new("- test pour la gestion des élections");
    g_option_context_add_main_entries(context, entries, NULL);
    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }

    V g_print("Mode verbeux activé\n");

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

    // Début des tests
    afficherSeparateurTest("Test de création d'élection");
    CreerElectionCmd creerCmd = {.identifiant = "ELECT123", .question = "Question 1?", .dateDebut = "2023-01-01", .dateFin = "2023-12-31", .status = "active"};
    traitementCreerElection(&creerCmd);

    afficherSeparateurTest("Test de lecture d'élection");
    LireElectionCmd lireCmd = {.identifiant = "ELECT123"};
    traitementLireElection(&lireCmd);

    afficherSeparateurTest("Test de modification d'élection");
    ModifierElectionCmd modifierCmd = {.identifiant = "ELECT123", .nouvelleQuestion = "Question modifiée?"};
    traitementModifierElection(&modifierCmd);

    afficherSeparateurTest("Test de suppression d'élection");
    SupprimerElectionCmd supprimerCmd = {.identifiant = "ELECT123"};
    traitementSupprimerElection(&supprimerCmd);


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

