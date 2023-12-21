#ifndef MESSAGE_H
#define MESSAGE_H

#include "protocol.h"

typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} AjoutElecteurCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} SupprimeElecteurCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} LireElecteurCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} ModifierElecteurCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} EstPresentCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} CreerElectionCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} ModifierElectionCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} LireElectionCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} SupprimerElectionCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} ResultatElectionCmd;
typedef struct {
    char identifiantVotant[ENTITY_ID_SIZE];
    char identifiantElection[ENTITY_ID_SIZE];
    char bulletin[ENTITY_ID_SIZE];
} VoterCmd;
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} ValiderVoteCmd;

typedef enum {
    NOP = 0,
    /* Commandes pour les électeurs */

    AJOUT_ELECTEUR, // Commande pour ajouter un électeur
    SUPPRIME_ELECTEUR, // Commande pour supprimer un électeur
    LIRE_ELECTEUR, //Commande pour lire les informations d'un électeur
    MODIFIER_ELECTEUR, //Commande pour modifier un électeur

    EST_PRESENT, //Commande qui vérifie si un électeur est présent dans la liste des électeurs

    /* Commandes pour les élections */

    CREER_ELECTION,
    MODIFIER_ELECTION,
    LIRE_ELECTION,
    SUPPRIMER_ELECTION,

    RESULTAT_ELECTION,

    /* Commandes pour la gestion des votes */
    VOTER,
    VALIDER_VOTE
} CommandType;

//--
typedef struct {
    CommandType type;
    char signature[256]; // la signature de la commande
    union {
        //Electeur
        AjoutElecteurCmd ajoutElecteur;
        ModifierElectionCmd modifierElection;
        LireElecteurCmd lireElecteur;
        SupprimeElecteurCmd supprimeElecteur;

        EstPresentCmd estPresent;

        //Election
        CreerElectionCmd creerElection;
        ModifierElectionCmd modifierElecteur;
        LireElectionCmd lireElection;
        ModifierElecteurCmd modifierElecteurCmd;
        SupprimerElectionCmd supprimerElection;
        ResultatElectionCmd relutatElection;
        // Vote
        VoterCmd voter;
        ValiderVoteCmd validerVote;

    } commande;
} Commande;

#endif
