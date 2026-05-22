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
    char texte[500];
    int reussite = 0;
    while (!reussite) {
        // Lecture de la chaine de caractere
        if (fgets(texte, sizeof(texte), stdin)) {
            texte[strcspn(texte, "\n")] = 0;
            // Verifications de la taille de caractere
            if (strlen(texte) >= tailleMax) {
                printf("Erreur : La taille est depassee (max %d car.). Veuillez reessayez : ", tailleMax - 1);
            } else if (strlen(texte) == 0) {
                printf("Erreur : La taille ne peut pas etre vide. Veuillez reessayez : ");
            } else {
                strcpy(destination, texte);  // Copie si la saisie est bonne
                reussite = 1;
            }
        }
    }
}

void afficher_liste_fichier() {
    struct dirent *lecture;
    DIR *dossier_qcm;
    char liste_categories[100][40];
    int nombre_categories = 0;

    printf("\n--- QCM DISPONIBLES PAR CATEGORIE ---\n");

    // Recuperation des categories
    dossier_qcm = opendir(".");
    if (dossier_qcm) {
        // On check le dossier pour lister les themes existants
        while ((lecture = readdir(dossier_qcm)) != NULL) {
            char *position_point = strrchr(lecture->d_name, '.');
            if (position_point && strcmp(position_point, ".txt") == 0) {
                FILE* test_F = fopen(lecture->d_name, "r");
                if (test_F) {
                    char poubelle[100];
                    char nom_categorie[40] = "Inconnue";
                    // Skip de la ligne 1 pour avoir la categorie en ligne 2
                    if (fgets(poubelle, sizeof(poubelle), test_F) && fgets(nom_categorie, sizeof(nom_categorie), test_F)) {
                        nom_categorie[strcspn(nom_categorie, "\n")] = 0;
                    }
                    fclose(test_F);

                    // Check doublon pour pas l'ajouter deux fois
                    int existe = 0;
                    for (int i = 0; i < nombre_categories; i++) {
                        if (strcmp(liste_categories[i], nom_categorie) == 0) {
                            existe = 1;
                            break;
                        }
                    }
                    // Ajout de la nouvelle categorie
                    if (!existe && nombre_categories < 100) {
                        strcpy(liste_categories[nombre_categories], nom_categorie);
                        nombre_categories++;
                    }
                }
            }
        }
        closedir(dossier_qcm);
    }

    // Si aucun fichier trouve
    if (nombre_categories == 0) {
        printf(" Aucun QCM n'est disponible pour le moment.\n");
    // Affichage par groupe de categorie
    } else {
        // On re-parcourt tout pour lier les qcm a leur theme
        for (int i = 0; i < nombre_categories; i++) {
            printf("\n Categorie : %s\n", liste_categories[i]);
            dossier_qcm = opendir(".");
            if (dossier_qcm) {
                while ((lecture = readdir(dossier_qcm)) != NULL) {
                    char *position_point = strrchr(lecture->d_name, '.');
                    if (position_point && strcmp(position_point, ".txt") == 0) {
                        FILE* test_F = fopen(lecture->d_name, "r");
                        if (test_F) {
                            char poubelle[100];
                            char cat_verification[40] = "";
                            // Verif si le fichier correspond bien a la categorie en cours
                            if (fgets(poubelle, sizeof(poubelle), test_F) && fgets(cat_verification, sizeof(cat_verification), test_F)) {
                                cat_verification[strcspn(cat_verification, "\n")] = 0;
                            }
                            fclose(test_F);

                            // Affichage propre sans le .txt
                            if (strcmp(cat_verification, liste_categories[i]) == 0) {
                                char nom_affichage[256];
                                strcpy(nom_affichage, lecture->d_name);
                                nom_affichage[position_point - lecture->d_name] = '\0';
                                printf("   - %s\n", nom_affichage);
                            }
                        }
                    }
                }
                closedir(dossier_qcm);
            }
        }
    }
    printf("-------------------------------------\n\n");
}

int saisir_entier(int minimum, int maximum) {
    int entier_saisi;
    // Controle de la saisie min/max
    while (scanf("%d", &entier_saisi) != 1 || entier_saisi < minimum || entier_saisi > maximum) {
        printf("Cette saisie est invalide (%d-%d). Veuillez reessayez : ", minimum, maximum);
        vider_buffer();
    }
    vider_buffer();
    return entier_saisi;
}

