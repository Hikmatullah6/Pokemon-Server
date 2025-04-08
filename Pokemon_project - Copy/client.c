// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "pokemon.h"

#define SERVER_IP "127.0.0.1"
#define PORT 80

Pokemon results[MAX_POKEMONS];
int totalResults = 0;

void searchPokemon() {
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        printf("Unable to establish connection to the PPS!\n");
        exit(1);
    }

    char searchType[MAX_TYPE_LEN];
    printf("Enter Type 1 to search: ");
    scanf("%s", searchType);

    write(socketFD, searchType, sizeof(searchType));

    Pokemon p;
    totalResults = 0;
    while (read(socketFD, &p, sizeof(Pokemon)) > 0) {
        if (strlen(p.name) == 0) break; // end signal
        results[totalResults++] = p;
    }

    printf("Search completed. Found %d Pokemon(s).\n", totalResults);

    close(socketFD);
}

void saveResults() {
    char saveFile[100];
    printf("Enter name of file to save results: ");
    scanf("%s", saveFile);

    FILE *out = fopen(saveFile, "w");
    if (!out) {
        printf("Unable to create the new file. Please try again.\n");
        return;
    }

    for (int i = 0; i < totalResults; i++) {
        fprintf(out, "%s,%s,%s\n", results[i].name, results[i].type1, results[i].type2);
    }

    fclose(out);
    printf("Results saved successfully.\n");
}

int main() {
    int choice;

    while (1) {
        printf("\nMenu:\n1. Type Search\n2. Save Results\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            searchPokemon();
        } else if (choice == 2) {
            saveResults();
        } else if (choice == 3) {
            printf("Exiting program...\n");
            break;
        } else {
            printf("Invalid option. Please choose 1, 2 or 3.\n");
        }
    }

    return 0;
}