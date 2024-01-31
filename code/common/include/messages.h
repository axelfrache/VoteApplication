#ifndef MESSAGES_H
#define MESSAGES_H

#define ENTITY_ID_SIZE 256

// Enumération pour identifier le type de commande
typedef enum {
    AJOUT_ELECTEUR,
    LIRE_ELECTEUR,
    MODIFIER_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT,
    CREER_ELECTION,
    LIRE_ELECTION,
    MODIFIER_ELECTION,
    SUPPRIMER_ELECTION,
    CREER_VOTE,
    LIRE_VOTE,
} CommandType;

// Structure de commande pour ajouter un électeur
typedef struct {
    char numeroID[ENTITY_ID_SIZE];
} AjoutElecteurCmd;

// Structure de commande pour lire un électeur
typedef struct {
    char numeroID[ENTITY_ID_SIZE];
} LireElecteurCmd;

// Structure de commande pour modifier un électeur
typedef struct {
    char ancienNumeroID[ENTITY_ID_SIZE];
    char nouvelNumeroID[ENTITY_ID_SIZE];
} ModifierElecteurCmd;

// Structure de commande pour supprimer un électeur
typedef struct {
    char numeroID[ENTITY_ID_SIZE];
} SupprimeElecteurCmd;

// Structure de commande pour vérifier la présence d'un électeur
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} EstPresentCmd;

// Structure de commande pour créer une élection
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
    char question[ENTITY_ID_SIZE];
    char dateDebut[20];
    char dateFin[20];
    char status[10]; // Peut être 'active', 'closed', 'canceled'
} CreerElectionCmd;

// Structure de commande pour lire les détails d'une élection
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} LireElectionCmd;

// Structure de commande pour modifier une élection
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
    char nouvelleQuestion[ENTITY_ID_SIZE];
} ModifierElectionCmd;

// Structure de commande pour supprimer une élection
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
} SupprimerElectionCmd;



// Structure de commande pour créer un vote
typedef struct {
    char identifiant[ENTITY_ID_SIZE];
    char numeroID[ENTITY_ID_SIZE];
    char ballot[256];
    char hashValidation[256];
} CreerVoteCmd;


// Structure de commande pour lire un vote
typedef struct {
    int idElection;
} LireVoteCmd;

// Structure générale pour une commande
typedef struct {
    CommandType type;
    union {
        AjoutElecteurCmd ajoutElecteur;
        LireElecteurCmd lireElecteur;
        ModifierElecteurCmd modifierElecteur;
        SupprimeElecteurCmd supprimeElecteur;
        EstPresentCmd estPresent;
        CreerElectionCmd creerElection;
        LireElectionCmd lireElection;
        ModifierElectionCmd modifierElection;
        SupprimerElectionCmd supprimerElection;
        CreerVoteCmd creerVote;
        LireVoteCmd lireVote;
        // Ajouter plus de commandes selon les besoins
    } commande;
} Commande;

#endif // MESSAGES_H
