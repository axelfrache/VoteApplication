#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "./../common/include//messages.h"
#include "./../common/include/bd.h"
#include "../common/include/util.h"

extern void enqueueCommand(Commande*);
extern void* processCommands(void* arg);


void handleElecteurCRUD(int choix) {
    Commande *cmd = malloc(sizeof(Commande));
    char identifiant[ENTITY_ID_SIZE];

    if (!cmd) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return;
    }
    memset(cmd, 0, sizeof(Commande));

    switch (choix) {
        case 1: // Ajouter Electeur
            cmd->type = AJOUT_ELECTEUR;
            printf("Entrez l'identifiant de l'électeur à ajouter: ");
            fgets(identifiant, ENTITY_ID_SIZE, stdin);
            strncpy(cmd->commande.ajoutElecteur.identifiant, identifiant, ENTITY_ID_SIZE-1);
            break;
        case 2: // Lire Electeur
            cmd->type = LIRE_ELECTEUR;
            printf("Entrez l'identifiant de l'électeur à lire: ");
            fgets(identifiant, ENTITY_ID_SIZE, stdin);
            strncpy(cmd->commande.lireElecteur.identifiant, identifiant, ENTITY_ID_SIZE-1);
            break;
        case 3: // Modifier Electeur
            cmd->type = MODIFIER_ELECTEUR;
            printf("Entrez l'ancien identifiant de l'électeur à modifier: ");
            fgets(identifiant, ENTITY_ID_SIZE, stdin);
            strncpy(cmd->commande.modifierElecteur.ancienIdentifiant, identifiant, ENTITY_ID_SIZE-1);
            printf("Entrez le nouvel identifiant de l'électeur: ");
            fgets(identifiant, ENTITY_ID_SIZE, stdin);
            strncpy(cmd->commande.modifierElecteur.nouvelIdentifiant, identifiant, ENTITY_ID_SIZE-1);
         break;
        case 4: // Supprimer Electeur
            cmd->type = SUPPRIME_ELECTEUR;
            printf("Entrez l'identifiant de l'électeur à supprimer: ");
            fgets(identifiant, ENTITY_ID_SIZE, stdin);
            strncpy(cmd->commande.supprimeElecteur.identifiant, identifiant, ENTITY_ID_SIZE-1);
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
    char buffer[1024];

    if (!cmd) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return;
    }
    memset(cmd, 0, sizeof(Commande));

    switch (choix) {
        case 1: // Créer Election
            cmd->type = CREER_ELECTION;
            printf("Entrez l'identifiant de l'élection à créer: ");
            fgets(cmd->commande.creerElection.identifiant, sizeof(cmd->commande.creerElection.identifiant), stdin);
            printf("Entrez la question de l'élection: ");
            fgets(cmd->commande.creerElection.question, sizeof(cmd->commande.creerElection.question), stdin);
            printf("Entrez la date de début (YYYY-MM-DD): ");
            fgets(cmd->commande.creerElection.dateDebut, sizeof(cmd->commande.creerElection.dateDebut), stdin);
            printf("Entrez la date de fin (YYYY-MM-DD): ");
            fgets(cmd->commande.creerElection.dateFin, sizeof(cmd->commande.creerElection.dateFin), stdin);
            printf("Entrez le statut de l'élection (active/closed/canceled): ");
            fgets(cmd->commande.creerElection.status, sizeof(cmd->commande.creerElection.status), stdin);
            break;
        case 2: // Lire Election
            cmd->type = LIRE_ELECTION;
            notif(GREEN,"Entrez l'ID de l'élection à lire: ");
            fgets(buffer, sizeof(buffer), stdin);
            strcpy(cmd->commande.lireElection.identifiant, buffer);
            break;
        case 3: // Modifier Election
            cmd->type = MODIFIER_ELECTION;
            notif(GREEN,"Entrez l'ID de l'élection à modifier: ");
            fgets(buffer, sizeof(buffer), stdin);
            strcpy(cmd->commande.modifierElection.identifiant, buffer);
            notif(GREEN, "Entrez la nouvelle question de l'élection: ");
            fgets(cmd->commande.modifierElection.nouvelleQuestion, sizeof(cmd->commande.modifierElection.nouvelleQuestion), stdin);
            break;
        case 4: // Supprimer Election
            cmd->type = SUPPRIMER_ELECTION;
            notif(GREEN,"Entrez l'ID de l'élection à supprimer: ");
            fgets(buffer, sizeof(buffer), stdin);
            strcpy(cmd->commande.supprimerElection.identifiant, buffer);
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
    char buffer[1024];

    if (!cmd) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return;
    }
    memset(cmd, 0, sizeof(Commande));

    switch (choix) {
        case 1: // Créer Vote
            cmd->type = CREER_VOTE;
            notif(GREEN, "Entrez l'identifiant de l'électeur votant: ");
            fgets(buffer, sizeof(buffer), stdin);
            cmd->commande.creerVote.idVotant = atoi(buffer);
            notif(GREEN, "Entrez l'identifiant de l'élection pour le vote: ");
            fgets(buffer, sizeof(buffer), stdin);
            strcpy(cmd->commande.creerVote.identifiantElection, buffer);
            // Ici, tu dois saisir et affecter les données du vote (cmd->commande.creerVote.ballot, etc.) selon ton implémentation
            enqueueCommand(cmd);
            break;
        case 2: // Lire les résultats des votes
            cmd->type = LIRE_VOTE;
            notif(GREEN, "Entrez l'ID de l'élection pour voir les résultats: ");
            fgets(buffer, sizeof(buffer), stdin);
            strcpy(cmd->commande.lireVote.idElection, buffer);
            enqueueCommand(cmd);
            break;
        case 3: // Retour au menu principal
            free(cmd);
            return;
        default:
            notif(RED, "Choix non valide.\n");
            free(cmd);
            return;
    }
}

