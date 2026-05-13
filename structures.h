#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAX_QUESTIONS 20
#define MAX_CHAINE 200

typedef struct {
    int points_negatif;
    int multi_reponse;
    int mode_sequentiel;
} parametres;

typedef struct {
    char enonce[MAX_CHAINE];
    char proposition[4][MAX_CHAINE];
    int bonne_reponse[4];
} question;

typedef struct {
    char nom_du_qcm[50];
    parametres config;
    int nombre_de_question;
    question liste_questions[MAX_QUESTIONS];
} qcm;

#endif