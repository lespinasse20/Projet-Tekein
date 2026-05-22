#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAX_CHAINE 100
#define MAX_QUESTIONS 20

// Les options pour le QCM
typedef struct {
    int points_negatif;                         // 1 pour oui, 0 pour non
    int mode_sequentiel;                        // 1 si on veut le mode sequentiel
    int multi_reponse;                          // Les questions peuvent avoir plusieurs rep
} configuration;

// Une question du QCM
typedef struct {
    char enonce[MAX_CHAINE];                    // La question posee
    char proposition[4][MAX_CHAINE];            // Rep 1, rep 2, etc (max 4)
    int bonne_reponse[4];                       // Tableau avec des 1 sur les bonnes reponses
    int nb_propositions;                        // Nombres de choix affiches au final
    float points;                               // Les points que rapporte la question
} question;

// La structure finale du QCM
typedef struct {
    char nom_du_qcm[60];                        // Nom du fichier .txt
    char categorie[40];                         // Theme (sport, info...)
    int nombre_de_question;                     // Nombre total de questions dans le QCM
    question liste_questions[MAX_QUESTIONS];    // Toutes les questions stockees
    configuration config;                       // La config du QCM
} qcm;

#endif