void* receiveCommands(void* arg) {
    int choixPrincipal, choixCRUD;
    char buffer[1024];
    Commande *cmd;

    while (1) {
        notif(NONE, "\nMenu Principal:\n");
        notif(YELLOW, "1. Electeur\n");
        notif(YELLOW, "2. Election\n");
        notif(YELLOW, "3. Vote\n");
        notif(YELLOW, "4. Quitter\n");
        printf("Votre choix: ");
        fgets(buffer, sizeof(buffer), stdin);
        choixPrincipal = atoi(buffer);

        switch (choixPrincipal) {
            case 1: // Electeur
                printf("\nElecteur - Choisissez une action:\n");
                notif(YELLOW, "1. Ajouter Electeur\n");
                notif(YELLOW, "2. Lire Electeur\n");
                notif(YELLOW, "3. Modifier Electeur\n");
                notif(YELLOW, "4. Supprimer Electeur\n");
                notif(YELLOW, "5. Retour au menu principal\n");
                notif(NONE, "Votre choix: ");
                fgets(buffer, sizeof(buffer), stdin);
                choixCRUD = atoi(buffer);
                handleElecteurCRUD(choixCRUD);
                break;

            case 2: // Election
                printf("\nElection - Choisissez une action:\n");
                notif(YELLOW, "1. Créer Election\n");
                notif(YELLOW, "2. Lire Election\n");
                notif(YELLOW, "3. Modifier Election\n");
                notif(YELLOW, "4. Supprimer Election\n");
                notif(YELLOW, "5. Retour au menu principal\n");
                notif(NONE, "Votre choix: ");
                fgets(buffer, sizeof(buffer), stdin);
                choixCRUD = atoi(buffer);
                handleElectionCRUD(choixCRUD);
                break;

            case 3: // Vote
                notif(NONE, "\nVote - Choisissez une action:\n");
                notif(YELLOW, "1. Créer Vote\n");
                notif(YELLOW, "2. Lire les résultats des votes\n");
                notif(YELLOW,"3. Retour au menu principal\n");
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
        sleep(2);
    }
}

int main(int argc, char *argv[]) {

    pthread_t threadReceiver, threadProcessor;

    // Initialisation de la base de données
    const char *dbPath = "../data_base/base_de_donnees.db";
    sqlite3 *db;

    // Ouvrir ou créer la base de données si elle n'existe pas
    db = database_open(dbPath);
    
    if (!db) {
        err_n_die("Impossible d'ouvrir ou de créer la base de données.\n");
        return 1;
    }

    // Initialiser la base de données en créant les tables nécessaires
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
