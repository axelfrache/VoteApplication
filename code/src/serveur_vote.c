#include "./../common/include//messages.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_COMMANDS 100

// Structure du tableau circulaire pour les commandes
Commande commandQueue[MAX_COMMANDS];
int queueStart = 0;
int queueEnd = 0;

// Mutex et variables conditionnelles pour la synchronisation
pthread_mutex_t mutexQueue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condQueueNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t condQueueNotFull = PTHREAD_COND_INITIALIZER;

// Ajouter une commande dans le tableau circulaire
void enqueueCommand(Commande cmd) {
    pthread_mutex_lock(&mutexQueue);

    while ((queueEnd + 1) % MAX_COMMANDS == queueStart) {
        // Attendre qu'il y ait de la place dans le tableau
        pthread_cond_wait(&condQueueNotFull, &mutexQueue);
    }

    commandQueue[queueEnd] = cmd;
    queueEnd = (queueEnd + 1) % MAX_COMMANDS;

    pthread_cond_signal(&condQueueNotEmpty);
    pthread_mutex_unlock(&mutexQueue);
}

// Récupérer une commande du tableau circulaire
Commande dequeueCommand() {
    pthread_mutex_lock(&mutexQueue);

    while (queueStart == queueEnd) {
        // Attendre qu'une commande soit disponible
        pthread_cond_wait(&condQueueNotEmpty, &mutexQueue);
    }

    Commande cmd = commandQueue[queueStart];
    queueStart = (queueStart + 1) % MAX_COMMANDS;

    pthread_cond_signal(&condQueueNotFull);
    pthread_mutex_unlock(&mutexQueue);

    return cmd;
}

// Thread pour la réception des commandes
void* receiveCommands(void* arg) {
    while (1) {
        // Logique pour recevoir des commandes
        // Exemple : Commande cmd = recevoirCommande();
        // enqueueCommand(cmd);
    }
    return NULL;
}

// Thread pour le traitement des commandes
void* processCommands(void* arg) {
    while (1) {
        Commande cmd = dequeueCommand();
        // Logique de traitement des commandes
        // Exemple : processCommand(&cmd);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threadReceiver, threadProcessor;

    pthread_create(&threadReceiver, NULL, receiveCommands, NULL);
    pthread_create(&threadProcessor, NULL, processCommands, NULL);

    pthread_join(threadReceiver, NULL);
    pthread_join(threadProcessor, NULL);

    return 0;
}
