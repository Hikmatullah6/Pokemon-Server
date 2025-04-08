// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "pokemon.h"

#define PORT 80

Pokemon pokemons[MAX_POKEMONS];
int totalPokemon = 0;

void handleClient(int clientSocket) {
    char searchType[MAX_TYPE_LEN];
    int bytesRead = read(clientSocket, searchType, sizeof(searchType));
    if (bytesRead <= 0) {
        close(clientSocket);
        return;
    }

    printf("Received search request for type: %s\n", searchType);

    for (int i = 0; i < totalPokemon; i++) {
        if (strcmp(pokemons[i].type1, searchType) == 0) {
            write(clientSocket, &pokemons[i], sizeof(Pokemon));
        }
    }

    // Send end signal
    Pokemon endSignal;
    memset(&endSignal, 0, sizeof(Pokemon));
    write(clientSocket, &endSignal, sizeof(Pokemon));

    close(clientSocket);
}

int main() {
    char filename[100];
    printf("Enter the name of the Pokemon CSV file: ");
    scanf("%s", filename);

    totalPokemon = loadPokemonData(filename, pokemons, MAX_POKEMONS);
    if (totalPokemon == -1) {
        printf("Pokemon file is not found.\n");
        return 1;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            perror("Accept failed");
            continue;
        }

        handleClient(clientSocket);
    }

    close(serverSocket);
    return 0;
}