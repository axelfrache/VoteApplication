#ifndef MESSAGE_H
#define MESSAGE_H

#include "protocol.h"

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} AjoutElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} SupprimeElecteurCmd;

typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} EstPresentElecteurCmd;
typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} AjoutVoteurCmd;
typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} SupprimeVoteurCmd;
typedef struct
{
    char identifiant[ENTITY_ID_SIZE];
} EstPresentVoteurCmd;


typedef enum
{
    NOP = 0,
    AJOUT_ELECTEUR,
    SUPPRIME_ELECTEUR,
    EST_PRESENT
} CommandType;

//--
typedef struct
{
    CommandType type;
    char signature[256]; // la signature de la commande
    union
    {
        AjoutElecteurCmd ajoutElecteur;
        SupprimeElecteurCmd supprimeElecteur;
        EstPresentElecteurCmd estPresentElecteur;
        AjoutVoteurCmd ajoutVoteur;
        SupprimeVoteurCmd supprimeVoteur;
        EstPresentVoteurCmd estPresentVoteur;
    } commande;
} Commande;

#endif
