# Projet de Système de Vote

## Présentation du Projet

Ce projet simule un système de vote électronique. Il est conçu autour du modèle producteur/consommateur, utilisant des threads pour gérer les commandes dans une file d'attente. Les commandes sont déposées dans un tableau circulaire partagé et traitées par des threads consommateurs.

Ce projet a été réalisé par :

- **FRACHE Axel**
- **GONFIANTINI Gaëtan**
- **JIMENEZ Alfonso**

## Rappel des besoins

Le projet vise à élaborer un système de vote électronique garantissant l'authenticité et la justesse des votes, avec un décompte complet et transparent des résultats, tout en assurant l'accessibilité à tous les électeurs éligibles. Les fonctionnalités essentielles comprennent une gestion solide des électeurs et des élections, avec des opérations CRUD pour les deux, ainsi qu'un système de vote sécurisé et un comptage fiable des résultats. Le système met un accent particulier sur la validation des données, les mesures de sécurité pour prévenir les actions non autorisées, et les mécanismes pour vérifier l'éligibilité des électeurs et l'intégrité des votes.


Pour résumer les besoins du projet de système de vote dans votre README, vous pourriez inclure les points suivants :

- **Authenticité des votes** : Assurer que chaque vote est authentique et provient d'un électeur éligible.
- **Intégrité et transparence** : Garantir la justesse du décompte des votes et la transparence des résultats.
- **Accessibilité** : Permettre à tous les électeurs éligibles de participer facilement au vote.
- **Sécurité** : Mettre en place des mesures de sécurité robustes pour prévenir les actions non autorisées et protéger les données.
- **Vérification** : Disposer de mécanismes pour vérifier l'éligibilité des électeurs et l'intégrité des votes.

## Structure du projet

La structure du projet est organisée en plusieurs dossiers clés pour une séparation claire des composants et une meilleure maintenabilité :

- **build/** : Contient les exécutables et les binaires, y compris les tests unitaires, générés à partir du code source.

- **common/include/** : Regroupe les en-têtes communs, incluant les définitions pour la base de données et les fonctions de cryptographie, facilitant la réutilisation du code.

- **data_base/** : Héberge la base de données SQLite, centralisant les données des électeurs, élections et votes.

- **src/** : Contient les fichiers sources du projet, avec le code principal et le Makefile pour la compilation.

- **test/** : Dédie un espace aux tests unitaires, permettant de valider la fiabilité des différentes composantes du système.


Voici en image la structure complète du projet :

![imageStructureDuProjet.png](imgREADME%2FimageStructureDuProjet.png)

## Logique de programmation

La logique de programmation de ce système de vote est conçue pour garantir la fluidité, la sécurité et l'efficacité du traitement des commandes. Voici les principaux éléments :

- **Gestion de la base de données** : Le programme gère automatiquement la base de données située dans le dossier "data_base". Il crée la base de données si elle n'existe pas et la charge au démarrage du système, assurant ainsi l'intégrité et la disponibilité des données.


- **Tableau circulaire** : Nous utilisons un tableau circulaire pour stocker les commandes dans une file d'attente. Cette structure de données optimise l'utilisation de l'espace mémoire et facilite le traitement séquentiel des commandes sans perte de performance.


- **Threading** : Le système emploie une architecture multithread pour séparer la réception des commandes de leur traitement. Cela permet de réduire les temps d'attente et d'améliorer la réactivité du système face aux sollicitations simultanées.


- **Synchronisation** : Des mutex et des variables conditionnelles sont utilisés pour protéger les accès concurrents aux ressources partagées, notamment le tableau circulaire des commandes. Cette approche prévient les conditions de course et assure la cohérence des données à travers les différents threads.


- **Traitement des commandes** : Le système prend en charge diverses commandes pour la gestion des électeurs, des élections et des votes. Chaque commande est encapsulée dans une structure unifiée, permettant un traitement générique et flexible des différentes actions requises.

Cette architecture permet au système de vote de gérer efficacement un grand nombre de commandes tout en garantissant la sécurité et l'intégrité des opérations effectuées.


## Logique des tests

Concernant les tests nous avons mis en place une stratégie de tests.

Nous avons 3 types de tests répartis dans 3 dossier différents :

- **Tests des fonctions de bd.c** : Ces tests ont étaient fournis par le professeur et permettent de tester les fonctions de la base de données.
- **Tests des fonctions de traitement des commandes** : Ces tests permettent de tester les fonctions de traitement des commandes, notamment les CRUD des électeurs, élections et votes.
- **Tests de libraire** : Ces tests permettent de tester les fonctions de la librairie, notamment les librairies de cryptographie et de base de données.

## Cryptographie des votes

La sécurité des votes est assurée par cryptographie, utilisant la bibliothèque GMP. Les fonctions clés sont **Election_castVote** pour le chiffrement des votes et **Election_processVotes** pour le déchiffrement et le comptage.
Ces fonctions se trouvent dans le fichier "bd.c" et permettent de chiffrer les votes lorsqu'on en ajoute un et de déchiffrer les votes lorsqu'on veut consulter les résultats d'un vote.

Notre stratégie est de chiffré les votes lors de l'ajout d'un vote et de les déchiffrer lors de la consultation des résultats d'un vote.

Je sais que vous pourrez faire la remarque dans le future que si il y a un client/server, le trajet des votes se feront en clair donc nous pensons par la suite mettre en place des communications sécurisées entre le client et le serveur pour palier ce problème.
Le but de notre stratégie est que si un attaquant arrive à rentrer dans le serveur, il ne pourra pas voir les votes en clair.

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


