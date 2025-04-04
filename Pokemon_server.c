#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_POKEMONS 1000 // Maximum number of Pokémon that can be loaded
#define MAX_NAME_LEN 50   // Maximum length of Pokémon name
#define MAX_TYPE_LEN 30   // Maximum length for Pokémon types

// Structure to store Pokemon data
typedef struct
{
    int id;                   // Unique Pokémon identifier
    char name[MAX_NAME_LEN];  // Pokémon name
    char type1[MAX_TYPE_LEN]; // Primary type of the Pokémon
    char type2[MAX_TYPE_LEN]; // Secondary type of the Pokémon
    int total;                // Total stats (sum of other attributes)
    int hp;                   // Health points
    int attack;               // Attack stat
    int defense;              // Defense stat
    int sp_atk;               // Special attack stat
    int sp_def;               // Special defense stat
    int speed;                // Speed stat
    int generation;           // Pokémon generation number
    int legendary;            // Whether the Pokémon is legendary (1 for true, 0 for false)
} Pokemon;

// Function to load Pokémon data from a CSV file
int loadPokemonData(const char *filename, Pokemon pokemons[], int max)
{
    FILE *fp = fopen(filename, "r"); // Open the file for reading
    if (!fp)                         // If the file doesn't exist or can't be opened, return -1
        return -1;

    char line[256];                // Buffer for reading each line of the file
    int count = 0;                 // Variable to track the number of Pokémon read
    fgets(line, sizeof(line), fp); // Skip the header line in the CSV file

    // Read each line and store the data into the pokemons array
    while (fgets(line, sizeof(line), fp) && count < max)
    {
        // Use sscanf to parse the CSV line and store the data into the structure
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
        count++; // Increment the count of Pokémon loaded
    }
    fclose(fp);   // Close the file after reading
    return count; // Return the number of Pokémon successfully loaded
}

// Main program
int main()
{
    Pokemon pokemons[MAX_POKEMONS];        // Array to store all loaded Pokémon
    Pokemon results[MAX_POKEMONS];         // Array to store the search results
    int totalPokemon = 0, resultCount = 0; // Track total number of loaded Pokémon and search results
    char filename[100];                    // Variable to hold the name of the input file

    // File open loop: Keep asking for the file name until a valid file is loaded
    while (1)
    {
        printf("Enter the name of the Pokemon CSV file: ");
        scanf("%s", filename);                                            // Get the file name from user input
        totalPokemon = loadPokemonData(filename, pokemons, MAX_POKEMONS); // Load the Pokémon data

        if (totalPokemon == -1) // If the file couldn't be loaded, show an error
        {
            printf("Pokemon file is not found. Please enter the name of the file again or type 'exit'.\n");
            continue; // Ask for the file name again
        }
        break; // If file is successfully loaded, exit the loop
    }

    // Menu loop: Displays options for the user to search, save, or exit
    int choice, successfulQueries = 0; // Variables to track user choice and successful queries
    char createdFiles[10][100];        // Array to store names of files created
    int fileCount = 0;                 // Counter for the number of files created

    while (1)
    {
        // Display the menu and ask for the user's choice
        printf("\nMenu:\n1. Type Search\n2. Save Results\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) // If the user selects 'Type Search'
        {
            char searchType[MAX_TYPE_LEN]; // Store the Pokémon type to search
            printf("Enter Type 1 to search: ");
            scanf("%s", searchType); // Get the search type from the user
            resultCount = 0;         // Reset the result count

            // Search through all loaded Pokémon for the specified type
            for (int i = 0; i < totalPokemon; i++)
            {
                if (strcmp(pokemons[i].type1, searchType) == 0) // Compare type1 with search type
                {
                    results[resultCount++] = pokemons[i]; // Add matching Pokémon to the results
                }
            }

            printf("Found %d Pokemon(s) of type %s.\n", resultCount, searchType); // Display the search results
            successfulQueries++;                                                  // Increment the count of successful queries
        }
        else if (choice == 2) // If the user selects 'Save Results'
        {
            char saveFile[100]; // Variable to hold the file name to save the results
            FILE *out;          // File pointer for saving the results
            while (1)
            {
                printf("Enter name of file to save results: ");
                scanf("%s", saveFile);      // Get the save file name from the user
                out = fopen(saveFile, "w"); // Open the file for writing
                if (!out)                   // If the file couldn't be created, show an error
                {
                    printf("Unable to create the new file. Please try again.\n");
                }
                else
                    break; // Exit the loop once the file is successfully opened
            }

            // Write the search results to the file
            for (int i = 0; i < resultCount; i++)
            {
                fprintf(out, "%s,%s,%s\n", results[i].name, results[i].type1, results[i].type2);
            }
            fclose(out);                                 // Close the file after writing
            strcpy(createdFiles[fileCount++], saveFile); // Store the created file name
            printf("Results saved successfully.\n");
        }
        else if (choice == 3) // If the user selects 'Exit'
        {
            // Display the summary of the session before exiting
            printf("Total successful queries: %d\n", successfulQueries);
            printf("Files created:\n");
            for (int i = 0; i < fileCount; i++)
            {
                printf("- %s\n", createdFiles[i]);
            }
            printf("Exiting program...\n");
            break; // Exit the program
        }
        else // If the user enters an invalid option
        {
            printf("Invalid option. Please choose 1, 2 or 3.\n");
        }
    }

    return 0; // Exit the program successfully
}
