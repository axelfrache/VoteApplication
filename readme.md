# Projet de Système de Vote

## Présentation du Projet

Ce projet simule un système de vote électronique. Il est conçu autour du modèle producteur/consommateur, utilisant des threads pour gérer les commandes dans une file d'attente. Les commandes sont déposées dans un tableau circulaire partagé et traitées par des threads consommateurs.

## Rappel des besoins




## Patron de Conception

Le projet s'appuie sur le patron de conception Commande, avec les commandes suivantes implémentées :

### Pour les Électeurs :
- Création d'un électeur
- Lecture des informations d'un électeur
- Mise à jour des informations d'un électeur
- Suppression d'un électeur

### Pour les Élections :
- Création d'une élection
- Lecture des informations d'une élection
- Mise à jour des informations d'une élection
- Suppression d'une élection

### Pour les Votes :
- Enregistrement d'un vote
- Consultation des résultats d'un vote


## Utilisation de l'Application

### Prérequis

Pour pouvoir utiliser l'application, vous devez avoir installé les paquets suivant : 

- GMP pour la cryptographie des votes
````bash
sudo apt-get install libgmp3-dev
````
- SQLite pour la base de données
````bash
sudo apt update
sudo apt install sqlite3
sudo apt install libsqlite3-dev
sqlite3 --version
````
- glib pour les différentes structures de données
````bash
sudo apt update
sudo apt install libglib2.0-dev
pkg-config --modversion glib-2.0
````

Il existe deux manières d'utiliser l'application :

### 1. Exécution Directe :

Cette manière d'utiliser l'application permet d'intéragir directemment avec l'application via le terminal.
Cette méthode sera présenté sous forme de menu et de sous-menu.

Dans les menus, seront affichés électeurs, élections et votes. Chacun de ces menus permettra d'accéder à un sous-menu permettant de créer, lire, mettre à jour ou supprimer un élément.

Voici un apperçu du menu et d'un sous-menu :

Le menu principal :

![imageMenuPrincipal.png](imgREADME%2FimageMenuPrincipal.png)

Le sous menu des électeurs :

![imageSousMenuElecteur.png](imgREADME%2FimageSousMenuElecteur.png)

Pour utiliser l'application de cette manière vous devez : 

Vous rendre dans le dossier "**src**" et lancer le fichier make :

```bash
cd src
make main

```
Ensuite rendez-vous dans le dossier "**build**" et lancez le fichier main:

```bash
cd ../build
./main
```

Maintenant vous n'avez plus qu'à suivre les instructions du menu.
Des affichages vous indiqueront si les commandes ont été exécutées avec succès ou non.
Ils vous indiqueront également l'état actuel du buffer qui contient les commandes quand il y a une commande qui a été ajoutée traitée ou supprimée.

### 2. Tests : 

Concernant cette deuxième manière d'utiliser l'application, elle permet de tester les fonctions de traitement des commandes.
Pour cela, nous avons mis en place des tests unitaires qui permettent de tester les fonctions de traitement des commandes.

Afin de pouvoir utiliser les tests, vous devez :

Vous rendre dans le dossier "**src**" et lancer le fichier make :
```bash
cd src
make
```

Ensuite rendez-vous dans le dossier "**test**" et lancez le fichier make:

```bash

cd ../test
make
```

Enfin, exécutez les tests depuis le dossier "build" :

```bash
cd ../build
./nom_du_test
```

Voici la liste de toutles tests disponibles :

![imageListeTestDisponible.png](imgREADME%2FimageListeTestDisponible.png)


## Logique de programmation




Un point important à aborder c'est la base de données qui se trouve dans le dossier "data_base" et l'utilisateur n'a pas besoin de s'en occuper, car le programme s'en occupe tout seul.
Le programme s'en occupe dans tous les cas possibles : 
- Si la base de données n'existe pas, le programme la crée.
- Si la base de données existe, le programme la charge (et la vide).



## Logique des tests

Concernant les tests nous avons mis en place une stratégie de tests

## Cryptographie des votes

La sécurité des votes est assurée par cryptographie, utilisant la bibliothèque GMP. Les fonctions clés sont **Election_castVote** pour le chiffrement des votes et **Election_processVotes** pour le déchiffrement et le comptage.
Ces fonctions se trouvent dans le fichier "bd.c" et permettent de chiffrer les votes lorsqu'on en ajoute un et de déchiffrer les votes lorsqu'on veut consulter les résultats d'un vote.

Notre stratégie est de chiffré les votes lors de l'ajout d'un vote et de les déchiffrer lors de la consultation des résultats d'un vote.

Je sais que vous pourrez faire la remarque dans le future que si il y a un client/server, le trajet des votes se feront en clair donc nous pensons par la suite mettre en place des communications sécurisées entre le client et le serveur pour palier ce problème.
Le but de notre stratégie est que si un attaquant arrive à rentrer dans le serveur, il ne pourra pas voir les votes en clair.



Installation de GMP :

````bash
apt-get install libgmp3-dev
````


