#include "./../common/include/messages.h"
#include "./../common/include/bd.h"
#include "../common/include/util.h"
#include "../common/include/crypto.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_COMMANDS 100

// Structure du tableau circulaire pour les commandes
Commande* commandQueue[MAX_COMMANDS];
int queueStart = 0;
int queueEnd = 0;

// Mutex et variables conditionnelles pour la synchronisation
pthread_mutex_t mutexQueue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condQueueNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t condQueueNotFull = PTHREAD_COND_INITIALIZER;

void enqueueCommand(Commande *cmd) {
    pthread_mutex_lock(&mutexQueue);

    while ((queueEnd + 1) % MAX_COMMANDS == queueStart) {
        notif(RED, "Buffer plein, en attente d'une place...\\n");
        pthread_cond_wait(&condQueueNotFull, &mutexQueue);
    }

    commandQueue[queueEnd] = cmd;
    queueEnd = (queueEnd + 1) % MAX_COMMANDS;

    notif(GREEN, "Commande ajoutée au buffer.\\n");
    pthread_cond_signal(&condQueueNotEmpty);
    pthread_mutex_unlock(&mutexQueue);
}

Commande* dequeueCommand() {
    pthread_mutex_lock(&mutexQueue);

    while (queueStart == queueEnd) {
        notif(RED, "Buffer vide, en attente d'une commande...");
        pthread_cond_wait(&condQueueNotEmpty, &mutexQueue);
    }

    Commande *cmd = commandQueue[queueStart];
    queueStart = (queueStart + 1) % MAX_COMMANDS;

    notif(GREEN, "Commande récupérée du buffer.");
    pthread_cond_signal(&condQueueNotFull);
    pthread_mutex_unlock(&mutexQueue);

    return cmd;
}

/**
 * CRUD Electeur
 */
