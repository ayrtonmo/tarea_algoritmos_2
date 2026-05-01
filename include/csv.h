/**
 * @file csv.h
 * @brief Operaciones para generar y cargar datos CSV de deportistas.
 */

#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "deportista.h"

/**
 * @brief Genera un archivo CSV con datos aleatorios de deportistas.
 *
 * @param dataAmount Cantidad de registros a generar.
 */
void create_deportistas_csv(int dataAmount);

/**
 * @brief Carga los deportistas desde el CSV configurado.
 *
 * @param count Direccion donde se almacena la cantidad cargada.
 * @return Deportista* Arreglo dinamico de deportistas o NULL si falla la carga.
 */
Deportista *load_deportistas_array(int *count);

/**
 * @brief Libera un arreglo dinamico de deportistas.
 *
 * @param deportistas Arreglo a liberar.
 * @param count Cantidad de elementos almacenados.
 */
void free_deportistas_array(Deportista *deportistas, int count);

#endif
