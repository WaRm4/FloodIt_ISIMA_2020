#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "modele.h"

int COULEURS; // mieux entre 4 et 8
int MAX_ESSAIS;
int TAILLE; // mieux entre 2 et 24
int WINDOW_H = 960;
int WINDOW_L = 1920;
int LARGEUR_CASE;
int NB_ESSAIS = 0;
TTF_Font * FONT;

/**
 * @brief Initialisation de SDL
 * 
 */
void initiateSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("ERREUR : initialisation SDL : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Initialisation de la fenêtre du jeu
 * 
 * @param title 
 * 			titre de la fenêtre
 * @return SDL_Window* 
 * 			La fenêtre en question
 */
SDL_Window * initiateWindow(char *title)
{
	SDL_Window *window = SDL_CreateWindow(title, 
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
								WINDOW_L, WINDOW_H, SDL_WINDOW_RESIZABLE);

	if(!window){
		printf("ERREUR : Creation fenetre echouee : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return window;
}

/**
 * @brief Initialisation du rendu de la fenêtre
 * 
 * @param window 
 * 			La fenêtre dans laquelle afficher le rendu
 * @return SDL_Renderer* 
 * 			Le rendu 
 */
SDL_Renderer * initiateRenderer(SDL_Window * window)
{
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED ); /*  SDL_RENDERER_SOFTWARE */
	if (renderer == 0) {
		printf("Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); 
		exit(EXIT_FAILURE);
	}
	return renderer;
} 

/**
 * @brief Initialisation de la police à afficher
 * 
 */
void initiateFont(){
	if (TTF_Init() != 0)
	{
		printf("Erreur d'initialisation TTF : %s\n", TTF_GetError()); 
	}
}

/**
 * @brief Affiche le texte positionné en bas de la fenêtre.
 * 
 * @param renderer 
 * 			Le rendu à afficher
 * @param text 
 * 			Le texte à afficher
 */
void displayLowerText(SDL_Renderer *renderer, char *text)
{
	int iW , iH ;
	SDL_Rect rect;

	FONT = TTF_OpenFont("../polices/fake.receipt.ttf", 200 );

	rect.x = LARGEUR_CASE * (TAILLE + 3) + WINDOW_L / 8;
	rect.y = WINDOW_H / 2;
	rect.w = LARGEUR_CASE * (TAILLE / 3);
	rect.h = WINDOW_H / 3 - LARGEUR_CASE * 2;

	SDL_Color couleur = {0, 0, 255, 255};        
	SDL_Surface * surf = TTF_RenderText_Blended(FONT, text, couleur);
	SDL_Texture * texttext = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_QueryTexture(texttext, NULL, NULL, &iW, &iH);
	SDL_RenderCopy(renderer, texttext, NULL, &rect);

	TTF_CloseFont(FONT);
}

/**
 * @brief Affiche le texte positionné en haut de la fenêtre.
 * 
 * @param renderer 
 * 			Le rendu à afficher
 * @param text 
 * 			Le texte à afficher
 */
void displayUpperText(SDL_Renderer *renderer, char *text)
{
	int iW , iH ;
	SDL_Rect rect;

	FONT = TTF_OpenFont("../polices/chlorinar.regular.ttf", 200 );

	rect.x = LARGEUR_CASE * (TAILLE + 3);
	rect.y = LARGEUR_CASE;
	rect.w = WINDOW_L - (LARGEUR_CASE * (TAILLE + 4));
	rect.h = WINDOW_H / 2 - LARGEUR_CASE * 2;

	SDL_Color couleur = {0, 0, 255, 255};        
	SDL_Surface * surf = TTF_RenderText_Blended(FONT, text, couleur);
	SDL_Texture * texttext = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_QueryTexture(texttext, NULL, NULL, &iW, &iH);
	SDL_RenderCopy(renderer, texttext, NULL, &rect);

	TTF_CloseFont(FONT);
}

/**
 * @brief Affiche le grille du jeu à l'écran.
 * 
 * @param renderer 
 * 			Le rendu à afficher
 * @param grille 
 * 			La grille contenant l'état courant du jeu
 */
void displayGrid(SDL_Renderer *renderer, int *grille)
{
	SDL_Rect rect;
	int i, j;
	int colors[27] = {
		183, 28, 28,
		74, 20, 140,
		1, 87, 155,
		51, 105, 30,
		245, 127, 23,
		62, 39, 35,
		255, 241, 118,
		128, 222, 234
	};

	for(i=0; i<TAILLE; i++){
		for(j=0; j<TAILLE; j++){
			
			SDL_SetRenderDrawColor(renderer,
			 colors[ grille[j + TAILLE * i] * 3 - 3 ], 
			 colors[ grille[j + TAILLE * i] * 3 - 2], 
			 colors[ grille[j + TAILLE * i] * 3 - 1 ], 
			 0);
			
			rect.x = LARGEUR_CASE + i * LARGEUR_CASE;
			rect.y = ((WINDOW_H / 2) - (LARGEUR_CASE * (TAILLE / 2))) + j * LARGEUR_CASE ;
			rect.w = rect.h = LARGEUR_CASE;
			SDL_RenderFillRect(renderer, &rect );
		}
	}
}

/**
 * @brief Ajuste les tailles des cases de la grille.
 * 
 */
void adjustSizes()
{
	if(WINDOW_L <= WINDOW_H * 2)
		LARGEUR_CASE = (WINDOW_L / 2) / ((float) TAILLE + 2) ;
	else
		LARGEUR_CASE = WINDOW_H / ((float) TAILLE + 2) ;
}

/**
 * @brief Fait le rendu à l'écran des textes, grille, et couleurs.
 * 
 * @param renderer 
 * 			Le rendu à afficher
 * @param grille 
 * 			La grille contenant l'état courant du jeu
 * @param upperText 
 * 			Le texte à afficher en haut
 * @param lowerText 
 * 			Le texte à afficher en bas
 */
void displayAll(SDL_Renderer *renderer, int *grille, char *upperText, char *lowerText)
{
	// Couleur de fond du rendu
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
	SDL_RenderClear(renderer);
	displayGrid(renderer, grille);
	displayUpperText(renderer, upperText);
	displayLowerText(renderer, lowerText);
	SDL_RenderPresent(renderer);	
}

/**
 * @brief Re-initialise la grille et met le nombre d'essais à 0;
 * 
 * @return int* 
 * 			Une nouvelle grille
 */
int * playAgain()
{
	NB_ESSAIS = 0;
	return initialiserGrille(TAILLE, COULEURS);
}

/**
 * @brief Boucle de jeu
 * 
 * @param renderer 
 * 			Le rendu à afficher
 * @param grille 
 * 			La grille contenant l'état courant du jeu
 */
void gameLoop(SDL_Renderer *renderer ,int *grille)
{
	SDL_Event event;
	int x, y;
	int running = 1;
	int choix, couleurP;

	couleurP = grille[0];

	while (running) 
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_WINDOWEVENT:
					switch (event.window.event)  
					{
						case SDL_WINDOWEVENT_CLOSE:  
							running = 0;
							break;

						case SDL_WINDOWEVENT_SIZE_CHANGED:
							WINDOW_L = event.window.data1;
							WINDOW_H = event.window.data2;
							adjustSizes();
							break;

						default:
						if(NB_ESSAIS < MAX_ESSAIS && fin(grille, TAILLE) != 1)
						{
							char lowerText[6];
							sprintf(lowerText, "%d-%d", NB_ESSAIS, MAX_ESSAIS);
							displayAll(renderer, grille, "FloodIt", lowerText);
						}
						if(NB_ESSAIS >= MAX_ESSAIS || fin(grille, TAILLE) == 1)
						{
							if( fin(grille, TAILLE) == 1)
								displayAll(renderer, grille, "You won", "Play again");
							else
								displayAll(renderer, grille, "You lost", "Play again");
						}
					}   
					break;

				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x - LARGEUR_CASE;
					y = event.button.y - ((WINDOW_H / 2) - (LARGEUR_CASE * (TAILLE / 2)));
					if(x < 0 || y < 0)
						x = y = -1;
					else
					{
						x = x / LARGEUR_CASE;
						y = y / LARGEUR_CASE;
					}

					if(NB_ESSAIS < MAX_ESSAIS && fin(grille, TAILLE) != 1)
					{
						if(x < TAILLE && y < TAILLE && x >= 0 && y >= 0)
						{
							choix = grille[y + TAILLE * x];
							couleurP = grille[0];
							if(choix != couleurP){
								NB_ESSAIS++;
								remplirGrille(grille, TAILLE, couleurP, choix, 0, 0);
								couleurP = grille[0];
							}
						}
						char lowerText[6];
						sprintf(lowerText, "%d-%d", NB_ESSAIS, MAX_ESSAIS);
						displayAll(renderer, grille, "FloodIt", lowerText);
					}
					if(NB_ESSAIS >= MAX_ESSAIS || fin(grille, TAILLE) == 1)
					{
						if(fin(grille, TAILLE) == 1)
							displayAll(renderer, grille, "You won", "Play again");
						else
							displayAll(renderer, grille, "You lost", "Play again");
						
						int playAgainX = LARGEUR_CASE * (TAILLE + 3) + WINDOW_L / 8;
						int playAgainY = WINDOW_H / 2;
						int playAgainW = LARGEUR_CASE * (TAILLE / 3);
						int playAgainH = WINDOW_H / 3 - LARGEUR_CASE * 2;
						if(event.button.x >= playAgainX && event.button.x <= playAgainX + playAgainW
							&& event.button.y >= playAgainY && event.button.y <= playAgainY + playAgainH)
							grille = playAgain();
					}

					break;

				case SDL_QUIT :  
					running = 0;
			}
		}	
		SDL_Delay(1); //  delai minimal
	}
}

