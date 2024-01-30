#include "../common/include/bd.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../common/include/crypto.h"


const char *electeur_create = "CREATE TABLE IF NOT EXISTS Electeur(id INTEGER PRIMARY KEY, numeroID BLOB);";

const char *election_create = "CREATE TABLE IF NOT EXISTS Election(\
    id INTEGER PRIMARY KEY, \
    identifiant BLOB ,\
    question TEXT CHECK(length(question) <= 256),\
    dateDebut TEXT, \
    dateFin TEXT, \
    status TEXT CHECK(status IN('canceled', 'active', 'closed')));\
                              ";

const char *vote_create = "CREATE TABLE IF NOT EXISTS Vote(id INTEGER PRIMARY KEY,\
    idVotant INTEGER,\
    idElection INTEGER,\
    timestamp TEXT,\
    ballot BLOB ,\
    hashValidation TEXT CHECK(length(hashValidation) <= 256) );";

int database_exists(const char *path)
{
    struct stat buffer;
    return (stat(path, &buffer));
}

sqlite3 *database_open(const char *path)
{
    sqlite3 *db = NULL;
    int rc = sqlite3_open(path, &db);
    if (rc != SQLITE_OK)
    {
        // Gérer l'erreur
        sqlite3_close(db);
        return NULL;
    }
    return db;
};

int database_close(sqlite3 *db)
{
    return (sqlite3_close(db) == SQLITE_OK) ? 0 : 1;
};

int database_init(sqlite3 *db)
{
    printf("database_init\n");

    const char *requests[3] = {electeur_create, election_create, vote_create};
    int i;
    for (i = 0; i < 3; i++)
    {
        const char *sql = requests[i];
        char *errMsg = 0;
        int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK)
        {
            // Gérer l'erreur
            if (errMsg)
            {
                sqlite3_free(errMsg);
            }
            sqlite3_close(db);
            return 1;
        }
    }
    return 0;
};

void createElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Electeur (numeroID) VALUES (?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        const char *data = numeroID;
        // int size = strlen(data) + 1; // +1 pour le caractère nul de fin

        sqlite3_bind_blob(stmt, 1, data, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur ajouté avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void readElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Electeur WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const char *id = sqlite3_column_blob(stmt, 0);
            printf("Electeur: %s\n", id);
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int electeurExists(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Electeur WHERE numeroID = ?;";
    int result = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result = sqlite3_column_int(stmt, 0) > 0;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return result;
}

void updateElecteur(sqlite3 *db, const char *oldNumeroID, int size1, const char *newNumeroID, int size2)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Electeur SET numeroID = ? WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, newNumeroID, size1, SQLITE_STATIC);
        sqlite3_bind_blob(stmt, 2, oldNumeroID, size2, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de la mise à jour: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur mis à jour avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void deleteElecteur(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Electeur WHERE numeroID = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            printf("Erreur lors de la suppression: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Electeur supprimé avec succès\n");
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Electeur WHERE numeroID = ?;";
    int id = -1; // Valeur par défaut si l'ID n'est pas trouvé

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0); // Récupère l'ID de la première colonne
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return id;
}

void createElection(sqlite3 *db, const char *identifiant, int sizeId, const char *question, const char *dateDebut, const char *dateFin, const char *status)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Election (identifiant, question, dateDebut, dateFin, status) VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, identifiant, sizeId, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, question, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, dateDebut, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, dateFin, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, status, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int Election_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM Election WHERE identifiant = ?;";
    int id = -1; // Valeur par défaut si l'ID n'est pas trouvé

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_blob(stmt, 1, numeroID, size, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0); // Récupère l'ID de la première colonne
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    return id;
}

void readElection(sqlite3 *db, int id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Election WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // Supposons que id est la première colonne
            int id = sqlite3_column_int(stmt, 0);
            printf("Election: %d\n", id);
            // Affiche la question de l'élection
            const char *question = sqlite3_column_text(stmt, 2);
            printf("Question: %s\n", question);
            // Affiche le status de l'élection
            const char *status = sqlite3_column_text(stmt, 5);
            printf("Status: %s\n", status);
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void updateElection(sqlite3 *db, int id, const char *question)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Election SET question = ? WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, question, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de la mise à jour: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

void deleteElection(sqlite3 *db, int id)
{
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Election WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            fprintf(stderr, "Erreur lors de la suppression: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

// usecases election

void Election_castVote(sqlite3 *db, int idVotant, int idElection, const char *choix, mpz_t n, mpz_t g) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Vote (idVotant, idElection, timestamp, ballot) VALUES (?, ?, datetime('now'), ?);";

    // Convertir le choix en un grand nombre
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    mpz_set_ui(m, (unsigned long int)choix[0]);  // Assumer que le choix est une simple valeur pour la démonstration

    // Chiffrer le choix
    encrypt(c, m, n, g);

    // Convertir le texte chiffré en chaîne pour le stockage
    char *c_str = mpz_get_str(NULL, 10, c);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, idVotant);
        sqlite3_bind_int(stmt, 2, idElection);
        sqlite3_bind_text(stmt, 3, c_str, -1, SQLITE_TRANSIENT);  // SQLITE_TRANSIENT pour que SQLite fasse une copie

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Erreur lors de l'insertion: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Vote ajouté avec succès\n");
        }

        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    // Libérer la mémoire
    mpz_clears(m, c, NULL);
    free(c_str);
}

void Election_processVotes(sqlite3 *db, int electionId, mpz_t lambda, mpz_t mu, mpz_t n) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Vote WHERE idElection = ?;";
    int totalVotes = 0, option0 = 0, option1 = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, electionId);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char *c_str = (const char *)sqlite3_column_text(stmt, 4); // ballot
            mpz_t c, m;
            mpz_inits(c, m, NULL);

            // Convertir la chaîne chiffrée en grand nombre
            mpz_set_str(c, c_str, 10);

            // Déchiffrer le vote
            decrypt(m, c, lambda, mu, n);

            // Traiter le vote déchiffré
            totalVotes++;
            if (mpz_cmp_ui(m, 'O') == 0) {  // Assumer 'O' pour oui
                option1++;
            } else {
                option0++;
            }

            // Libérer la mémoire
            mpz_clears(c, m, NULL);
        }

        printf("Total votes: %d\nOption 0: %d\nOption 1: %d\n", totalVotes, option0, option1);

        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }
}

int hasUserAlreadyVoted(sqlite3 *db, int idVotant, int idElection) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Vote WHERE idVotant = ? AND idElection = ?;";
    int result = 0;

    // Préparation de la requête SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Liaison des paramètres à la requête
        sqlite3_bind_int(stmt, 1, idVotant);
        sqlite3_bind_int(stmt, 2, idElection);

        // Exécution de la requête
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Récupération du résultat (le nombre de lignes correspondant à la condition)
            result = sqlite3_column_int(stmt, 0);
        }

        // Libération de la mémoire associée à l'objet statement
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Erreur de préparation: %s\n", sqlite3_errmsg(db));
    }

    // Si le résultat est supérieur à 0, alors un vote existe déjà pour cet utilisateur et cette élection
    return result > 0;
}

