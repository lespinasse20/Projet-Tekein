# Projet TEIKEN - Application de Gestion et Passage de QCM

TEIKEN est une application console développée en langage C permettant à des enseignants de créer, modifier et supprimer des questionnaires à choix multiples (QCM), et à des étudiants de s'évaluer à travers ces tests. 

L'application intègre des fonctionnalités avancées comme la gestion par catégories, la notation partielle, la prise en compte des points négatifs, et un mode examen (séquentiel).

## 🚀 Fonctionnalités

### 👨‍🏫 Mode Enseignant (Sécurisé par mot de passe)
* Authentification : Accès protégé par un mot de passe administrateur (Configurable via une constante globale).
* Création / Modification de QCM :
  * Définition d'une catégorie personnalisée (ex: Informatique, Histoire...).
  * Configuration fine du barème (Points par question, activation/désactivation des points négatifs).
  * Choix du comportement du QCM (Choix unique ou choix multiples, mode classique ou mode examen séquentiel).
  * Création de 2 à 4 propositions par question avec choix des bonnes réponses.
* Suppression de QCM : Nettoyage et suppression directe des fichiers depuis l'application.
* Gestion du mot de passe : Possibilité de modifier le mot de passe admin en cours de session.

### 👨‍🎓 Mode Étudiant
* Navigation par Catégorie : Affichage clair et dynamique des QCM disponibles regroupés par thématiques.
* Flexibilité : Possibilité d'annuler ou de revenir au menu principal avant le lancement du questionnaire en entrant 0.
* Moteur de Réponse Flexible : Gestion des réponses multiples (ex: taper 13 pour cocher les propositions 1 et 3), gestion du passage de question (sauf en mode examen) et calcul automatique de la note finale ramenée sur 20 points.

---

## 📁 Structure du Projet

Le projet est découpé de manière modulaire afin de séparer les structures de données, le point d'entrée de l'application et la logique métier :

* structures.h : Contient les définitions des types personnalisés (configuration, question, qcm) ainsi que les limites globales (MAX_QUESTIONS, MAX_CHAINE).
* main.c : Point d'entrée de l'application. Il gère la boucle du menu principal et sécurise l'aiguillage vers les modes Enseignant ou Étudiant.
* fonctions.c : Regroupe l'intégralité de la logique de l'application (fonctions d'affichage, moteurs de saisies sécurisées, algorithme de calcul des scores et gestion des fichiers .txt).
* Makefile : Fichier de configuration automatisant la compilation du projet de manière hybride (compatible Windows et Linux).

---

## 🛠️ Compilation et Exécution sous Windows

Pour compiler et lancer le projet directement depuis l'invite de commandes Windows en utilisant le compilateur de Code::Blocks, suivez scrupuleusement ces étapes :

### Étape 1 : Ouvrir l'invite de commandes au bon endroit
1. Ouvrez l'explorateur de fichiers Windows et rendez-vous dans le dossier contenant vos fichiers sources (main.c, Makefile, fonctions.c...).
2. Cliquez directement dans la barre d'adresse tout en haut de l'explorateur (là où le chemin du dossier est écrit).
3. Effacez tout le texte présent, tapez "cmd" et appuyez sur la touche Entrée.
4. Une invite de commandes noire s'ouvre, automatiquement positionnée dans votre dossier de projet.

### Étape 2 : Configurer le chemin du compilateur (Path)
Pour que Windows trouve temporairement les outils de compilation de Code::Blocks dans la console ouverte, copiez-collez la commande suivante et validez avec Entrée :
set PATH=%PATH%;C:\Program Files\CodeBlocks\MinGW\bin

### Étape 3 : Compiler avec le Makefile
Lancez la compilation automatisée en exécutant la commande suivante :
mingw32-make

### Étape 4 : Lancer l'application
Pour exécuter votre programme, tapez simplement son nom dans la console et validez :
projet_teiken

---

## 🐧 Compilation et Exécution sous Linux

Si vous êtes sous Linux, ouvrez un terminal dans le dossier du projet et utilisez l'une des deux méthodes suivantes :

* **Avec le Makefile :**
  make

* **Manuellement avec GCC :**
  gcc main.c fonctions.c -o projet_teiken

Pour lancer l'application après la compilation, tapez :
./projet_teiken

---

## 🧹 Commandes de Nettoyage utiles

Le Makefile intègre une règle de nettoyage automatique pour vider les fichiers objets temporaires (.o) et l'ancien exécutable.

* Pour tout nettoyer proprement sous Windows :
  mingw32-make clean

* Pour tout nettoyer proprement sous Linux :
  make clean

* Pour recompiler à neuf immédiatement après un nettoyage :
  mingw32-make (Windows) ou make (Linux)

---

## 💾 Emplacement des fichiers QCM (.txt)
Lorsque vous compilez manuellement avec le Makefile, l'exécutable projet_teiken est créé directement à la racine de votre dossier de travail. 

IMPORTANT : Pour que vos questionnaires existants soient visibles dans le mode Étudiant, assurez-vous que vos fichiers de QCM (ex: Histoire.txt) soient situés dans le même dossier que l'exécutable projet_teiken (à la racine de votre dossier de projet), et non cachés ou restés dans le sous-dossier bin/Debug de Code::Blocks.

---

## 🔐 Configuration Initiale
Par défaut, le mot de passe d'accès au mode enseignant est défini par la macro MDP dans le fichier fonctions.c :
#define MDP "cytech"

Vous pouvez modifier directement cette chaîne dans le code source avant la compilation pour définir votre propre clé d'accès initiale.