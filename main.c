#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// Prototypes des modes
void mode_enseignant();
void mode_etudiant();

int main() {
    int choix = 0;

    while (choix != 3) {
        printf("\n--- PROJET TEIKEN ---\n");
        printf("\nChoisissez votre mode:\n");
        printf("\n1. Enseignant\n2. Etudiant\n3. Quitter\n\nChoix : ");

        // Lecture du choix et securite si l'utilisateur met des lettres
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            printf("Erreur : Votre chiffre est invalide, veuilliez ressayer.\n");
            continue;
        }

        // Nettoyage du retour a la ligne restant dans le buffer
        while(getchar() != '\n');

        // Aiguillage vers le mode choisi
        switch (choix) {
            case 1:
                mode_enseignant();
                break;
            case 2:
                mode_etudiant();
                break;
            case 3:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez choisir entre 1, 2 ou 3.\n");
        }
    }
    return 0;
}
