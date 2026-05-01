/**
 * @file print_format.c
 * @brief Implementacion de utilidades de formato.
 */

#include "print_format.h"

/**
 * @brief Imprime una lista de deportistas.
 *
 * @param deportistas Array de deportistas a imprimir.
 * @param rankingAmount Número de deportistas a imprimir.
 */
void print_deportistas_array(Deportista *deportistas, int rankingAmount)
{
    if(deportistas == NULL || rankingAmount <= 0) {
        return;
    }

    for(int i = 0; i < rankingAmount; i++) {
        print_deportista(deportistas[i]);
    }
}
