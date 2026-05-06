#ifndef structures_h
#define structures_h


typedef struct {
int points_negatif; // 1 si on veut faire retier des points pour des mauvaise reponse sinon 0
int multi_reponse; //1 si on veut un qcm a choix multiple sinon 0
int mode_sequentiel; // 1 si on veut le mode sequentiel
} parametres;


typedef struct {
char enonce[255]; // Texte pour chaque question
char proposition[4][100]; // 4 choix possible avec 100 caratère maximum
int bonne_reponse[4]; // 1 si la reponse est bonne sinon 0
} question;


typedef struct {
char nom_du_qcm[50]; // Ecriture du nom du qcm
Parametres config; // Configuration des parametre pour le qcm
int nombre_de_question; // le nombre de question qu'on creer pour chaque qcm max 20 ducoup
question liste_questions[20]; // La liste des 20 question pour le qcm
} qcm;


#endif