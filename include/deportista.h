/**
 * @file deportista.h
 * @brief Declaraciones del tipo Deportista y sus operaciones basicas.
 */

#ifndef DEPORTISTA_H
#define DEPORTISTA_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Estructura que representa a un deportista.
 */
typedef struct DeportistaData {
    int id;             /**< Identificador unico del deportista. */
    char *nombre;       /**< Nombre (cadena dinamica, propiedad del deportista). */
    char *equipo;       /**< Equipo (cadena dinamica, propiedad del deportista). */
    float puntaje;      /**< Puntaje acumulado del deportista. */
    int competencias;   /**< Cantidad de competencias disputadas. */
} DeportistaData;

/**
 * @brief Alias de puntero para manipular deportistas dinamicos.
 */
typedef DeportistaData *Deportista;

/**
 * @brief Crea un deportista con los datos entregados.
 *
 * La funcion reserva memoria para la estructura y para las cadenas entregadas.
 * El deportista resultante debe liberarse con delete_deportista().
 *
 * @param id Identificador unico del deportista.
 * @param nombre Nombre del deportista.
 * @param equipo Equipo al que pertenece.
 * @param puntaje Puntaje acumulado.
 * @param competencias Cantidad de competencias disputadas.
 * @return Deportista Nuevo deportista creado o NULL si falla la reserva.
 */
Deportista create_deportista(
    int id,
    char *nombre,
    char *equipo,
    float puntaje,
    int competencias
);

/**
 * @brief Libera la memoria asociada a un deportista.
 *
 * Libera la estructura y las cadenas internas asociadas.
 *
 * @param deportista Deportista a destruir.
 */
void delete_deportista(Deportista deportista);

/**
 * @brief Intercambia dos posiciones de un arreglo de deportistas.
 *
 * @param left Direccion del primer deportista.
 * @param right Direccion del segundo deportista.
 */
void swap_deportistas(Deportista *left, Deportista *right);

/**
 * @brief Imprime un deportista por consola.
 *
 * @param deportista Deportista a imprimir.
 */
void print_deportista(Deportista deportista);

#endif
