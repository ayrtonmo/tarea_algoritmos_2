/**
 * @file sorting.h
 * @brief Ordenamiento para deportistas.
 */

#ifndef SORTING_H
#define SORTING_H

#include "deportista.h"
#include <string.h>
#include <stdlib.h>

/**
 * @brief Criterios disponibles para ordenar deportistas.
 */
typedef enum {
    SORT_BY_ID = 1,             /**< Ordenar/Comparar por ID. */
    SORT_BY_PUNTAJE = 2,        /**< Ordenar/Comparar por puntaje. */
    SORT_BY_COMPETENCIAS = 3,   /**< Ordenar/Comparar por competencias. */
    SORT_BY_NOMBRE = 4,         /**< Ordenar/Comparar por nombre (lexicografico). */
    SORT_BY_EQUIPO = 5          /**< Ordenar/Comparar por equipo (lexicografico). */
} SortCriteria;

/**
 * @brief Sentidos de ordenamiento disponibles.
 */
typedef enum {
    DESCENDING = 1, /**< Mayor a menor. */
    ASCENDING = 0   /**< Menor a mayor. */
} SortOrder;

/**
 * @brief Algoritmos de ordenamiento disponibles.
 */
typedef enum {
    QUICK_SORT = 1,
    MERGE_SORT = 2
} SortAlgorithm;

/**
 * @brief Algoritmos de busqueda disponibles.
 */
typedef enum {
    SEQUENTIAL_SEARCH = 1,
    BINARY_SEARCH = 2,
    EXPONENTIAL_SEARCH = 3,
    INTERPOLATION_SEARCH = 4
} SearchAlgorithm;

/**
 * @brief Criterios disponibles para la busqueda.
 */
typedef enum {
    SEARCH_BY_ID = 1
} SearchCriteria;

/**
 * @brief Tipos de pivote disponibles para la busqueda.
 */
typedef enum {
    PIVOT_LAST = 1,
    PIVOT_FIRST = 2,
    PIVOT_RANDOM = 3,
    PIVOT_MEDIAN_OF_THREE = 4
} PivotType;

int compare_by_criteria(Deportista left, Deportista right, SortCriteria criteria);
void quick_sort_deportistas(Deportista *deportistas, int count, SortCriteria criteria, SortOrder order, PivotType pivotType);
Deportista quick_select_deportista(Deportista *deportistas, int count, int k, SortCriteria criteria, SortOrder order, PivotType pivotType);
void merge_sort(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order, Deportista *temp);
void merge_insertion(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order, int threshold, Deportista *temp);
void merge(Deportista *deportistas, int low, int mid, int high, SortCriteria criteria, SortOrder order, Deportista *temp);
void insertion_sort_deportistas(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order);

#endif
