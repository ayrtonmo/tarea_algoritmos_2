/**
 * @file base.h
 * @brief Constantes globales y prototipos del flujo principal del programa.
 */

#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "csv.h"
#include "errors.h"
#include "searching.h"
#include "sorting.h"
#include "print_format.h"

/**
 * @name Constantes de configuracion
 * @brief Limites y rutas usadas por el programa.
 *
 * Estas constantes centralizan limites de validacion y rutas a archivos usados
 * por el flujo interactivo y por los benchmarks.
 * @{
 */
#define NAME_LENGTH_MIN 3             /**< Largo minimo para nombres generados. */
#define NAME_LENGTH_MAX 8             /**< Largo maximo para nombres generados. */
#define MAX_DATA 2147483647           /**< Maximo de registros permitidos (int32). */
#define MIN_DATA 10                   /**< Minimo de registros permitidos. */
#define MAX_SCORE 100.0               /**< Puntaje maximo permitido. */
#define MIN_SCORE 0.0                 /**< Puntaje minimo permitido. */
#define MAX_COMPETITIONS 45           /**< Maximo de competencias generadas. */
#define MIN_COMPETITIONS 1            /**< Minimo de competencias generadas. */
#define INTERVAL_SIZE 50              /**< Tamano de intervalo usado en benchmarks. */
#define EXPERIMENT_REPEATS 5          /**< Repeticiones por escenario en benchmarks. */
#define CSV_ROUTE "./db/deportistas.csv"            /**< CSV principal de deportistas. */
#define SEARCH_BENCHMARK_ROUTE "./db/search_benchmark.csv"  /**< Salida de benchmark de busqueda. */
#define SORT_BENCHMARK_ROUTE "./db/sort_benchmark.csv"      /**< Salida de benchmark de ordenamiento. */
#define SELECT_BENCHMARK_ROUTE "./db/select_benchmark.csv"   /** Salida benchmark quick select */
#define RANGE_BENCHMARK_ROUTE "./db/range_benchmark.csv"     /** Salida benchmark busqueda por rango */
#define MERGE_THRESHOLD_BENCHMARK_ROUTE "./db/merge_threshold_benchmark.csv"  /**< Salida benchmark merge_insertion con thresholds */
#define MIN_MERGE_THRESHOLD_BENCHMARK_SIZE 100  /**< Cantidad minima de datos para ejecutar benchmark de merge_insertion con thresholds. */
/** @} */


/**
 * @brief Ejecuta el flujo interactivo de ordenamiento.
 */
void run_experiment(void);

/**
 * @brief Ejecuta el benchmark de algoritmos de busqueda.
 */
void run_search_benchmark();

/**
 * @brief Ejecuta el benchmark de algoritmos de ordenamiento.
 */
void run_sort_benchmark();

/**
 * @brief Ejecuta benchmark de merge_insertion variando el threshold.
 */
void run_merge_threshold_benchmark();

/**
 * @brief Ejecuta el benchmark de Quick Select (algoritmo de seleccion).
 */
void run_select_benchmark();

/**
 * @brief Ejecuta el benchmark de busqueda binaria por rango de puntaje.
 */
void run_range_benchmark();

/**
 * @brief Busca un deportista por su ID.
 *
 * @param targetId ID a buscar.
 */
void search_by_id(int targetId);

/**
 * @brief Muestra el ranking con los mejores deportistas por puntaje.
 *
 * @param rankingAmount Cantidad de deportistas a mostrar.
 */
void show_ranking(int rankingAmount);

/**
 * @brief Muestra el k-esimo mejor deportista segun puntaje usando Quick Select.
 *
 * @param k Posicion del deportista a mostrar, comenzando desde 1.
 */
void show_kth_best(int k);

/**
 * @brief Imprime la ayuda de uso del programa.
 *
 * @param programName Nombre del ejecutable.
 */
void print_help(const char *programName);

#endif
