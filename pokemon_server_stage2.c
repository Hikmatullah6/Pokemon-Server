#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_POKEMONS 1000
#define MAX_NAME_LEN 50
#define MAX_TYPE_LEN 30
#define MAX_THREADS 10

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char type1[MAX_TYPE_LEN];
    char type2[MAX_TYPE_LEN];
    int total;
    int hp, attack, defense;
    int sp_atk, sp_def, speed;
    int generation;
    int legendary;
} Pokemon;

typedef struct {
    char type[MAX_TYPE_LEN];
    Pokemon *pokemons;
    int total;
} SearchArgs;

// Global results array for all queries
Pokemon allResults[MAX_POKEMONS];
int totalResults = 0;

// Mutex for synchronizing writes to the results array
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Threads array to track running queries
pthread_t threads[MAX_THREADS];
int threadCount = 0;

// Function to load Pokémon data from CSV
int loadPokemonData(const char *filename, Pokemon pokemons[], int max) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    char line[256];
    int count = 0;
    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp) && count < max) {
        sscanf(line, "%d,%49[^,],%29[^,],%29[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d",
               &pokemons[count].id,
               pokemons[count].name,
               pokemons[count].type1,
               pokemons[count].type2,
               &pokemons[count].total,
               &pokemons[count].hp,
               &pokemons[count].attack,
               &pokemons[count].defense,
               &pokemons[count].sp_atk,
               &pokemons[count].sp_def,
               &pokemons[count].speed,
               &pokemons[count].generation,
               &pokemons[count].legendary);
        count++;
    }
    fclose(fp);
    return count;
}

// Background thread function for type search
void *typeSearchThread(void *arg) {
    SearchArgs *args = (SearchArgs *)arg;
    int count = 0;

    for (int i = 0; i < args->total; i++) {
        if (strcmp(args->pokemons[i].type1, args->type) == 0) {
            pthread_mutex_lock(&lock);
            allResults[totalResults++] = args->pokemons[i];
            pthread_mutex_unlock(&lock);
            count++;
        }
    }

    printf("Background Search Complete: Found %d Pokémon(s) of type %s\n", count, args->type);
    free(args);
    pthread_exit(NULL);
}

int main() {
    Pokemon pokemons[MAX_POKEMONS];
    int totalPokemon = 0;
    char filename[100];

    while (1) {
        printf("Enter the name of the Pokemon CSV file: ");
        scanf("%s", filename);
        totalPokemon = loadPokemonData(filename, pokemons, MAX_POKEMONS);
        if (totalPokemon == -1) {
            printf("Pokemon file is not found. Please enter the name of the file again or type 'exit'.\n");
            continue;
        }
        break;
    }

    int choice, successfulQueries = 0;
    char createdFiles[10][100];
    int fileCount = 0;

    while (1) {
        printf("\nMenu:\n1. Type Search\n2. Save Results\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            if (threadCount >= MAX_THREADS) {
                printf("Maximum number of background queries reached!\n");
                continue;
            }

            char searchType[MAX_TYPE_LEN];
            printf("Enter Type 1 to search: ");
            scanf("%s", searchType);

            SearchArgs *args = malloc(sizeof(SearchArgs));
            if (!args) {
                printf("Unable to start the query!\n");
                exit(1);
            }

            strcpy(args->type, searchType);
            args->pokemons = pokemons;
            args->total = totalPokemon;

            if (pthread_create(&threads[threadCount++], NULL, typeSearchThread, args) != 0) {
                printf("Unable to start the query!\n");
                exit(1);
            } else {
                printf("Search started in background...\n");
                successfulQueries++;
            }
        }
        else if (choice == 2) {
            for (int i = 0; i < threadCount; i++) {
                pthread_join(threads[i], NULL); // Wait for all background queries to complete
            }

            char saveFile[100];
            FILE *out;
            while (1) {
                printf("Enter name of file to save results: ");
                scanf("%s", saveFile);
                out = fopen(saveFile, "w");
                if (!out) {
                    printf("Unable to create the new file. Please try again.\n");
                } else {
                    break;
                }
            }

            for (int i = 0; i < totalResults; i++) {
                fprintf(out, "%s,%s,%s\n", allResults[i].name, allResults[i].type1, allResults[i].type2);
            }
            fclose(out);
            strcpy(createdFiles[fileCount++], saveFile);
            printf("Results saved successfully.\n");

            // Reset for future results
            totalResults = 0;
            threadCount = 0;
        }
        else if (choice == 3) {
            for (int i = 0; i < threadCount; i++) {
                pthread_cancel(threads[i]);
            }

            printf("Total successful queries: %d\n", successfulQueries);
            printf("Files created:\n");
            for (int i = 0; i < fileCount; i++) {
                printf("- %s\n", createdFiles[i]);
            }
            printf("Exiting program...\n");
            break;
        }
        else {
            printf("Invalid option. Please choose 1, 2 or 3.\n");
        }
    }

    return 0;
}
