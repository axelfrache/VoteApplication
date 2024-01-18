#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../common/include//messages.h"
#include "./../common/include/bd.h"
#include <pthread.h>

extern void enqueueCommand(Commande*);
extern void* processCommands(void* arg);

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

int main(int argc, char *argv[]) {

    pthread_t threadReceiver, threadProcessor;

    // Initialisation de la base de données
    const char *dbPath = "../data_base/base_de_donnees.db";
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
