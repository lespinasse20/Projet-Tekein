#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_CHAINE 100
#define MAX_QUESTIONS 50

typedef struct {
    int points_negatif;   // 1 pour Oui, 0 pour Non
    int mode_sequentiel;  // 1 pour Oui, 0 pour Non
    int multi_reponse;    // 1 pour Oui, 0 pour Non
} configuration;

typedef struct {
    char enonce[MAX_CHAINE];
    char proposition[4][MAX_CHAINE];
    int bonne_reponse[4]; // 1 si vrai, 0 si faux
} question;

typedef struct {
    char nom_du_qcm[60];
    int nombre_de_question;
    configuration config;
    question liste_questions[MAX_QUESTIONS];
} qcm;

#endif