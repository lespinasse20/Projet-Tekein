#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "structures.h"
#define MDP "cytech"

char mot_de_passe_actuel[20] = MDP;

void vider_buffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void saisir_chaine_securisee(char *destination, int tailleMax) {
    char temp[500];
    int valide = 0;
    while (!valide) {
        if (fgets(temp, sizeof(temp), stdin)) {
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) >= tailleMax) {
                printf("Erreur : La taille est depassee (max %d car.). Veuillez reessayez : ", tailleMax - 1);
            } else if (strlen(temp) == 0) {
                printf("Erreur : La taille ne peut pas etre vide. Veuillez reessayez : ");
            } else {
                strcpy(destination, temp);
                valide = 1;
            }
        }
    }
}

void afficher_liste_fichier() {
    struct dirent *lecture;
    DIR *rep;
    char categories_trouvees[100][40];
    int nombre_categories = 0;

    printf("\n--- QCM DISPONIBLES PAR CATEGORIE ---\n");

    rep = opendir(".");
    if (rep) {
        while ((lecture = readdir(rep)) != NULL) {
            char *dot = strrchr(lecture->d_name, '.');
            if (dot && strcmp(dot, ".txt") == 0 && strcmp(lecture->d_name, "mdp.txt") != 0) {
                FILE* test_F = fopen(lecture->d_name, "r");
                if (test_F) {
                    char poubelle[100];
                    char cat[40] = "Inconnue";
                    if (fgets(poubelle, sizeof(poubelle), test_F) && fgets(cat, sizeof(cat), test_F)) {
                        cat[strcspn(cat, "\n")] = 0;
                    }
                    fclose(test_F);

                    int existe = 0;
                    for (int i = 0; i < nombre_categories; i++) {
                        if (strcmp(categories_trouvees[i], cat) == 0) {
                            existe = 1;
                            break;
                        }
                    }
                    if (!existe && nombre_categories < 100) {
                        strcpy(categories_trouvees[nombre_categories], cat);
                        nombre_categories++;
                    }
                }
            }
        }
        closedir(rep);
    }

    if (nombre_categories == 0) {
        printf(" Aucun QCM n'est disponible pour le moment.\n");
    } else {
        for (int i = 0; i < nombre_categories; i++) {
            printf("\n Categorie : %s\n", categories_trouvees[i]);
            rep = opendir(".");
            if (rep) {
                while ((lecture = readdir(rep)) != NULL) {
                    char *dot = strrchr(lecture->d_name, '.');
                    if (dot && strcmp(dot, ".txt") == 0 && strcmp(lecture->d_name, "mdp.txt") != 0) {
                        FILE* test_F = fopen(lecture->d_name, "r");
                        if (test_F) {
                            char poubelle[100];
                            char cat_verification[40] = "";
                            if (fgets(poubelle, sizeof(poubelle), test_F) && fgets(cat_verification, sizeof(cat_verification), test_F)) {
                                cat_verification[strcspn(cat_verification, "\n")] = 0;
                            }
                            fclose(test_F);

                            if (strcmp(cat_verification, categories_trouvees[i]) == 0) {
                                char nom_affichage[256];
                                strcpy(nom_affichage, lecture->d_name);
                                nom_affichage[dot - lecture->d_name] = '\0';
                                printf("   - %s\n", nom_affichage);
                            }
                        }
                    }
                }
                closedir(rep);
            }
        }
    }
    printf("-------------------------------------\n\n");
}

int saisir_entier(int min, int max) {
    int n;
    while (scanf("%d", &n) != 1 || n < min || n > max) {
        printf("Cette saisie est invalide (%d-%d). Veuillez reessayez : ", min, max);
        vider_buffer();
    }
    vider_buffer();
    return n;
}

float saisir_flottant(float min, float max) {
    float n;
    while (scanf("%f", &n) != 1 || n < min || n > max) {
        printf("Cette saisie est invalide (%.1f-%.1f). Veuillez reessayez : ", min, max);
        vider_buffer();
    }
    vider_buffer();
    return n;
}

