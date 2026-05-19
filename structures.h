#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAX_CHAINE 100
#define MAX_QUESTIONS 20

typedef struct {
    int points_negatif;
    int mode_sequentiel;
    int multi_reponse;
} configuration;

typedef struct {
    char enonce[MAX_CHAINE];
    char proposition[4][MAX_CHAINE];
    int bonne_reponse[4];
    int nb_propositions;
    float points;         // NOUVEAU : Nombre de points attribués à CETTE question
} question;

typedef struct {
    char nom_du_qcm[60];
    char categorie[40];   // NOUVEAU : Catégorie du QCM (ex: Informatique, Histoire...)
    int nombre_de_question;
    question liste_questions[MAX_QUESTIONS];
    configuration config;
} qcm;

#endif