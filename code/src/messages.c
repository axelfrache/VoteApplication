#include "../common/include/messages.h"
#include "../common/include/bd.h"
#include "../common/include/util.h"

/**
 * Fonctions de traitement des commandes
 */
void traitementAjoutElecteur(AjoutElecteurCmd *cmd) {
    notif(GREEN,"Traitement AjoutElecteurCmd");
    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        printf("Commande invalide ou identifiant manquant.\n");
        return;
    }

    // Ouvrir la base de données (assurez-vous que le chemin est correct)
    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        err_n_die("Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Appeler createElecteur
    createElecteur(db, cmd->identifiant, ENTITY_ID_SIZE); // ENTITY_ID_SIZE est la taille de l'identifiant.

    // Fermer la base de données
    sqlite3_close(db);
}

void traitementLireElecteur(LireElecteurCmd *cmd){
    printf("LireElection");
    

}

void traitementModifierElecteur(ModifierElecteurCmd *cmd){

}

void traitementSupprimeElecteur(SupprimeElecteurCmd *cmd) {
    notif(GREEN, "Traitement SupprimeElecteurCmd");
    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        notif(RED,"Commande invalide ou identifiant manquant.");
        return;
    }

    // Ouvrir la base de données
    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        err_n_die("Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Appeler deleteElecteur
    deleteElecteur(db, cmd->identifiant, ENTITY_ID_SIZE);

    // Fermer la base de données
    sqlite3_close(db);
}

void traitementEstPresent(EstPresentCmd *cmd) {
    notif(GREEN, "Traitement EstPresentCmd");
    if (cmd == NULL || cmd->identifiant[0] == '\0') {
        notif(RED,"Commande invalide ou identifiant manquant.");
        return;
    }

    // Ouvrir la base de données
    sqlite3 *db;
    if (sqlite3_open("../data_base/base_de_donnees.db", &db) != SQLITE_OK) {
        err_n_die("Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Vérifier si l'électeur existe
    int exists = electeurExists(db, cmd->identifiant, ENTITY_ID_SIZE);
    if (exists) {
        notif(GREEN,"L'électeur est présent dans la base de données.");
    } else {
        printf(CYAN, "L'électeur n'est pas présent dans la base de données.");
    }

    // Fermer la base de données
    sqlite3_close(db);
}


/* Gestion des commandes Liées à Election */
void traitementCreerElection(CreerElectionCmd *cmd){

}

void traitementModifierElection(ModifierElectionCmd *cmd){

}
void traitementLireElection(LireElectionCmd *cmd){

}
void traitementSupprimerElection(SupprimerElectionCmd *cmd){
    
}