void sauvegarder_qcm(qcm *q) {
    FILE* f = fopen(q->nom_du_qcm, "w");
    if (f == NULL) return;
    fprintf(f, "%d %d %d %d\n", q->nombre_de_question,
            q->config.points_negatif, q->config.mode_sequentiel, q->config.multi_reponse);
    fprintf(f, "%s\n", q->categorie);
    for (int i = 0; i < q->nombre_de_question; i++) {
        fprintf(f, "%s\n%d\n%.2f\n", q->liste_questions[i].enonce, q->liste_questions[i].nb_propositions, q->liste_questions[i].points);
        for (int j = 0; j < q->liste_questions[i].nb_propositions; j++) {
            fprintf(f, "%s\n%d\n", q->liste_questions[i].proposition[j], q->liste_questions[i].bonne_reponse[j]);
        }
    }
    fclose(f);
}

qcm charger_qcm(char nomFichier[]) {
    qcm q;
    FILE* f = fopen(nomFichier, "r");
    if (f == NULL) {
        strcpy(q.nom_du_qcm, "ERREUR");
        return q;
    }
    fscanf(f, "%d %d %d %d\n", &q.nombre_de_question,
               &q.config.points_negatif, &q.config.mode_sequentiel, &q.config.multi_reponse);

    if (fgets(q.categorie, sizeof(q.categorie), f))
        q.categorie[strcspn(q.categorie, "\n")] = 0;

    for (int i = 0; i < q.nombre_de_question; i++) {
        if (fgets(q.liste_questions[i].enonce, MAX_CHAINE, f))
            q.liste_questions[i].enonce[strcspn(q.liste_questions[i].enonce, "\n")] = 0;
        fscanf(f, "%d\n", &q.liste_questions[i].nb_propositions);
        fscanf(f, "%f\n", &q.liste_questions[i].points);
        for (int j = 0; j < q.liste_questions[i].nb_propositions; j++) {
            if (fgets(q.liste_questions[i].proposition[j], MAX_CHAINE, f))
                q.liste_questions[i].proposition[j][strcspn(q.liste_questions[i].proposition[j], "\n")] = 0;
            fscanf(f, "%d\n", &q.liste_questions[i].bonne_reponse[j]);
        }
    }
    fclose(f);
    strcpy(q.nom_du_qcm, nomFichier);
    return q;
}

void mode_enseignant() {
    char saisie[20];
    int choix_prof = 0;
    qcm actuel;

    printf("\nMot de passe admin : ");
    saisir_chaine_securisee(saisie, 20);

    if (strcmp(saisie, mot_de_passe_actuel) != 0) {
        printf("Echec de l'authentification.\n");
        return;
    }
    printf("Acces autorise.\n");
    while (choix_prof != 4) {
        printf("\n--- Mode Enseignant ---");
        printf("\n1. Creer/Modifier un QCM\n2. Supprimer un QCM\n3. Changer le mot de passe\n4. Retour\n\nChoix : ");
        choix_prof = saisir_entier(1, 4);

        if (choix_prof == 1) {
            afficher_liste_fichier();
            printf("Nom du QCM a creer ou modifier (ou '0' pour revenir au mode enseignant) : ");
            scanf("%s", actuel.nom_du_qcm);
            vider_buffer();

            if (strcmp(actuel.nom_du_qcm, "0") == 0) continue;
            if (strstr(actuel.nom_du_qcm, ".txt") == NULL) strcat(actuel.nom_du_qcm, ".txt");

            printf("Categorie du QCM (ex: Informatique, Histoire...) : ");
            saisir_chaine_securisee(actuel.categorie, 40);

            printf("Points negatifs (1:Oui / 0:Non) : ");
            actuel.config.points_negatif = saisir_entier(0, 1);
            printf("Mode examen (sequentiel) (1:Oui / 0:Non) : ");
            actuel.config.mode_sequentiel = saisir_entier(0, 1);
            printf("Multi-reponses possibles (1:Oui / 0:Non) : ");
            actuel.config.multi_reponse = saisir_entier(0, 1);
            printf("Nombre total de questions : ");
            actuel.nombre_de_question = saisir_entier(1, MAX_QUESTIONS);

            for (int i = 0; i < actuel.nombre_de_question; i++) {
                printf("\n--- Question %d ---\n", i+1);
                printf("Contenue de l'enonce (max 100 car.) : ");
                saisir_chaine_securisee(actuel.liste_questions[i].enonce, MAX_CHAINE);

                printf("Nombre de points pour cette question (ex: 1.00 ou 2.50) : ");
                actuel.liste_questions[i].points = saisir_flottant(0.1, 20.0);

                printf("Combien de propositions (2 a 4) ? ");
                actuel.liste_questions[i].nb_propositions = saisir_entier(2, 4);
                for (int j = 0; j < actuel.liste_questions[i].nb_propositions; j++) {
                    printf("  Contenue de la reponse %d (max 100 car.) : ", j+1);
                    saisir_chaine_securisee(actuel.liste_questions[i].proposition[j], MAX_CHAINE);
                    printf("  Est-ce correct (1:Oui / 0:Non) : ");
                    actuel.liste_questions[i].bonne_reponse[j] = saisir_entier(0, 1);
                }
            }
            sauvegarder_qcm(&actuel);
            printf("Votre QCM a ete enregistre avec succes !\n");
        }
        else if (choix_prof == 2) {
            char nom_supprimer[60];
            afficher_liste_fichier();
            printf("Nom du QCM a supprimer (ou '0' pour revenir au mode enseignant) : ");
            scanf("%s", nom_supprimer);
            if (strcmp(nom_supprimer, "0") == 0) continue;

            if (strstr(nom_supprimer, ".txt") == NULL) strcat(nom_supprimer, ".txt");
            if (remove(nom_supprimer) == 0) printf("Supprime !\n");
            else printf("Erreur : Le fichier est introuvable.\n");
        }
        else if (choix_prof == 3) {
            char nouv[20], conf[20];
            printf("\nNouveau mot de passe (max 19 caracteres) : ");
            saisir_chaine_securisee(nouv, 20);
            printf("Confirmez : ");
            saisir_chaine_securisee(conf, 20);

            if (strcmp(nouv, conf) == 0) {
                strcpy(mot_de_passe_actuel, nouv);
                printf("Mot de passe change !\n");
            } else {
                printf("Erreur : La confirmation est incorrecte.\n");
            }
        }
    }
}

