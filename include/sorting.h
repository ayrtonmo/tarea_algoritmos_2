/**
 * @file sorting.h
 * @brief Ordenamiento para deportistas.
 */

#ifndef SORTING_H
#define SORTING_H

#include "deportista.h"
#include <string.h>

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
    INSERTION_SORT = 1,        /**< Insertion sort. */
    BUBBLE_SORT = 2,           /**< Bubble sort optimizado. */
    SELECTION_SORT = 3,        /**< Selection sort optimizado. */
    COCKTAIL_SHAKER_SORT = 4   /**< Cocktail shaker sort. */
} SortAlgorithm;

/**
 * @brief Algoritmos de busqueda disponibles.
 */
typedef enum {
    SEQUENTIAL_SEARCH = 1, /**< Busqueda secuencial. */
    BINARY_SEARCH = 2      /**< Busqueda binaria (requiere arreglo ordenado). */
} SearchAlgorithm;

/**
 * @brief Criterios disponibles para la busqueda.
 */
typedef enum {
    SEARCH_BY_ID = 1 /**< Buscar por ID. */
} SearchCriteria;

#endif
