// pokemon.h
#ifndef POKEMON_H
#define POKEMON_H

#define MAX_POKEMONS 1000
#define MAX_NAME_LEN 50
#define MAX_TYPE_LEN 30

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

// Shared function declaration
int loadPokemonData(const char *filename, Pokemon pokemons[], int max);

// POKEMON_H
#endif