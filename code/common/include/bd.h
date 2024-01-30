#ifndef BD__
#define BD__

#include <sqlite3.h>
#include <gmp.h>


sqlite3 *database_open(const char *path);
int database_close(sqlite3 *db);
int database_init(sqlite3 *db);
int database_exists(const char *path);

// usecases electeur
void createElecteur(sqlite3 *db, const char *numeroID, int size);
int electeurExists(sqlite3 *db, const char *numeroID, int size);
void deleteElecteur(sqlite3 *db, const char *numeroID, int size);
void updateElecteur(sqlite3 *db, const char *oldNumeroID, int size1, const char *newNumeroID, int size2);
void readElecteur(sqlite3 *db, const char *numeroID, int size);
int getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);

// usecases election
void createElection(sqlite3 *db, const char *identifiant, int sizeId, const char *question, const char *dateDebut, const char *dateFin, const char *status);
void deleteElection(sqlite3 *db, int id);
void updateElection(sqlite3 *db, int id, const char *question);
void readElection(sqlite3 *db, int id);
int Election_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);

// bd.h
void Election_castVote(sqlite3 *db, int idVotant, int idElection, const char *choix, mpz_t n, mpz_t g);
void Election_processVotes(sqlite3 *db, int electionId, mpz_t lambda, mpz_t mu, mpz_t n);

#endif