/**
 * @brief Ferme la fenêtre ainsi que SDL et les polices
 * 
 * @param window 
 * 			La fenêtre à fermer
 */
void closeAllSDL(SDL_Window * window)
{
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/**
 * @brief Vérifie les arguments à l'appel du programme pour initialiser les paramètres.
 * 
 * @param argc
 * 			Le nombre d'arguments 
 * @param argv 
 * 			Le tableau d'arguments
 */
void checkArgs(int argc, char * argv[])
{
	int i;
	for(i=0; i < argc; i++)
	{
		if(strcmp(argv[i], "-t") == 0)
			TAILLE = atoi(argv[i + 1]);
		else
			if(strcmp(argv[i], "-c") == 0)
				COULEURS = atoi(argv[i + 1]);
			else
				if(strcmp(argv[i], "-e") == 0)
					MAX_ESSAIS = atoi(argv[i + 1]);
	}
	if(! TAILLE)
		TAILLE = 12;
	if(! COULEURS)
		COULEURS = 6;
	if(! MAX_ESSAIS)
		MAX_ESSAIS = 22;
}

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int * grille;

	checkArgs(argc, argv);
	
	grille = initialiserGrille(TAILLE, COULEURS);

	initiateSDL();

	window = initiateWindow("Floodit");
	
	renderer = initiateRenderer(window);

	initiateFont();

	adjustSizes();

	gameLoop(renderer, grille);

	closeAllSDL(window);
	free(grille);

	return 0;
}