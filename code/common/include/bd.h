#ifndef BD__
#define BD__

#include <sqlite3.h>
#include <gmp.h>


sqlite3 *database_open(const char *path);
int database_close(sqlite3 *db);
int database_init(sqlite3 *db);
int database_exists(const char *path);
int clear_database(sqlite3 *db);

// usecases electeur
void createElecteur(sqlite3 *db, const char *numeroID, int size);
int electeurExists(sqlite3 *db, const char *numeroID, int size);
void deleteElecteur(sqlite3 *db, const char *numeroID, int size);
void updateElecteur(sqlite3 *db, const char *oldNumeroID, int size1, const char *newNumeroID, int size2);
void readElecteur(sqlite3 *db, const char *numeroID, int size);
int getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);

// usecases election
void createElection(sqlite3 *db, const char *identifiant, int sizeId, const char *question, const char *dateDebut, const char *dateFin, const char *status);
void readElectionByIdentifiant(sqlite3 *db, const char *identifiant, int size);
void updateElectionByIdentifiant(sqlite3 *db, const char *identifiant, int sizeId, const char *newQuestion);
int Election_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);
int Electeur_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);

void deleteElectionByIdentifiant(sqlite3 *db, const char *identifiant, int identifiantSize);

int electionExists(sqlite3 *db, const char *identifiant, int size);


// usecase vote
void Election_castVote(sqlite3 *db, int idVotant, int idElection, const char *choix, mpz_t n, mpz_t g);
void Election_processVotes(sqlite3 *db, int electionId, mpz_t lambda, mpz_t mu, mpz_t n);
void afficheQuestionVote(sqlite3 *db, int idElection);

int hasUserAlreadyVoted(sqlite3 *db, int idVotant, int idElection);

#endif