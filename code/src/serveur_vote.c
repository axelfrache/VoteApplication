#include "./../common/include//messages.h"
#include "bd.c"
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

// Thread pour la réception des commandes
// Client temporaire pour la démonstration
void* receiveCommands(void* arg) {
    char buffer[1024];

    while (1) {
        printf("Entrez une commande: \n");
        if (fgets(buffer, 1024, stdin) == NULL) {
            break; // Sortie de la boucle si erreur ou fin de fichier
        }

        Commande *cmd = malloc(sizeof(Commande));
        memset(cmd, 0, sizeof(Commande)); // Initialisation de la commande

        // Exemple de décodage de la commande à partir de l'entrée
        // NOTE: Ceci est un exemple très simplifié pour démontrer le concept
        if (strncmp(buffer, "AJOUT", 5) == 0) {
            cmd->type = AJOUT_ELECTEUR;
            strcpy(cmd->commande.ajoutElecteur.identifiant, "Identifiant_Electeur");
        } else if (strncmp(buffer, "SUPPRIME", 8) == 0) {
            cmd->type = SUPPRIME_ELECTEUR;
            strcpy(cmd->commande.supprimeElecteur.identifiant, "Identifiant_Electeur");
        } else if (strncmp(buffer, "PRESENT", 7) == 0) {
            cmd->type = EST_PRESENT;
            strcpy(cmd->commande.estPresent.identifiant, "Identifiant_Electeur");
        } else {
            printf("Commande non reconnue\n");
            continue;
        }


        enqueueCommand(cmd);

    }

    return NULL;
}

/**
 * Fonctions de traitement des commandes
 */
void traitementAjoutElecteur(AjoutElecteurCmd *cmd) {
    printf("Traitement AjoutElecteurCmd\n");
    // Implémentation spécifique pour AjoutElecteurCmd
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
        free(cmd); // Ajouter cette ligne après le traitement de la commande
    }
    return NULL;
}


int main(int argc, char *argv[]) {

    pthread_t threadReceiver, threadProcessor;

    // Initialisation de la base de données
    const char *dbPath = "./../data_base/base_de_donnees.db";
    sqlite3 *db;

    // Ouvrir ou créer la base de données si elle n'existe pas
    db = database_open(dbPath);
    if (!db) {
        fprintf(stderr, "Impossible d'ouvrir ou de créer la base de données.\n");
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
