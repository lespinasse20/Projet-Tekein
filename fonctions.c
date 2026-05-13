#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

char mot_de_passe_actuel[20] = "cytech";

void viderBuffer() {
    while(getchar() != '\n');
}

void sauvegarderQCM(qcm *q) {
    FILE* f = fopen(q->nom_du_qcm, "w");
    if (f == NULL) return;
    fprintf(f, "%d %d %d %d\n", q->nombre_de_question,
            q->config.points_negatif, q->config.mode_sequentiel, q->config.multi_reponse);

    for (int i = 0; i < q->nombre_de_question; i++) {
        fprintf(f, "%s\n", q->liste_questions[i].enonce);
        for (int j = 0; j < 4; j++) {
            fprintf(f, "%s\n%d\n", q->liste_questions[i].proposition[j], q->liste_questions[i].bonne_reponse[j]);
        }
    }
    fclose(f);
}

qcm chargerQCM(char nomFichier[]) {
    qcm q;
    FILE* f = fopen(nomFichier, "r");
    if (f == NULL) {
        strcpy(q.nom_du_qcm, "ERREUR");
        return q;
    }
    fscanf(f, "%d %d %d %d\n", &q.nombre_de_question,
               &q.config.points_negatif, &q.config.mode_sequentiel, &q.config.multi_reponse);

    for (int i = 0; i < q.nombre_de_question; i++) {
        if (fgets(q.liste_questions[i].enonce, MAX_CHAINE, f))
            q.liste_questions[i].enonce[strcspn(q.liste_questions[i].enonce, "\n")] = 0;

        for (int j = 0; j < 4; j++) {
            if (fgets(q.liste_questions[i].proposition[j], MAX_CHAINE, f))
                q.liste_questions[i].proposition[j][strcspn(q.liste_questions[i].proposition[j], "\n")] = 0;
            fscanf(f, "%d\n", &q.liste_questions[i].bonne_reponse[j]);
        }
    }
    fclose(f);
    strcpy(q.nom_du_qcm, nomFichier);
    return q;
}

void modeEnseignant() {
    char saisie[20];
    int choix_prof = 0;
    qcm nouveau;

    printf("\nMot de passe : ");
    scanf("%s", saisie);
    if (strcmp(saisie, mot_de_passe_actuel) != 0) {
        printf("Echec authentification.\n");
        return;
    }

    while (choix_prof != 3) {
        printf("\n1. Creer QCM\n2. Changer MDP\n3. Retour\nChoix : ");
        if (scanf("%d", &choix_prof) != 1) { viderBuffer(); choix_prof = 0; continue; }

        if (choix_prof == 1) {
            // --- NOUVELLE VALIDATION STRICTE DU .TXT ---
            int extension_ok = 0;
            while (extension_ok == 0) {
                printf("Nom du fichier (Obligatoire : .txt) : ");
                scanf("%s", nouveau.nom_du_qcm);

                // On cherche si ".txt" est present dans le nom
                if (strstr(nouveau.nom_du_qcm, ".txt") != NULL) {
                    extension_ok = 1;
                } else {
                    printf("ERREUR : Vous avez tape '%s'. L'extension .txt est manquante !\n", nouveau.nom_du_qcm);
                }
            }

            printf("Points negatifs (1:Oui / 0:Non) : ");
            scanf("%d", &nouveau.config.points_negatif);
            printf("Mode Sequentiel (1:Oui / 0:Non) : ");
            scanf("%d", &nouveau.config.mode_sequentiel);
            printf("Multi-reponse (1:Oui / 0:Non) : ");
            scanf("%d", &nouveau.config.multi_reponse);
            printf("Nombre de questions : ");
            scanf("%d", &nouveau.nombre_de_question);
            viderBuffer();

            for (int i = 0; i < nouveau.nombre_de_question; i++) {
                printf("Enonce Q%d : ", i+1);
                scanf(" %[^\n]", nouveau.liste_questions[i].enonce);
                for (int j = 0; j < 4; j++) {
                    printf("  Reponse %d : ", j+1);
                    scanf(" %[^\n]", nouveau.liste_questions[i].proposition[j]);
                    printf("  Est correct (1/0) : ");
                    scanf("%d", &nouveau.liste_questions[i].bonne_reponse[j]);
                }
                viderBuffer();
            }
            sauvegarderQCM(&nouveau);
            printf("QCM Sauvegarde !\n");
        } else if (choix_prof == 2) {
            printf("Nouveau MDP : ");
            scanf("%s", mot_de_passe_actuel);
        }
    }
}

void modeEtudiant() {
    char nom[50];
    float note = 0;

    printf("\nFichier QCM (avec .txt) : ");
    scanf("%s", nom);
    viderBuffer();

    qcm q = chargerQCM(nom);
    if (strcmp(q.nom_du_qcm, "ERREUR") == 0) {
        printf("Erreur : Impossible de trouver le fichier '%s'.\n", nom);
        return;
    }

    for (int i = 0; i < q.nombre_de_question; i++) {
        int reponses_eleve[4] = {0,0,0,0};

        printf("\nQUESTION : %s\n", q.liste_questions[i].enonce);
        for (int j = 0; j < 4; j++) {
            printf("  %d) %s\n", j+1, q.liste_questions[i].proposition[j]);
        }

        int saisie_valide = 0;
        char ligne[100];

        while (!saisie_valide) {
            printf("Choix (1-4) puis Entree : ");
            if (!fgets(ligne, 100, stdin)) continue;

            ligne[strcspn(ligne, "\n")] = 0;
            if (strlen(ligne) == 0) continue;

            saisie_valide = 1;
            for(int r=0; r<4; r++) reponses_eleve[r] = 0;

            for(int k = 0; ligne[k] != '\0'; k++) {
                if (ligne[k] == ' ' || ligne[k] == '\t') continue;

                if(ligne[k] >= '1' && ligne[k] <= '4') {
                    reponses_eleve[ligne[k] - '1'] = 1;
                } else {
                    printf("ERREUR : '%c' est interdit. Utilisez 1, 2, 3 ou 4.\n", ligne[k]);
                    saisie_valide = 0;
                    break;
                }
            }

            int check = 0;
            for(int r=0; r<4; r++) if(reponses_eleve[r] == 1) check = 1;
            if(saisie_valide == 1 && check == 0) {
                printf("ERREUR : Aucune selection.\n");
                saisie_valide = 0;
            }
        }

        int faute = 0;
        for (int j = 0; j < 4; j++) {
            if (reponses_eleve[j] != q.liste_questions[i].bonne_reponse[j]) {
                faute = 1;
                break;
            }
        }

        if (faute == 0) {
            if (q.config.mode_sequentiel == 0) printf("-> BRAVO !\n");
            note += 1.0;
        } else {
            if (q.config.mode_sequentiel == 0) printf("-> MAUVAIS.\n");
            if (q.config.points_negatif == 1) note -= 0.5;
        }
    }

    float noteSur20 = (note / (float)q.nombre_de_question) * 20.0;
    if (noteSur20 < 0) noteSur20 = 0;

    printf("\n======================\n");
    printf(" NOTE FINALE : %.2f/20\n", noteSur20);
    printf("======================\n");
}