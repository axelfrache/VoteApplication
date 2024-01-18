#include "./../common/include//messages.h"
#include "./../common/include/bd.h"
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

// Ajouter une commande dans le tableau circulaire
void enqueueCommand(Commande *cmd) {
    pthread_mutex_lock(&mutexQueue);

    while ((queueEnd + 1) % MAX_COMMANDS == queueStart) {
        pthread_cond_wait(&condQueueNotFull, &mutexQueue);
    }

    commandQueue[queueEnd] = cmd;
    queueEnd = (queueEnd + 1) % MAX_COMMANDS;

    pthread_cond_signal(&condQueueNotEmpty);
    pthread_mutex_unlock(&mutexQueue);


}


// Récupérer une commande du tableau circulaire
Commande* dequeueCommand() {
    pthread_mutex_lock(&mutexQueue);

    while (queueStart == queueEnd) {
        pthread_cond_wait(&condQueueNotEmpty, &mutexQueue);
    }

    Commande *cmd = commandQueue[queueStart];
    queueStart = (queueStart + 1) % MAX_COMMANDS;

    pthread_cond_signal(&condQueueNotFull);
    pthread_mutex_unlock(&mutexQueue);

    return cmd;
}

/**
 * Fonctions de traitement des commandes
 */
void traitementAjoutElecteur(AjoutElecteurCmd *cmd) {
    printf("Traitement AjoutElecteurCmd\n");
    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant manquant.\n");
        return;
    }

    // Ouvrir la base de données (assurez-vous que le chemin est correct)
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

void traitementSupprimeElecteur(SupprimeElecteurCmd *cmd) {
    printf("Traitement SupprimeElecteurCmd\n");
    // Implémentation spécifique pour SupprimeElecteurCmd
}

void traitementEstPresent(EstPresentCmd *cmd) {
    printf("Traitement EstPresentCmd\n");
    // Implémentation spécifique pour EstPresentCmd
}
// Thread pour le traitement des commandes
void* processCommands(void* arg) {
    while (1) {
        Commande *cmd = dequeueCommand();
        switch (cmd->type) {
            case AJOUT_ELECTEUR:
                traitementAjoutElecteur(&cmd->commande.ajoutElecteur);
                break;
            case SUPPRIME_ELECTEUR:
                traitementSupprimeElecteur(&cmd->commande.supprimeElecteur);
                break;
            case EST_PRESENT:
                traitementEstPresent(&cmd->commande.estPresent);
                break;
                // Ajoutez ici des cas pour les autres types de commandes
            default:
                printf("Type de commande non reconnu\n");
        }
        free(cmd);
    }
    return NULL;
}