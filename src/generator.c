/**
 * @file generator.c
 * @brief Implementacion de generadores aleatorios para deportistas.
 */

#include "generator.h"
#include "base.h"

/**
 * @brief Equipos disponibles para la generacion aleatoria.
 */
static const char *teamNames[] = {
    "T1",
    "Damwon Gaming",
    "Gen.G",
    "Hanwha Life Esports",
    "Fnatic",
    "G2 Esports",
    "MAD Lions",
    "Rogue",
    "Cloud9",
    "Immortals",
    "FlyQuest",
    "100 Thieves",
    "Sentinels"
};

/**
 * @brief Genera un nombre aleatorio usando solo letras ASCII.
 *
 * @return char* Cadena dinamica con el nombre generado.
 */
char *generate_random_name(void)
{
    const char letters[] = "abcdefghijklmnopqrstuvwxyz";
    int n = (int)strlen(letters);
    int nameLength = (rand() % (NAME_LENGTH_MAX - NAME_LENGTH_MIN + 1)) + NAME_LENGTH_MIN;
    char *name = malloc((size_t)nameLength + 1);

    if(name == NULL) {
        return NULL;
    }

    for(int i = 0; i < nameLength; i++) {
        name[i] = letters[rand() % n];
    }

    name[nameLength] = '\0';
    return name;
}

/**
 * @brief Genera un equipo aleatorio.
 *
 * @return char* Cadena dinamica con el nombre del equipo.
 */
char *generate_random_team(void)
{
    int n = (int)(sizeof(teamNames) / sizeof(teamNames[0]));
    const char *selected = teamNames[rand() % n];
    char *team = malloc(strlen(selected) + 1);

    if(team == NULL) {
        return NULL;
    }

    strcpy(team, selected);
    return team;
}

/**
 * @brief Genera un puntaje aleatorio dentro del rango permitido.
 *
 * @return float Puntaje aleatorio.
 */
float generate_random_score(void)
{
    return ((float)rand() / RAND_MAX) * (MAX_SCORE - MIN_SCORE) + MIN_SCORE;
}

/**
 * @brief Genera una cantidad aleatoria de competencias.
 *
 * @return int Cantidad de competencias.
 */
int generate_random_competitions(void)
{
    return (rand() % (MAX_COMPETITIONS - MIN_COMPETITIONS + 1)) + MIN_COMPETITIONS;
}

/**
 * @brief Mezcla un arreglo de enteros usando Fisher-Yates.
 *
 * @param values Arreglo a mezclar.
 * @param length Cantidad de elementos.
 */
void fisher_yates_shuffle(int *values, int length)
{
    int i, j, temp;

    if(values == NULL) {
        return;
    }

    for(i = length - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = values[i];
        values[i] = values[j];
        values[j] = temp;
    }
}
