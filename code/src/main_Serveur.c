#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

#include <pthread.h>

extern void enqueueCommand(Commande*);
extern void* processCommands(void* arg);
sqlite3 *db;

void handleElecteurCRUD(int choix) {
    Commande *cmd = malloc(sizeof(Commande));
    char numeroID[ENTITY_ID_SIZE];

    if (!cmd) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return;
    }
    memset(cmd, 0, sizeof(Commande));

    switch (choix) {
        case 1: // Ajouter Electeur
            cmd->type = AJOUT_ELECTEUR;
            printf("Entrez le numeroID de l'électeur à ajouter: ");
            fgets(numeroID, ENTITY_ID_SIZE, stdin);
            numeroID[strcspn(numeroID, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            strncpy(cmd->commande.ajoutElecteur.numeroID, numeroID, ENTITY_ID_SIZE-1);
            break;
        case 2: // Lire Electeur
            cmd->type = LIRE_ELECTEUR;
            printf("Entrez le numeroID de l'électeur à lire: ");
            fgets(numeroID, ENTITY_ID_SIZE, stdin);
            numeroID[strcspn(numeroID, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            strncpy(cmd->commande.lireElecteur.numeroID, numeroID, ENTITY_ID_SIZE-1);
            break;
        case 3: // Modifier Electeur
            cmd->type = MODIFIER_ELECTEUR;
            printf("Entrez l'ancien numeroID de l'électeur à modifier: ");
            fgets(numeroID, ENTITY_ID_SIZE, stdin);
            numeroID[strcspn(numeroID, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            strncpy(cmd->commande.modifierElecteur.ancienNumeroID, numeroID, ENTITY_ID_SIZE-1);
            printf("Entrez le nouvel numeroID de l'électeur: ");
            fgets(numeroID, ENTITY_ID_SIZE, stdin);
            numeroID[strcspn(numeroID, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            strncpy(cmd->commande.modifierElecteur.nouvelNumeroID, numeroID, ENTITY_ID_SIZE-1);
            break;
        case 4: // Supprimer Electeur
            cmd->type = SUPPRIME_ELECTEUR;
            printf("Entrez le numeroID de l'électeur à supprimer: ");
            fgets(numeroID, ENTITY_ID_SIZE, stdin);
            numeroID[strcspn(numeroID, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            strncpy(cmd->commande.supprimeElecteur.numeroID, numeroID, ENTITY_ID_SIZE-1);
            break;
        case 5: // Retour au menu principal
            free(cmd);
            return;
        default:
            printf("Choix non valide.\n");
            free(cmd);
            return;
    }
    enqueueCommand(cmd);
}

void handleElectionCRUD(int choix) {
    Commande *cmd = malloc(sizeof(Commande));
    char buffer[ENTITY_ID_SIZE];

    if (!cmd) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return;
    }
    memset(cmd, 0, sizeof(Commande));

    switch (choix) {
        case 1: // Créer Election
            cmd->type = CREER_ELECTION;
            printf("Entrez l'identifiant de l'élection à créer: ");
            fgets(cmd->commande.creerElection.identifiant, ENTITY_ID_SIZE, stdin);
            cmd->commande.creerElection.identifiant[strcspn(cmd->commande.creerElection.identifiant, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            printf("Entrez la question de l'élection: ");
            fgets(cmd->commande.creerElection.question, sizeof(cmd->commande.creerElection.question), stdin);
            cmd->commande.creerElection.question[strcspn(cmd->commande.creerElection.question, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            printf("Entrez la date de début (YYYY-MM-DD): ");
            fgets(cmd->commande.creerElection.dateDebut, sizeof(cmd->commande.creerElection.dateDebut), stdin);
            cmd->commande.creerElection.dateDebut[strcspn(cmd->commande.creerElection.dateDebut, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            printf("Entrez la date de fin (YYYY-MM-DD): ");
            fgets(cmd->commande.creerElection.dateFin, sizeof(cmd->commande.creerElection.dateFin), stdin);
            cmd->commande.creerElection.dateFin[strcspn(cmd->commande.creerElection.dateFin, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            printf("Entrez le statut de l'élection (active/closed/canceled): ");
            fgets(cmd->commande.creerElection.status, sizeof(cmd->commande.creerElection.status), stdin);
            cmd->commande.creerElection.status[strcspn(cmd->commande.creerElection.status, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            break;
        case 2: // Lire Election
            cmd->type = LIRE_ELECTION;
            printf("Entrez l'identifiant de l'élection à lire: ");
            fgets(cmd->commande.lireElection.identifiant, ENTITY_ID_SIZE, stdin);
            cmd->commande.lireElection.identifiant[strcspn(cmd->commande.lireElection.identifiant, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            break;
        case 3: // Modifier Election
            cmd->type = MODIFIER_ELECTION;
            printf("Entrez l'identifiant de l'élection à modifier: ");
            fgets(cmd->commande.modifierElection.identifiant, ENTITY_ID_SIZE, stdin);
            cmd->commande.modifierElection.identifiant[strcspn(cmd->commande.modifierElection.identifiant, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            printf("Entrez la nouvelle question de l'élection: ");
            fgets(cmd->commande.modifierElection.nouvelleQuestion, ENTITY_ID_SIZE, stdin);
            cmd->commande.modifierElection.nouvelleQuestion[strcspn(cmd->commande.modifierElection.nouvelleQuestion, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            break;
        case 4: // Supprimer Election
            cmd->type = SUPPRIMER_ELECTION;
            printf("Entrez l'identifiant de l'élection à supprimer: ");
            fgets(cmd->commande.supprimerElection.identifiant, ENTITY_ID_SIZE, stdin);
            cmd->commande.supprimerElection.identifiant[strcspn(cmd->commande.supprimerElection.identifiant, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            break;
        case 5: // Retour au menu principal
            free(cmd);
            return;
        default:
            printf("Choix non valide.\n");
            free(cmd);
            return;
    }
    enqueueCommand(cmd);
}

void handleVoteCRUD(int choix) {
    Commande *cmd = malloc(sizeof(Commande));
    char buffer[ENTITY_ID_SIZE];

    if (!cmd) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return;
    }
    memset(cmd, 0, sizeof(Commande));

    switch (choix) {
        case 1: // Créer Vote
            cmd->type = CREER_VOTE;
            printf("Entrez le numéroID de l'électeur votant: ");
            fgets(cmd->commande.creerVote.numeroID, ENTITY_ID_SIZE, stdin);
            cmd->commande.creerVote.numeroID[strcspn(cmd->commande.creerVote.numeroID, "\n")] = 0; // Supprime le caractère de nouvelle ligne

            printf("Entrez l'identifiant de l'élection pour le vote: ");
            fgets(cmd->commande.creerVote.identifiant, ENTITY_ID_SIZE, stdin);
            cmd->commande.creerVote.identifiant[strcspn(cmd->commande.creerVote.identifiant, "\n")] = 0; // Supprime le caractère de nouvelle ligne

            // Récupération de l'idElection à partir de l'identifiant
            int idElection = Election_getIdFromNumeroID(db, cmd->commande.creerVote.identifiant, ENTITY_ID_SIZE);
            if (idElection > 0) {
                // Afficher la question de l'élection
                afficheQuestionVote(db, idElection);
            } else {
                printf("L'élection spécifiée n'existe pas.\n");
                break; // Sortie précoce si l'élection n'existe pas
            }

            printf("Entrez le choix du vote: ");
            fgets(cmd->commande.creerVote.ballot, 256, stdin);
            cmd->commande.creerVote.ballot[strcspn(cmd->commande.creerVote.ballot, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            enqueueCommand(cmd);
            break;

        case 2: // Lire les résultats des votes
            cmd->type = LIRE_VOTE;
            printf("Entrez l'ID de l'élection pour voir les résultats: ");
            fgets(cmd->commande.lireVote.identifiant, ENTITY_ID_SIZE, stdin);
            cmd->commande.lireVote.identifiant[strcspn(cmd->commande.lireVote.identifiant, "\n")] = 0; // Supprime le caractère de nouvelle ligne
            enqueueCommand(cmd);
            break;
        case 3: // Retour au menu principal
            free(cmd);
            return;
        default:
            printf("Choix non valide.\n");
            free(cmd);
            return;
    }
}

void* receiveCommands(void* arg) {
    int choixPrincipal, choixCRUD;
    char buffer[1024];
    Commande *cmd;

    while (1) {
        printf("\nMenu Principal:\n");
        printf("1. Electeur\n");
        printf("2. Election\n");
        printf("3. Vote\n");
        printf("4. Quitter\n");
        printf("Votre choix: ");
        fgets(buffer, sizeof(buffer), stdin);
        choixPrincipal = atoi(buffer);

        switch (choixPrincipal) {
            case 1: // Electeur
                printf("\nElecteur - Choisissez une action:\n");
                printf("1. Ajouter Electeur\n");
                printf("2. Lire Electeur\n");
                printf("3. Modifier Electeur\n");
                printf("4. Supprimer Electeur\n");
                printf("5. Retour au menu principal\n");
                printf("Votre choix: ");
                fgets(buffer, sizeof(buffer), stdin);
                choixCRUD = atoi(buffer);
                handleElecteurCRUD(choixCRUD);
                break;

            case 2: // Election
                printf("\nElection - Choisissez une action:\n");
                printf("1. Créer Election\n");
                printf("2. Lire Election\n");
                printf("3. Modifier Election\n");
                printf("4. Supprimer Election\n");
                printf("5. Retour au menu principal\n");
                printf("Votre choix: ");
                fgets(buffer, sizeof(buffer), stdin);
                choixCRUD = atoi(buffer);
                handleElectionCRUD(choixCRUD);
                break;

            case 3: // Vote
                printf("\nVote - Choisissez une action:\n");
                printf("1. Créer Vote\n");
                printf("2. Lire les résultats des votes\n");
                printf("3. Retour au menu principal\n");
                printf("Votre choix: ");
                fgets(buffer, sizeof(buffer), stdin);
                choixCRUD = atoi(buffer);
                handleVoteCRUD(choixCRUD);
                break;

            case 4: // Quitter
                printf("Fermeture du serveur...\n");
                return NULL;

            default:
                printf("Choix non valide.\n");
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    pthread_t threadReceiver, threadProcessor;

    // Initialisation de la base de données
    const char *dbPath = "../data_base/base_de_donnees.db";

    // Ouvrir ou créer la base de données si elle n'existe pas
    db = database_open(dbPath);
    if (!db) {
        fprintf(stderr, "Impossible d'ouvrir ou de créer la base de données.\n");
        return 1;
    }

    // Vider la base de données
    if (clear_database(db) != 0) {
        fprintf(stderr, "Erreur lors du vidage de la base de données.\n");
        sqlite3_close(db);
        return 1;
    }

    // Initialiser la base de données en recréant les tables nécessaires
    if (database_init(db) != 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de la base de données.\n");
        sqlite3_close(db);
        return 1;
    }

    // Création des threads pour la réception et le traitement des commandes
    pthread_create(&threadReceiver, NULL, receiveCommands, NULL);
    pthread_create(&threadProcessor, NULL, processCommands, NULL);

    // Attente de la fin des threads
    pthread_join(threadReceiver, NULL);
    pthread_join(threadProcessor, NULL);

    // Fermeture de la base de données
    if (database_close(db) != 0) {
        fprintf(stderr, "Erreur lors de la fermeture de la base de données.\n");
    }
    return 0;
}
