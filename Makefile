#CC Compilateur utilisé
CC=gcc

#C_FLAGS options de compilation
CFLAGS = -Wall -Wextra -I include

#SDL_FLAGS options pour SDL : bibliothèques pour l'édition de lien et autre
SDL_FLAGS = -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#EXEC chemin et nom de l'exécutable
EXEC= bin\jeu

#Dossier des fichiers source
SRC = src/

#Edition de liens pour créer l'executable
${EXEC}: modele.o jeu.o
	${CC} -o ${EXEC} modele.o jeu.o ${SDL_FLAGS}

#compilation et assemblage de fichier modele.c en fichier objet
modele.o: ${SRC}modele.h
	${CC} ${CFLAGS} ${SRC}modele.c -c

#compilation et assemblage de fichier jeu.c en fichier objet
jeu.o: ${SRC}modele.h
	${CC} ${CFLAGS} ${SRC}jeu.c -c

# Permet de supprimer tout les fichiers intermédiaires
clean:
	del *.o *.gch

# Supprime tout ce qui peut être régénéré et permet une reconstruction complète du projet
mrproper: clean
	del ${EXEC}.exe