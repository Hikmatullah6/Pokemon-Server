// common.c
#include <stdio.h>
#include "pokemon.h"

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
