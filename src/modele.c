#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "modele.h"

int * initialiserGrille(int taille, int nbCouleurs){
	int * grille;
	int i, j;

	grille = (int *)malloc(sizeof(int) * (taille * taille));

    if(!grille){
        printf("Problème malloc\n");
        exit(1);
    }

	srand(time(NULL));
    
	for(i=0; i<taille; i++)
		for(j=0; j<taille; j++)
            grille[j + taille * i] = rand() % nbCouleurs + 1; // grille[x + r*y], Avec y la ligne, r le nombre de cases dans une ligne et x la colonne

	return grille;
}

// Renvoie 1 si la grille est remplie de la meme couleur, 0 sinon.
int fin(int *grille, int taille){
	int i, j, premiere_couleur;

	premiere_couleur = grille[0];

	for(i=0; i<taille; i++)
		for(j=0; j<taille; j++)
			if(grille[j + taille * i] != premiere_couleur) // grille[x + r*y], Avec y la ligne, r le nombre de cases dans une ligne et x la colonne
				return 0;

	return 1;
}

// couleurP : couleur de la premiere case (0,0) avant changement.
// i et j sont les indices de la case qui va changer de couleur.
void remplirGrille(int *grille, int taille, int couleurP, int new_color, int i, int j){
	if(i >= 0 && i <= taille && j >= 0 && j <= taille){
		if(grille[j + taille * i] == couleurP){ // grille[x + r*y], Avec y la ligne, r le nombre de cases dans une ligne et x la colonne
			grille[j + taille * i] = new_color; // grille[x + r*y], Avec y la ligne, r le nombre de cases dans une ligne et x la colonne
			remplirGrille(grille, taille, couleurP, new_color, i+1, j); // case a droite de celle etudiee
			remplirGrille(grille, taille, couleurP, new_color, i-1, j); // case a gauche de celle etudiee
			remplirGrille(grille, taille, couleurP, new_color, i, j+1); // case en haut de celle etudiee
			remplirGrille(grille, taille, couleurP, new_color, i, j-1); // case en bas de celle etudiee
		}
	}
}

// Pour debug
void afficherGrille(int *grille, int taille){
    int i, j;

    for(i=0; i<taille; i++)
    {
		for(j=0; j<taille; j++)
		{
			printf("%d", grille[j + taille * i]);
		}
        printf("\n");
    }
}
