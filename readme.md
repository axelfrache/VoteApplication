# Projet de vote

Cette branche se concentre sur la partie crypto du projet.

Pour installer gmp.h
apt-get install  libgmp3-dev

Commande pour lancer le test de crypto :

```bash
gcc test_crypto.c ./../src/crypto.c -o crypto -lgmp
```

Pour la cryptographie des votes :

Dans Election_castVote :

Le choix de vote (choix) est converti en un nombre entier grand format (mpz_t) en utilisant la bibliothèque GMP.
Ce nombre est ensuite chiffré à l'aide de la fonction encrypt, qui utilise les clés publiques (n et g) pour produire le texte chiffré c.
Le texte chiffré c est converti en chaîne de caractères et stocké dans la base de données comme le bulletin de vote chiffré.

Dans Election_processVotes :

Pour chaque vote récupéré de la base de données, la chaîne représentant le vote chiffré est convertie en un nombre entier grand format (mpz_t).
Le vote chiffré est déchiffré en utilisant la fonction decrypt, qui utilise les clés privées (lambda et mu) pour retrouver le vote original m.
Les votes déchiffrés sont ensuite comptabilisés en fonction de leur contenu (par exemple, 'O' pour oui).

Notes personnelles :

Pour lancer le test du prof :

Il faut d'abord "make" dans le dossier src pour compiler les fichiers .c

Puis aller dans le dossier test et faire "make" pour compiler les fichiers .c

Enfin, lancer le test avec la commande :

./test_electeur --file ../data_base/base_de_donnees.db
