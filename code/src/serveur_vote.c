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
Commande *commandQueue[MAX_COMMANDS];
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

Commande *dequeueCommand() {
    pthread_mutex_lock(&mutexQueue);

    while (queueStart == queueEnd) {
        notif(RED, "Buffer vide, en attente d'une commande...");
        pthread_cond_wait(&condQueueNotEmpty, &mutexQueue);
    }

    Commande *cmd = commandQueue[queueStart];
    queueStart = (queueStart + 1) % MAX_COMMANDS;

    notif(GREEN, "Commande récupérée du buffer.\n");
    pthread_cond_signal(&condQueueNotFull);
    pthread_mutex_unlock(&mutexQueue);

    return cmd;
}

/**
 * CRUD Electeur
 */
void traitementCreerElecteur(AjoutElecteurCmd *cmd) {
    printf("Traitement AjoutElecteurCmd\n");
    if (cmd == NULL || cmd->numeroID[0] == '\0') {
        printf("Commande invalide ou identifiant manquant.\n");
        return;
    }
    // Ouvrir la base de données (assurez-vous que le chemin est correct)
    sqlite3 *db;
    if (sqlite3_open("./../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Avant d'insérer un électeur, vérifiez s'il existe déjà
    if (electeurExists(db, cmd->numeroID, ENTITY_ID_SIZE) != 0) {
        printf("L'électeur avec le numeroID %s existe déjà.\n", cmd->numeroID);
    } else {
        createElecteur(db, cmd->numeroID, ENTITY_ID_SIZE);
    }
    // Fermer la base de données
    sqlite3_close(db);
}

void traitementLireElecteur(LireElecteurCmd *cmd) {
    printf("Traitement LireElecteurCmd\n");
    if (cmd == NULL || cmd->numeroID[0] == '\0') {
        printf("Commande invalide ou numeroID manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Vérifie si l'électeur existe dans la base de données
    if (electeurExists(db, cmd->numeroID, ENTITY_ID_SIZE)) {
        // Si l'électeur existe, procède à la lecture de ses informations
        readElecteur(db, cmd->numeroID, ENTITY_ID_SIZE);
    } else {
        // Si l'électeur n'existe pas, affiche un message d'erreur
        printf("L'électeur avec le numeroID spécifié n'existe pas dans la base de données.\n");
    }

    sqlite3_close(db);
}

void traitementModifierElecteur(ModifierElecteurCmd *cmd) {
    printf("Traitement ModifierElecteurCmd\n");

    if (cmd == NULL || cmd->ancienNumeroID[0] == '\0' || cmd->nouvelNumeroID[0] == '\0') {
        printf("Commande invalide, numeroID manquant ou nouvel numeroID manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (!electeurExists(db, cmd->ancienNumeroID, ENTITY_ID_SIZE)) {
        printf("L'électeur avec le numeroID '%s' n'existe pas.\n", cmd->ancienNumeroID);
    } else {
        // Procède directement à la mise à jour sans demander de confirmation
        updateElecteur(db, cmd->ancienNumeroID, ENTITY_ID_SIZE, cmd->nouvelNumeroID, ENTITY_ID_SIZE);
    }

    sqlite3_close(db);
}


void traitementSupprimerElecteur(SupprimeElecteurCmd *cmd) {
    printf("Traitement SupprimeElecteurCmd\n");
    if (cmd == NULL || cmd->numeroID[0] == '\0') {
        printf("Commande invalide ou numeroID manquant.\n");
        return;
    }
    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Vérifie si l'électeur existe avant de tenter de le supprimer
    if (!electeurExists(db, cmd->numeroID, ENTITY_ID_SIZE)) {
        printf("L'électeur avec le numeroID '%s' n'existe pas.\n", cmd->numeroID);
    } else {
        // Appeler deleteElecteur pour supprimer l'électeur
        deleteElecteur(db, cmd->numeroID, ENTITY_ID_SIZE);
    }

    sqlite3_close(db);
}

/**
 * CRUD Election
 */
void traitementCreerElection(CreerElectionCmd *cmd) {
    printf("Traitement CreerElectionCmd\n");
    if (!cmd) {
        printf("Commande invalide.\n");
        return;
    }

    // Nettoyage et vérification du statut
    char statusClean[256];
    sscanf(cmd->status, "%255s", statusClean);
    if (strcmp(statusClean, "active") != 0 && strcmp(statusClean, "closed") != 0 &&
        strcmp(statusClean, "canceled") != 0) {
        printf("Statut non valide. Les valeurs autorisées sont 'active', 'closed', 'canceled'.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur d'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Vérification de l'existence de l'élection
    if (electionExists(db, cmd->identifiant, ENTITY_ID_SIZE) != 0) {
        printf("L'élection avec l'identifiant '%s' existe déjà.\n", cmd->identifiant);
    } else {
        createElection(db, cmd->identifiant, ENTITY_ID_SIZE, cmd->question, cmd->dateDebut, cmd->dateFin, statusClean);
        printf("Élection créée avec succès.\n");
    }

    sqlite3_close(db);
}

void traitementLireElection(LireElectionCmd *cmd) {
    printf("Traitement LireElectionCmd\n");
    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant d'élection manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }
    // Vérification de l'existence de l'élection avant de la lire
    if (electionExists(db, cmd->identifiant, ENTITY_ID_SIZE) == 0) {
        printf("L'élection avec l'identifiant '%s' n'existe pas.\n", cmd->identifiant);
    } else {
        readElectionByIdentifiant(db, cmd->identifiant, ENTITY_ID_SIZE);
    }


    sqlite3_close(db);
}


void traitementModifierElection(ModifierElectionCmd *cmd) {
    printf("Traitement ModifierElectionCmd\n");

    //Affichage de la nouvelle question
    //printf("Nouvelle question : %s\n", cmd->nouvelleQuestion);
    //Affichage de l'identifiant
    //printf("Identifiant : %s\n", cmd->identifiant);

    if (cmd == NULL || cmd->identifiant[0] == '\0' || cmd->nouvelleQuestion[0] == '\0') {
        printf("Commande invalide, identifiant d'élection manquant ou nouvelle question manquante.\n");
        return;
    }
    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (electionExists(db, cmd->identifiant, ENTITY_ID_SIZE) == 0) {
        printf("L'élection avec l'identifiant '%s' n'existe pas.\n", cmd->identifiant);
    } else {
        updateElectionByIdentifiant(db, cmd->identifiant, ENTITY_ID_SIZE, cmd->nouvelleQuestion);
        printf("Élection modifiée avec succès.\n");
    }

    sqlite3_close(db);
}


void traitementSupprimerElection(SupprimerElectionCmd *cmd) {
    printf("Traitement SupprimerElectionCmd\n");

    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant d'élection manquant.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }
    // Vérification de l'existence de l'élection avant de supprimer
    if (!electionExists(db, cmd->identifiant, ENTITY_ID_SIZE)) {
        printf("L'élection avec l'identifiant '%s' n'existe pas.\n", cmd->identifiant);
    } else {
        printf("Suppression de l'élection avec l'identifiant '%s'...\n", cmd->identifiant);
        //Affiche l'identifiant de l'élection à supprimer avec
        printf("L'élection avec l'identifiant '%s' a été supprimée.\n", cmd->identifiant);
        deleteElectionByIdentifiant(db, cmd->identifiant, ENTITY_ID_SIZE);
        printf("Élection supprimée avec succès.\n");
    }

    sqlite3_close(db);
}


/**
 * CRUD Vote
 */
void traitementCreerVote(CreerVoteCmd *cmd) {
    printf("Traitement CreerVoteCmd\n");

    if (!cmd) {
        printf("La commande est NULL.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    int idVotant = Electeur_getIdFromNumeroID(db, cmd->numeroID, ENTITY_ID_SIZE);
    int idElection = Election_getIdFromNumeroID(db, cmd->identifiant, ENTITY_ID_SIZE);

    printf("idVotant : %d, idElection : %d\n", idVotant, idElection);

    if (idElection < 1 || idVotant < 1) {
        printf("ID d'électeur ou d'élection invalide.\n");
        sqlite3_close(db);
        return;
    }

    if (hasUserAlreadyVoted(db, idVotant, idElection)) {
        printf("L'électeur avec l'ID %d a déjà voté pour l'élection %d.\n", idVotant, idElection);
        sqlite3_close(db);
        return;
    }

    // Initialisation des variables GMP pour la cryptographie
    mpz_t n, g, lambda, mu;
    mpz_inits(n, g, lambda, mu, NULL);
    generate_keys(n, lambda, g, mu);

    Election_castVote(db, idVotant, idElection, cmd->ballot, n, g);

    mpz_clears(n, g, lambda, mu, NULL);
    sqlite3_close(db);
}


void traitementLireVote(LireVoteCmd *cmd) {
    printf("Traitement LireVoteCmd\n");

    if (!cmd || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant d'élection manquant.\n");
        return;
    }

    // Supprimer le caractère de nouvelle ligne de l'identifiant si présent
    cmd->identifiant[strcspn(cmd->identifiant, "\n")] = 0;

    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    //Affichage de l'identifiant
    printf("Identifiant : %s\n", cmd->identifiant);
    int idElection = Election_getIdFromNumeroID(db,cmd->identifiant, ENTITY_ID_SIZE);
    printf("idElection : %d\n", idElection);

    if (idElection == -1) {
        printf("L'élection avec l'identifiant '%s' n'existe pas.\n", cmd->identifiant);
        sqlite3_close(db);
        return;
    }

    // Initialisation des variables GMP pour la cryptographie
    mpz_t n, g, lambda, mu;
    mpz_inits(n, g, lambda, mu, NULL);

    // Générer les clés publiques et privées (n, g, lambda, mu)
    // Cette étape peut varier en fonction de votre système. Vous pourriez avoir besoin de récupérer les clés d'une autre manière.
    generate_keys(n, lambda, g, mu);

    // Traitement et affichage des votes
    Election_processVotes(db, idElection, lambda, mu, n);

    // Libération des ressources GMP et de la base de données
    mpz_clears(n, g, lambda, mu, NULL);
    sqlite3_close(db);
}


// Thread pour le traitement des commandes
void *processCommands(void *arg) {
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
            default:
                printf("Type de commande non reconnu: %d\n", cmd->type);
                break;
        }

        free(cmd);
    }
    return NULL;
}
