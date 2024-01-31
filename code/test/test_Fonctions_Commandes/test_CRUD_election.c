#include "../../common/include/votechain.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>

#define V if (verbose)
#define ID_SIZE 10

void traitementCreerElection(CreerElectionCmd *cmd);
void traitementLireElection(LireElectionCmd *cmd);
void traitementModifierElection(ModifierElectionCmd *cmd);
void traitementSupprimerElection(SupprimerElectionCmd *cmd);

// Fonction d'affichage pour les séparateurs de test
void afficherSeparateurTest(const char *titreTest) {
    printf("\n================== %s ==================\n", titreTest);
}

// Fonction d'affichage pour les résultats des tests
void afficherResultatTest(const char *testDescription, gboolean reussi) {
    printf("%s: %s\n", testDescription, reussi ? "Réussi" : "Échoué");
    printf("-------------------------------------------------\n");
}

int main(int argc, char *argv[]) {
    GError *error = NULL;
    GOptionContext *context;

    // Définition des options
    gboolean verbose = FALSE;
    GOptionEntry entries[] = {
            {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
            {NULL}};

    // Création d'un contexte d'option
    context = g_option_context_new("- test pour la gestion des élections");
    g_option_context_add_main_entries(context, entries, NULL);

    // Analyse des options de ligne de commande
    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }

    // Affichage des résultats
    if (verbose) {
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

    // Début des tests

    afficherSeparateurTest("Test de création d'élection");
    CreerElectionCmd creerCmd = {.identifiant = "ELECT123", .question = "Question 1?", .dateDebut = "2023-01-01", .dateFin = "2023-12-31", .status = "active"};
    traitementCreerElection(&creerCmd);

    afficherSeparateurTest("Test de lecture d'élection");
    LireElectionCmd lireCmd = {.idElection = 1}; // Assurez-vous que cet ID existe ou est correct après la création
    traitementLireElection(&lireCmd);

    afficherSeparateurTest("Test de modification d'élection");
    ModifierElectionCmd modifierCmd = {.idElection = 1, .nouvelleQuestion = "Question modifiée?"};
    traitementModifierElection(&modifierCmd);

    afficherSeparateurTest("Test de suppression d'élection");
    SupprimerElectionCmd supprimerCmd = {.idElection = 1};
    traitementSupprimerElection(&supprimerCmd);

    // Fermeture de la base de données
    sqlite3_close(db);

    // Libération de la mémoire
    g_option_context_free(context);
    return 0;
}
