#ifndef MODELE_H_
#define MODELE_H_

int * initialiserGrille(int taille, int nbCouleurs);
int fin(int *grille, int taille);
void remplirGrille(int *grille, int taille, int couleurP, int new_color, int i, int j);
void afficherGrille(int *grille, int taille);

#endif