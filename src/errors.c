/**
 * @file errors.c
 * @brief Implementacion del manejo centralizado de errores.
 */

#include <stdio.h>

#include "errors.h"
#include "print_format.h"

/**
 * @brief Traduce un codigo de error al mensaje base correspondiente.
 *
 * @param errorCode Codigo del error.
 * @return const char* Mensaje base asociado.
 */
static const char *get_error_message(ErrorCode errorCode)
{
    switch(errorCode) {
        case ERROR_INVALID_DATA_AMOUNT:
            return "Cantidad de datos invalida";
        case ERROR_INVALID_RANKING_AMOUNT:
            return "La cantidad del ranking debe ser mayor que 0";
        case ERROR_NO_DATA_LOADED:
            return "No hay deportistas cargados en el CSV";
        case ERROR_FILE_CREATE_FAILED:
            return "No se pudo crear el archivo solicitado";
        case ERROR_MEMORY_ALLOCATION_FAILED:
            return "No se pudo reservar memoria";
        case ERROR_BENCHMARK_DATA_LOAD_FAILED:
            return "No se pudieron cargar deportistas para el benchmark";
        case ERROR_UNKNOWN_OPTION:
            return "Opcion desconocida";
        case ERROR_NOT_IMPLEMENTED:
            return "La opcion seleccionada aun no esta implementada";
        case ERROR_DEPORTISTA_NOT_FOUND:
            return "No se encontro el deportista solicitado";
        default:
            return "Error no identificado";
    }
}

void print_error(ErrorCode errorCode, const char *detail)
{
    const char *message = get_error_message(errorCode);

    fprintf(stderr, RED BOLD "\n[ERROR]" RESET RED " %s", message);

    if(detail != NULL && detail[0] != '\0') {
        fprintf(stderr, ": %s", detail);
    }

    fprintf(stderr, "\n\n" RESET);
}
