# Projet de vote


## Présention du projet

Ce projet est un projet qui simule un serveur de vote. 

Il repose sur le système de producteur/consommateur. 

Nous avons mis en place un système de thread qui permettent de mettre des commandes dans une file d'attente
et d'autres threads qui vont récupérer ces commandes et les traiter.

Il y a également uns système de ressources partagées via un tableau circulaire qui permet de déposer des commandes
et de les récupérer.

## Patron commande

Ce projet repose essentiellement sur le patron de conception commande.

Voici les commandes que nous avons implémentées dans ce projet :

Pour les électeurs : 

- Créer un électeur
- Lire un électeur
- Mettre à jour un électeur
- Supprimer un électeur

Pour les élections : 

- Créer une élection
- Lire une élection
- Mettre à jour une élection
- Supprimer une élection

Pour les votes :

- Créer un vote
- Lire un vote

## Fonctionnement de notre application

Comme dans le projet il y avait déja des tests nous avons continué à creer des fichiers de tests pour tester nos commandes.

Pour se faire vous devez dans un premier aller dans le dossier "src" ouvrez un terminal et lancer la commande "make" pour compiler les fichiers .c

Ensuite vous devez aller dans le dossier "test" et lancer la commande "make" pour compiler les fichiers .c

Enfin rendez vous dans le dossier "build" et lancer les fichiers de test avec la commande "./nom_du_fichier"

## Notre logique de programmation

Commencons par la logique du CRUD d'un électeur : 



















--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Cette branche se concentre sur la partie crypto du projet.


Pour installer gmp.h : 
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