void traitementCreerElecteur(AjoutElecteurCmd *cmd) {
    printf("Traitement AjoutElecteurCmd\n");
    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant manquant.\n");
        return;
    }
    // Ouvrir la base de données
    sqlite3 *db;
    if (sqlite3_open("./../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }
    // Appeler createElecteur
    createElecteur(db, cmd->identifiant, ENTITY_ID_SIZE); // ENTITY_ID_SIZE est la taille de l'identifiant.

    // Fermer la base de données
    sqlite3_close(db);

}

void traitementLireElecteur(LireElecteurCmd *cmd) {
    printf("Traitement LireElecteurCmd\n");

    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    readElecteur(db, cmd->identifiant, ENTITY_ID_SIZE);

    sqlite3_close(db);
}


void traitementModifierElecteur(ModifierElecteurCmd *cmd) {
    printf("Traitement ModifierElecteurCmd\n");

    if (cmd == NULL || cmd->ancienIdentifiant[0] == '\0' || cmd->nouvelIdentifiant[0] == '\0') {
        printf("Commande invalide, identifiant manquant ou nouvel identifiant manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Demander confirmation avant de procéder à la mise à jour
    printf("Confirmez-vous la mise à jour de l'électeur '%s' vers '%s'? (y/n): ", cmd->ancienIdentifiant, cmd->nouvelIdentifiant);
    char confirmation[2];
    if (fgets(confirmation, 2, stdin) && confirmation[0] == 'y') {
        updateElecteur(db, cmd->ancienIdentifiant, ENTITY_ID_SIZE, cmd->nouvelIdentifiant, ENTITY_ID_SIZE);
    } else {
        printf("Mise à jour annulée.\n");
    }

    sqlite3_close(db);
}


void traitementSupprimerElecteur(SupprimeElecteurCmd *cmd) {
    printf("Traitement SupprimeElecteurCmd\n");
    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant manquant.\n");
        return;
    }
    // Ouvrir la base de données
    sqlite3 *db;
    if (sqlite3_open("./../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }
    // Appeler deleteElecteur
    deleteElecteur(db, cmd->identifiant, ENTITY_ID_SIZE);

    // Fermer la base de données
    sqlite3_close(db);
}

/**
 * CRUD Election
 */
// Fonctions de traitement des commandes élection
void traitementCreerElection(CreerElectionCmd *cmd) {
    printf("Traitement CreerElectionCmd\n");
    if (!cmd) {
        printf("Commande invalide.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur d'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    createElection(db, cmd->identifiant, ENTITY_ID_SIZE, cmd->question, cmd->dateDebut, cmd->dateFin, cmd->status);
    sqlite3_close(db);
}

void traitementLireElection(LireElectionCmd *cmd) {
    printf("Traitement LireElectionCmd\n");

    if(cmd == NULL || cmd->idElection < 1){
        printf("Commande invalide ou ID d'élection manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    readElection(db, cmd->idElection);

    sqlite3_close(db);
}

void traitementModifierElection(ModifierElectionCmd *cmd) {
    printf("Traitement ModifierElectionCmd\n");

    if(cmd == NULL || cmd->idElection < 1 || cmd->nouvelleQuestion[0] == '\0'){
        printf("Commande invalide, ID d'élection manquant ou nouvelle question manquante.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    updateElection(db, cmd->idElection, cmd->nouvelleQuestion);

    sqlite3_close(db);
}


void traitementSupprimerElection(SupprimerElectionCmd *cmd) {
    printf("Traitement SupprimerElectionCmd\n");

    if(cmd == NULL || cmd->idElection < 1){
        printf("Commande invalide ou ID d'élection manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    deleteElection(db, cmd->idElection);

    sqlite3_close(db);
}


/**
 * CRUD Vote
 */
void traitementCreerVote(CreerVoteCmd *cmd) {
    printf("Traitement CreerVoteCmd\n");

    if (cmd == NULL || cmd->idElection < 1 || cmd->idVotant < 1) {
        printf("Commande invalide ou données manquantes pour le vote.\n");
        return;
    }

    // Ouvre la base de données
    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Ici, utilise les membres corrects de cmd pour la fonction Election_castVote
    Election_castVote(db, cmd->idVotant, cmd->idElection, cmd->ballot, sizeof(cmd->ballot), cmd->hashValidation);

    // Ferme la base de données
    sqlite3_close(db);
}

void traitementLireVote(LireVoteCmd *cmd) {
    printf("Traitement LireVoteCmd\n");

    if (cmd == NULL || cmd->idElection < 1) {
        printf("Commande invalide ou ID d'élection manquant.\n");
        return;
    }

    // Ouvre la base de données
    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Traite les résultats des votes
    int option0 = 0, option1 = 0, totalVotes = 0;
    Election_processVotes(db, cmd->idElection, &option0, &option1, &totalVotes);

    printf("Total votes: %d\nOption 0: %d\nOption 1: %d\n", totalVotes, option0, option1);

    // Ferme la base de données
    sqlite3_close(db);
}





// Thread pour le traitement des commandes
void* processCommands(void* arg) {
    while (1) {
        Commande *cmd = dequeueCommand(); // Récupère une commande de la file
        if (cmd == NULL) {
            continue; // Si la commande est NULL, passe à la suivante
        }

        switch (cmd->type) {
            case AJOUT_ELECTEUR:
                traitementCreerElecteur(&cmd->commande.ajoutElecteur);
                break;
            case LIRE_ELECTEUR:
                traitementLireElecteur(&cmd->commande.lireElecteur);
                break;
            case MODIFIER_ELECTEUR:
                traitementModifierElecteur(&cmd->commande.modifierElecteur);
                break;
            case SUPPRIME_ELECTEUR:
                traitementSupprimerElecteur(&cmd->commande.supprimeElecteur);
                break;
            case CREER_ELECTION:
                traitementCreerElection(&cmd->commande.creerElection);
                break;
            case LIRE_ELECTION:
                traitementLireElection(&cmd->commande.lireElection);
                break;
            case MODIFIER_ELECTION:
                traitementModifierElection(&cmd->commande.modifierElection);
                break;
            case SUPPRIMER_ELECTION:
                traitementSupprimerElection(&cmd->commande.supprimerElection);
                break;
            case CREER_VOTE:
                traitementCreerVote(&cmd->commande.creerVote);
                break;
            case LIRE_VOTE:
                traitementLireVote(&cmd->commande.lireVote);
                break;
                // Ajouter d'autres cas si nécessaire
            default:
                printf("Type de commande non reconnu: %d\n", cmd->type);
                break;
        }

        free(cmd);
    }
    return NULL;
}
