#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// Prototypes
void modeEnseignant();
void modeEtudiant();

int main() {
    int choix = 0;

    while (choix != 3) {
        printf("\n--- LOGICIEL QCM ---\n");
        printf("1. Enseignant\n2. Etudiant\n3. Quitter\nChoix : ");
        
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n'); // vider buffer si lettre tapee
            continue;
        }

        switch (choix) {
            case 1: modeEnseignant(); break;
            case 2: modeEtudiant(); break;
            case 3: printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }
    }
    return 0;
}