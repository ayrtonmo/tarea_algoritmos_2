/**
 * @file errors.h
 * @brief Codigos y utilidades de manejo de errores de la aplicacion.
 */

#ifndef ERRORS_H
#define ERRORS_H

/**
 * @brief Codigos de error disponibles en la aplicacion.
 */
typedef enum {
    ERROR_INVALID_DATA_AMOUNT = 1,          /**< Cantidad solicitada fuera de rango. */
    ERROR_INVALID_RANKING_AMOUNT = 2,       /**< Ranking solicitado invalido (<= 0). */
    ERROR_NO_DATA_LOADED = 3,               /**< No se pudieron cargar datos desde el CSV. */
    ERROR_FILE_CREATE_FAILED = 4,           /**< No se pudo crear un archivo de salida. */
    ERROR_MEMORY_ALLOCATION_FAILED = 5,     /**< Fallo una reserva de memoria dinamica. */
    ERROR_BENCHMARK_DATA_LOAD_FAILED = 6,   /**< No se pudo cargar el dataset para benchmarking. */
    ERROR_UNKNOWN_OPTION = 7,               /**< Se recibio una opcion invalida por linea de comandos. */
    ERROR_NOT_IMPLEMENTED = 8,              /**< Opcion valida pero no implementada. */
    ERROR_DEPORTISTA_NOT_FOUND = 9          /**< No existe deportista para el ID solicitado. */
} ErrorCode;

/**
 * @brief Imprime un error centralizado a partir de un codigo documentado.
 *
 * @param errorCode Codigo del error a imprimir.
 * @param detail Texto adicional opcional para complementar el mensaje.
 */
void print_error(ErrorCode errorCode, const char *detail);

#endif