float saisir_flottant(float minimum, float maximum) {
    float flottant_saisi;
    while (scanf("%f", &flottant_saisi) != 1 || flottant_saisi < minimum || flottant_saisi > maximum) {
        printf("Cette saisie est invalide (%.1f-%.1f). Veuillez reessayez : ", minimum, maximum);
        vider_buffer();
    }
    vider_buffer();
    return flottant_saisi;
}

void sauvegarder_qcm(qcm *q) {
    // Creation ou ecrasement du fichier txt
    FILE* f = fopen(q->nom_du_qcm, "w");
    if (f == NULL) return;
    // Ligne 1 : config du qcm
    fprintf(f, "%d %d %d %d\n", q->nombre_de_question,
            q->config.points_negatif, q->config.mode_sequentiel, q->config.multi_reponse);
    // Ligne 2 : theme du qcm
    fprintf(f, "%s\n", q->categorie);
    // Boucle blocs questions (enonce, prop, reponses)
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
    // Ouverture en lecture seule
    FILE* f = fopen(nomFichier, "r");
    if (f == NULL) {
        strcpy(q.nom_du_qcm, "ERREUR");
        return q;
    }
    // Recuperation de la config en ligne 1
    fscanf(f, "%d %d %d %d\n", &q.nombre_de_question,
               &q.config.points_negatif, &q.config.mode_sequentiel, &q.config.multi_reponse);

    if (fgets(q.categorie, sizeof(q.categorie), f))
        q.categorie[strcspn(q.categorie, "\n")] = 0;

    // Chargement de toutes les questions
    for (int i = 0; i < q.nombre_de_question; i++) {
        if (fgets(q.liste_questions[i].enonce, MAX_CHAINE, f))
            q.liste_questions[i].enonce[strcspn(q.liste_questions[i].enonce, "\n")] = 0;
        // Recup du nombre de choix possibles
        fscanf(f, "%d\n", &q.liste_questions[i].nb_propositions);
        fscanf(f, "%f\n", &q.liste_questions[i].points);
        // Lecture de chaque proposition associee
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

    // Verif du mot de passe pour entrer
    printf("\nMot de passe admin : ");
    saisir_chaine_securisee(saisie, 20);

    // Si c'est pas le bon mdp on ejecte
    if (strcmp(saisie, mot_de_passe_actuel) != 0) {
        printf("Echec de l'authentification.\n");
        return;
    }
    printf("Acces autorise.\n");
    while (choix_prof != 4) {
        // Menu de l'enseignant
        printf("\n--- Mode Enseignant ---");
        printf("\n1. Creer/Modifier un QCM\n2. Supprimer un QCM\n3. Changer le mot de passe\n4. Retour\n\nChoix : ");
        choix_prof = saisir_entier(1, 4);

        if (choix_prof == 1) {
            afficher_liste_fichier();
            printf("Nom du QCM a creer ou modifier (ou '0' pour revenir au mode enseignant) : ");
            scanf("%s", actuel.nom_du_qcm);
            vider_buffer();

            // Si l'utilisateur met pas le .txt on le rajoute nous meme
            if (strcmp(actuel.nom_du_qcm, "0") == 0) continue;
            if (strstr(actuel.nom_du_qcm, ".txt") == NULL) strcat(actuel.nom_du_qcm, ".txt");

            printf("Categorie du QCM (ex: Informatique, Histoire...) : ");
            saisir_chaine_securisee(actuel.categorie, 40);

            // Parametrage des options du qcm
            printf("Points negatifs (1:Oui / 0:Non) : ");
            actuel.config.points_negatif = saisir_entier(0, 1);
            printf("Mode examen (sequentiel) (1:Oui / 0:Non) : ");
            actuel.config.mode_sequentiel = saisir_entier(0, 1);
            printf("Multi-reponses possibles (1:Oui / 0:Non) : ");
            actuel.config.multi_reponse = saisir_entier(0, 1);
            printf("Nombre total de questions : ");
            actuel.nombre_de_question = saisir_entier(1, MAX_QUESTIONS);

            // Parametrage des options du qcm
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
            // On sauvegarde tout dans le fichier
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
            // Suppression du fichier texte
            if (remove(nom_supprimer) == 0) printf("Supprime !\n");
            else printf("Erreur : Le fichier est introuvable.\n");
        }
        else if (choix_prof == 3) {
            char nouveau[20], confirmation[20];
            printf("\nNouveau mot de passe (max 19 caracteres) : ");
            saisir_chaine_securisee(nouveau, 20);
            printf("Confirmez : ");
            saisir_chaine_securisee(confirmation, 20);

            // On change le mdp seulement si les deux entrees sont pareilles
            if (strcmp(nouveau, confirmation) == 0) {
                strcpy(mot_de_passe_actuel, nouveau);
                printf("Mot de passe change !\n");
            } else {
                printf("Erreur : La confirmation est incorrecte.\n");
            }
        }
    }
}

void mode_etudiant() {
    char nom[60];
    // Liste des qcm dispos au debut
    afficher_liste_fichier();
    printf("Quel QCM voulez-vous faire (Entrez le nom exact ou '0' pour annuler) ? ");
    scanf("%s", nom);

    // Retour menu si annulation
    if (strcmp(nom, "0") == 0) {
        vider_buffer();
        return;
    }

    if (strstr(nom, ".txt") == NULL) strcat(nom, ".txt");
    vider_buffer();

    // Chargement du fichier qcm choisi
    qcm q = charger_qcm(nom);
    if (strcmp(q.nom_du_qcm, "ERREUR") == 0) {
        printf("Erreur : Le QCM est introuvable.\n");
        return;
    }

    // Affichage du recap des options du qcm
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

    // Option d'annulation avant le premier clic
    char confirmation[10];
    if (fgets(confirmation, sizeof(confirmation), stdin)) {
        if (confirmation[0] == '0') {
            printf("-> Retour au menu principal.\n");
            return;
        }
    }

    float note_obtenue = 0;
    float total_points_qcm = 0;

    // Boucle principale du questionnaire
    for (int i = 0; i < q.nombre_de_question; i++) {
        total_points_qcm += q.liste_questions[i].points;
        int reponses_eleve[4] = {0};

        printf("\nQUESTION %d/%d (%.2f pts) : %s\n", i+1, q.nombre_de_question, q.liste_questions[i].points, q.liste_questions[i].enonce);
        // Affichage des propositions de reponse
        for (int j = 0; j < q.liste_questions[i].nb_propositions; j++) {
            printf("  %d) %s\n", j+1, q.liste_questions[i].proposition[j]);
        }

        int saisie_valide = 0;
        char ligne[100];
        int question_passee = 0;

        // Boucle de saisie avec blindage d'erreur
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
            // Check si le caractere tape est un chiffre valide
            for(int k = 0; ligne[k] != '\0'; k++) {
                if (ligne[k] == ' ' || ligne[k] == '\t') continue;
                int index = ligne[k] - '1';
                if (index >= 0 && index < q.liste_questions[i].nb_propositions) {
                    reponses_eleve[index] = 1;
                    nombre_coches++;
                // Erreur si le chiffre n'existe pas dans les choix
                } else {
                    printf("ERREUR : LE caractere '%c' est incorrect. Veuillez utilisez les chiffres demandes.\n", ligne[k]);
                    saisie_valide = 0;
                    break;
                }
            }

            // Bloquage si choix unique mais plusieurs reponses entrees
            if (saisie_valide && q.config.multi_reponse == 0 && nombre_coches > 1) {
                printf("ERREUR : Ce QCM ne permet qu'UNE SEULE reponse par question.\n");
                saisie_valide = 0;
            }

            if (!saisie_valide) {
                for(int r = 0; r < 4; r++) reponses_eleve[r] = 0;
            }
        }

        // Debut bareme et calcul des points
        if (!question_passee) {
            int a_coche_une_erreur = 0;
            int total_bonnes_reponses_possibles = 0;
            int bonnes_reponses_trouvees_par_eleve = 0;

            // Comparaison choix eleve vs vraies reponses
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

    // Securite pas de note en dessous de zero
    if (note_obtenue < 0) note_obtenue = 0;

    // Calcul de la note finale sur 20
    float note_sur_20 = (note_obtenue / total_points_qcm) * 20.0;

    printf("\n========================================\n");
    printf("TERMINE ! Votre score total : %.2f / %.2f points\n", note_obtenue, total_points_qcm);
    printf("Votre note finale ramenee sur 20 : %.2f/20\n", note_sur_20);
    printf("========================================\n");
    printf("Appuyez sur Entree pour revenir au menu...");
    getchar();
}