void mode_etudiant() {
    char nom[60];
    afficher_liste_fichier();
    printf("Quel QCM voulez-vous faire (Entrez le nom exact ou '0' pour annuler) ? ");
    scanf("%s", nom);

    if (strcmp(nom, "0") == 0) {
        vider_buffer();
        return;
    }

    if (strstr(nom, ".txt") == NULL) strcat(nom, ".txt");
    vider_buffer();

    qcm q = charger_qcm(nom);
    if (strcmp(q.nom_du_qcm, "ERREUR") == 0) {
        printf("Erreur : Le QCM est introuvable.\n");
        return;
    }

    printf("\n========================================\n");
    printf("     PARAMETRES DE CE QUESTIONNAIRE     \n");
    printf("========================================\n");
    printf("- Categorie : %s\n", q.categorie);
    printf("- Points negatifs : %s\n", q.config.points_negatif ? "OUI" : "NON");
    printf("- Mode examen (sequentiel) : %s\n", q.config.mode_sequentiel ? "OUI" : "NON");
    printf("- Multi-reponses possibles : %s\n", q.config.multi_reponse ? "OUI" : "NON");
    printf("- Nombre de questions : %d\n", q.nombre_de_question);
    printf("========================================\n");
    printf("Appuyez sur Entree pour commencer (ou entrez '0' pour revenir au menu) : ");

    char confirmation[10];
    if (fgets(confirmation, sizeof(confirmation), stdin)) {
        // Si l'étudiant a tapé '0' avant d'appuyer sur Entrée
        if (confirmation[0] == '0') {
            printf("-> Retour au menu principal.\n");
            return; // On quitte immédiatement la fonction pour revenir au menu
        }
    }

    float note_obtenue = 0;
    float total_points_qcm = 0;

    for (int i = 0; i < q.nombre_de_question; i++) {
        total_points_qcm += q.liste_questions[i].points;
        int reponses_eleve[4] = {0};

        printf("\nQUESTION %d/%d (%.2f pts) : %s\n", i+1, q.nombre_de_question, q.liste_questions[i].points, q.liste_questions[i].enonce);
        for (int j = 0; j < q.liste_questions[i].nb_propositions; j++) {
            printf("  %d) %s\n", j+1, q.liste_questions[i].proposition[j]);
        }

        int saisie_valide = 0;
        char ligne[100];
        int question_passee = 0;

        while (!saisie_valide) {
            if (q.config.multi_reponse == 1) {
                printf("Choix multiples (ex: '13' pour cocher 1 et 3%s) : ",
                       q.config.mode_sequentiel ? "" : " ou Entree pour passer");
            } else {
                printf("Choix unique (1 a %d%s) : ", q.liste_questions[i].nb_propositions,
                       q.config.mode_sequentiel ? "" : " ou Entree pour passer");
            }

            if (!fgets(ligne, 100, stdin)) continue;
            ligne[strcspn(ligne, "\n")] = 0;

            if (strlen(ligne) == 0) {
                if (q.config.mode_sequentiel == 1) {
                    printf("ERREUR : Le mode examen est actif, vous devez obligatoirement repondre !\n");
                    continue;
                } else {
                    printf("-> Question passee sans reponse.\n");
                    question_passee = 1;
                    saisie_valide = 1;
                    break;
                }
            }

            saisie_valide = 1;
            int nombre_coches = 0;
            for(int k = 0; ligne[k] != '\0'; k++) {
                if (ligne[k] == ' ' || ligne[k] == '\t') continue;
                int index = ligne[k] - '1';
                if (index >= 0 && index < q.liste_questions[i].nb_propositions) {
                    reponses_eleve[index] = 1;
                    nombre_coches++;
                } else {
                    printf("ERREUR : LE caractere '%c' est incorrect. Veuillez utilisez les chiffres demandes.\n", ligne[k]);
                    saisie_valide = 0;
                    break;
                }
            }

            if (saisie_valide && q.config.multi_reponse == 0 && nombre_coches > 1) {
                printf("ERREUR : Ce QCM ne permet qu'UNE SEULE reponse par question.\n");
                saisie_valide = 0;
            }

            if (!saisie_valide) {
                for(int r = 0; r < 4; r++) reponses_eleve[r] = 0;
            }
        }


        if (!question_passee) {
            int a_coche_une_erreur = 0;
            int total_bonnes_reponses_possibles = 0;
            int bonnes_reponses_trouvees_par_eleve = 0;

            for (int j = 0; j < q.liste_questions[i].nb_propositions; j++) {
                if (q.liste_questions[i].bonne_reponse[j] == 1) {
                    total_bonnes_reponses_possibles++;
                    if (reponses_eleve[j] == 1) {
                        bonnes_reponses_trouvees_par_eleve++;
                    }
                } else {
                    if (reponses_eleve[j] == 1) {
                        a_coche_une_erreur = 1;
                    }
                }
            }

            if (a_coche_une_erreur) {
                if (q.config.mode_sequentiel == 0) printf("-> INCORRECT.\n");
                if (q.config.points_negatif == 1) {
                    note_obtenue -= 1;
                }
            }
            else if (bonnes_reponses_trouvees_par_eleve == 0) {
                if (q.config.mode_sequentiel == 0) printf("-> INCORRECT.\n");
            }
            else if (bonnes_reponses_trouvees_par_eleve == total_bonnes_reponses_possibles) {
                if (q.config.mode_sequentiel == 0) printf("-> CORRECT !\n");
                note_obtenue += q.liste_questions[i].points;
            }
            else {
                float points_partiels = q.liste_questions[i].points * ((float)bonnes_reponses_trouvees_par_eleve / (float)total_bonnes_reponses_possibles);
                if (q.config.mode_sequentiel == 0) printf("-> PARTIELLEMENT CORRECT ! (+%.2f pts)\n", points_partiels);
                note_obtenue += points_partiels;
            }
        }

        if (q.config.mode_sequentiel == 1) {
            printf("-> Reponse enregistree. Question suivante...\n");
        }
    }

    if (note_obtenue < 0) note_obtenue = 0;

    float note_sur_20 = (note_obtenue / total_points_qcm) * 20.0;

    printf("\n========================================\n");
    printf("TERMINE ! Votre score total : %.2f / %.2f points\n", note_obtenue, total_points_qcm);
    printf("Votre note finale ramenee sur 20 : %.2f/20\n", note_sur_20);
    printf("========================================\n");
    printf("Appuyez sur Entree pour revenir au menu...");
    getchar();
}
