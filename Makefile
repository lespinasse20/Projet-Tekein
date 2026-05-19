# --- Parametres du projet ---
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = projet_teiken

# --- Liste des fichiers ---
SRC = main.c fonctions.c
OBJ = $(SRC:.c=.o)

# --- Regles de compilation ---
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

main.o: main.c structures.h
	$(CC) $(CFLAGS) -c main.c

fonctions.o: fonctions.c structures.h
	$(CC) $(CFLAGS) -c fonctions.c

# --- Regle de nettoyage hybride ---
clean:
ifeq ($(OS), Windows_NT)
	@echo "Nettoyage Windows..."
	del /f $(OBJ) $(TARGET).exe
else
	@echo "Nettoyage Linux..."
	rm -f $(OBJ) $(TARGET)
endif
