# Projet de vote

Cette branche se concentre sur la partie crypto du projet.


Pour installer gmp.h
apt-get install  libgmp3-dev

Commande pour lancer le test de crypto : 

```bash
gcc test_crypto.c ./../src/crypto.c -o crypto -lgmp
```



Notes personnelles : 

Pour lancer le test du prof : 

Il faut d'abord "make" dans le dossier src pour compiler les fichiers .c

Puis aller dans le dossier test et faire "make" pour compiler les fichiers .c

Enfin, lancer le test avec la commande :

./test_electeur --file ../data_base/base_de_donnees.db

