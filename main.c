#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// Prototypes des fonctions principales
void mode_enseignant();
void mode_etudiant();

int main() {
    int choix = 0;

    while (choix != 3) {
        printf("\n--- PROJET TEIKEN ---\n");
        printf("\nChoisissez votre mode:\n");
        printf("\n1. Enseignant\n2. Etudiant\n3. Quitter\n\nChoix : ");

        // Étape 1 : On tente de lire l'entier
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n'); // Sécurité : vide le buffer complet si l'utilisateur a tapé des lettres
            printf("Erreur : Votre chiffre est invalide, veuilliez ressayer.\n");
            continue;
        }

        // Étape 2 : CORRECTION IMPÉRATIVE
        // On nettoie systématiquement la touche 'Entrée' (\n) laissée par le scanf valide
        while(getchar() != '\n');

        // Étape 3 : On aiguille vers le bon mode avec un buffer d'entrée tout propre
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
