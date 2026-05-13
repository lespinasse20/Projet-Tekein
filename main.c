#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

void modeEnseignant();
void modeEtudiant();
void viderBuffer();

int main() {
    int choix = 0;
    while (choix != 3) {
        printf("\n--- LOGICIEL QCM ---\n1. Enseignant\n2. Etudiant\n3. Quitter\nChoix : ");

        while (scanf("%d", &choix) != 1) {
            printf("Erreur. Entrez 1, 2 ou 3 : ");
            viderBuffer();
        }

        if (choix == 1) modeEnseignant();
        else if (choix == 2) modeEtudiant();
    }
    printf("Au revoir !\n");
    return 0;
}