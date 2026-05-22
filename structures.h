#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAX_CHAINE 100
#define MAX_QUESTIONS 20

// les options pour le qcm
typedef struct {
    int points_negatif;                         // 1 pour oui, 0 pour non
    int mode_sequentiel;                        // 1 si on veut le mode sequentiel
    int multi_reponse;                          // les questions peuvent avoir plusieurs rep
} configuration;

// une question de la liste
typedef struct {
    char enonce[MAX_CHAINE];                    // la question posee
    char proposition[4][MAX_CHAINE];            // rep 1, rep 2, etc (max 4)
    int bonne_reponse[4];                       // tableau avec des 1 sur les bonnes reponses
    int nb_propositions;                        // nombres de choix affiches au final
    float points;                               // les points que rapporte la question
} question;

// la structure finale du qcm
typedef struct {
    char nom_du_qcm[60];                        // nom du fichier .txt
    char categorie[40];                         // theme (sport, info...)
    int nombre_de_question;                     // nb total de questions dans le qcm
    question liste_questions[MAX_QUESTIONS];    // toutes les questions stockees
    configuration config;                       // la config du qcm
} qcm;

#endif
