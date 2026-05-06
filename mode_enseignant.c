#include <stdlib.h>
#include <string.h>
#include "structures.h"

// Mode enseignant

void modeEnseignant() {
    qcm nouveau_qcm;
    int i, j;

    printf("\n--- CREATION D'UN NOUVEAU QCM ---\n");

    // Parametrage initial du qcm par l'enseignant
    printf("Nom du fichier (ex: quiz1.txt) : ");
    scanf("%s", nouveau_qcm.nom_du_qcm);

    printf("Points negatifs ? (1:Oui / 0:Non) : ");
    scanf("%d", &nouveau_qcm.config.points_negatif);
    
    printf("Mode sequentiel ? (1:Oui / 0:Non) : ");
    scanf("%d", &nouveau_qcm.config.mode_sequentiel);
    
    nouveau_qcm.config.multi_reponse = 0; // C'est le choix par defaut si on veut pas choisir

    printf("Nombre de questions (max 50) : ");
    scanf("%d", &nouveau_qcm.nombre_de_question);

    // C'est la saisie des questions  
    for (i = 0; i < nouveau_qcm.nombre_de_question; i++) {
        printf("\nQuestion n%d : ", i + 1);
        scanf("%s", nouveau_qcm.liste_questions[i].enonce);

        for (j = 0; j < 4; j++) {
            printf("  Choix %d : ", j + 1);
            scanf("%s", nouveau_qcm.liste_questions[i].proposition[j][100];
            
            printf("  Est-ce juste ? (1:Oui / 0:Non) : ");
            scanf("%d", &nouveau_qcm.liste_questions[i].bonne_reponse[j]);
        }
    }

    // 3. Sauvegarde automatique quand fini ( pas encore commencer )
    sauvegarderQCM(nouveau_qcm);
}