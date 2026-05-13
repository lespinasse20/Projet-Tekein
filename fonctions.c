#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

char mot_de_passe_actuel[20] = "cytech";

void viderBuffer() {
    while(getchar() != '\n');
}

// Fonction pour forcer un choix entre deux bornes (ex: 0 et 1)
int saisirEntier(int min, int max) {
    int n;
    while (scanf("%d", &n) != 1 || n < min || n > max) {
        printf("Saisie invalide (min:%d, max:%d). Reessayez : ", min, max);
        viderBuffer();
    }
    viderBuffer();
    return n;
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
        printf("Echec authentification : Mot de passe incorrect.\n");
        return;
    }
    printf("Authentification reussie !\n");

    while (choix_prof != 3) {
        printf("\n1. Creer QCM\n2. Changer MDP\n3. Retour\nChoix : ");
        choix_prof = saisirEntier(1, 3);

        if (choix_prof == 1) {
            int extension_ok = 0;
            while (!extension_ok) {
                printf("Nom du fichier (ex: Histoire.txt) : ");
                scanf("%s", nouveau.nom_du_qcm);
                if (strstr(nouveau.nom_du_qcm, ".txt") != NULL) extension_ok = 1;
                else printf("ERREUR : L'extension .txt est obligatoire !\n");
            }
            
            printf("Points negatifs (1:Oui / 0:Non) : ");
            nouveau.config.points_negatif = saisirEntier(0, 1);
            
            printf("Mode Sequentiel (1:Oui / 0:Non) : ");
            nouveau.config.mode_sequentiel = saisirEntier(0, 1);
            
            printf("Multi-reponse (1:Oui / 0:Non) : ");
            nouveau.config.multi_reponse = saisirEntier(0, 1);
            
            printf("Nombre de questions : ");
            nouveau.nombre_de_question = saisirEntier(1, MAX_QUESTIONS);

            for (int i = 0; i < nouveau.nombre_de_question; i++) {
                printf("Enonce Q%d : ", i+1);
                scanf(" %[^\n]", nouveau.liste_questions[i].enonce);
                for (int j = 0; j < 4; j++) {
                    printf("  Reponse %d : ", j+1);
                    scanf(" %[^\n]", nouveau.liste_questions[i].proposition[j]);
                    printf("  Est correct (1:Oui / 0:Non) : ");
                    nouveau.liste_questions[i].bonne_reponse[j] = saisirEntier(0, 1);
                }
            }
            sauvegarderQCM(&nouveau);
            printf("QCM Sauvegarde avec succes !\n");
        } else if (choix_prof == 2) {
            char confirm[20];
            int mdp_ok = 0;
            while (!mdp_ok) {
                printf("Nouveau MDP : ");
                scanf("%s", mot_de_passe_actuel);
                printf("Confirmez le nouveau MDP : ");
                scanf("%s", confirm);
                if (strcmp(mot_de_passe_actuel, confirm) == 0) {
                    printf("Mot de passe change avec succes !\n");
                    mdp_ok = 1;
                } else {
                    printf("ERREUR : Les MDP ne correspondent pas. Reessayez.\n");
                }
            }
        }
    }
}

void modeEtudiant() {
    char nom[50];
    float note = 0;

    printf("\nFichier QCM : ");
    scanf("%s", nom);
    viderBuffer(); 

    qcm q = chargerQCM(nom);
    if (strcmp(q.nom_du_qcm, "ERREUR") == 0) {
        printf("Erreur : Fichier introuvable.\n");
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
            printf("Choix (1-4) : ");
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
                    printf("ERREUR : Caractere '%c' interdit.\n", ligne[k]);
                    saisie_valide = 0;
                    break;
                }
            }
        }

        int faute = 0;
        for (int j = 0; j < 4; j++) {
            if (reponses_eleve[j] != q.liste_questions[i].bonne_reponse[j]) faute = 1;
        }

        if (faute == 0) {
            printf("-> CORRECT !\n");
            note += 1.0;
        } else {
            printf("-> INCORRECT.\n");
            if (q.config.points_negatif == 1) note -= 0.5;
        }
    }

    float noteSur20 = (note / (float)q.nombre_de_question) * 20.0;
    if (noteSur20 < 0) noteSur20 = 0;
    printf("\nNOTE FINALE : %.2f/20\n", noteSur20);
}