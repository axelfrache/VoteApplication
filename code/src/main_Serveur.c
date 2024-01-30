#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/include//messages.h"
#include "../common/include/bd.h"
#include "../common/include/util.h"
#include <pthread.h>

extern void enqueueCommand(Commande*);
extern void* processCommands(void* arg);
 

// Thread pour la réception des commandes
// Client temporaire pour la démonstration
void* receiveCommands(void* arg) {
    int commandNumber;
    char identifiant[ENTITY_ID_SIZE]; // Assurez-vous que ENTITY_ID_SIZE est défini correctement
    char* commandList[] = {
                "AJOUT_ELECTEUR",
                "SUPPRIME_ELECTEUR",
                "LIRE_ELECTEUR",
                "MODIFIER_ELECTEUR",
                "EST_PRESENT",
                "CREER_ELECTION",
                "MODIFIER_ELECTION",
                "LIRE_ELECTION",
                "SUPPRIMER_ELECTION",
                "RESULTAT_ELECTION",
                "VOTER",
                "VALIDER_VOTE"
            };
    CommandType allCommands[] = {
        AJOUT_ELECTEUR,
        SUPPRIME_ELECTEUR,
        LIRE_ELECTEUR,
        MODIFIER_ELECTEUR,
        EST_PRESENT,
        CREER_ELECTION,
        MODIFIER_ELECTEUR,
        LIRE_ELECTION,
        SUPPRIMER_ELECTION,
        RESULTAT_ELECTION,
        VOTER,
        VALIDER_VOTE
    };
    while (1) {
        Commande *cmd = malloc(sizeof(Commande));
        memset(cmd, 0, sizeof(Commande));
        printf("Entrez une commande: \n");
        for (int i = 0; i < COMMAND_NUMBER; i++){
            notif(YELLOW, "%d: %s", i, commandList[i]);
        }
        notif(BLUE, "Test");
        // Traitement succésif (Zero Nester) d'erreurs
        if (scanf("%d", &commandNumber) != 1) {
            break;
        }
        if (!cmd) {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            continue;
        }
        
        if(commandNumber > COMMAND_NUMBER - 1){
            notif(RED,"Commande non reconnue\n");
            free(cmd);
            continue;
        }
        // Gestion des commandes 
        cmd->type = allCommands[commandNumber];
        notif(BLUE, "Entrez l'identifiant : ");
        if(fgets(identifiant, ENTITY_ID_SIZE, stdin) == NULL){
            err_n_die("Error reading input with fgets in main_Serveur");
        }
        strncpy(cmd->commande.ajoutElecteur.identifiant, identifiant, ENTITY_ID_SIZE-1);
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
