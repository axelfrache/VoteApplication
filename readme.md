# Projet de Système de Vote

## Présentation du Projet

Ce projet simule un système de vote électronique. Il est conçu autour du modèle producteur/consommateur, utilisant des threads pour gérer les commandes dans une file d'attente. Les commandes sont déposées dans un tableau circulaire partagé et traitées par des threads consommateurs.

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

Il existe deux manières d'utiliser l'application :

### 1. Exécution Directe :

Compilez le projet avec la commande :

```bash
make main
```
Naviguez ensuite dans le dossier "build" et exécutez le programme principal :

```bash
./main
```

### 2. Tests : 

Pour tester les différentes commandes, compilez d'abord les fichiers sources :

```bash
cd src
make
```

Puis compilez les tests :

```bash

cd ../test
make
```

Enfin, exécutez les tests depuis le dossier "build" :

```bash
cd ../build
./nom_du_test
```

## Logique de programmation



## Cryptographie des votes

La sécurité des votes est assurée par cryptographie, utilisant la bibliothèque GMP. Les fonctions clés sont Election_castVote pour le chiffrement des votes et Election_processVotes pour le déchiffrement et le comptage.

Installation de GMP :

````bash
apt-get install libgmp3-dev
````


