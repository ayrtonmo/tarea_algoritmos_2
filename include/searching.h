/**
 * @file searching.h
 * @brief Algoritmos de busqueda sobre arreglos de deportistas.
 */

#ifndef SEARCHING_H
#define SEARCHING_H

#include "deportista.h"

void sort_deportistas_by_id_ascending(Deportista *deportistas, int count);

int binary_search_by_id_recursive(Deportista *deportistas, int left, int right, int targetId);
int interpolation_search_by_id(Deportista* deportistas, int count, int targetId);
int exponential_search_by_id(Deportista *deportistas, int count, int targetId);
int binary_search_by_score(Deportista *deportistas, int left, int right, float targetScore);
int binary_search_by_id(Deportista *deportistas, int left, int right, int targetId);
int binary_search_by_range(Deportista *deportistas, int count, float lowScore, float highScore, int *outLeft, int *outRight);

#endif
