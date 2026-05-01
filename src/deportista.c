/**
 * @file deportista.c
 * @brief Implementacion del tipo Deportista y sus utilidades.
 */

#include "deportista.h"

/**
 * @brief Crea una instancia dinamica de deportista.
 *
 * @param id Identificador del deportista.
 * @param nombre Nombre del deportista.
 * @param equipo Equipo del deportista.
 * @param puntaje Puntaje del deportista.
 * @param competencias Cantidad de competencias.
 * @return Deportista Instancia creada o NULL si falla la reserva.
 */
Deportista create_deportista(int id, char *nombre, char *equipo, float puntaje, int competencias)
{
    Deportista newDeportista = malloc(sizeof(DeportistaData));

    if(newDeportista == NULL) {
        return NULL;
    }

    newDeportista->id = id;
    newDeportista->nombre = nombre;
    newDeportista->equipo = equipo;
    newDeportista->puntaje = puntaje;
    newDeportista->competencias = competencias;

    return newDeportista;
}

/**
 * @brief Elimina una instancia de deportista, liberando la memoria asociada.
 * 
 * @param deportista Deportista a eliminar.
 */
void delete_deportista(Deportista deportista)
{
    if(deportista == NULL) {
        return;
    }

    free(deportista->nombre);
    free(deportista->equipo);
    free(deportista);
}

/**
 * @brief Imprime un deportista.
 *
 * @param deportista Deportista a imprimir.
 */
void print_deportista(Deportista deportista)
{
    if(deportista == NULL) {
        return;
    }

    printf("ID: %d | Nombre: %s | Equipo: %s | Puntaje: %.2f | Competencias: %d\n",
        deportista->id,
        deportista->nombre,
        deportista->equipo,
        deportista->puntaje,
        deportista->competencias);
}

/**
 * @brief Intercambia los datos de dos deportistas.
 * 
 * @param left Deportista izquierdo a intercambiar.
 * @param right Deportista derecho a intercambiar.
 */
void swap_deportistas(Deportista *left, Deportista *right)
{
    Deportista tmp = *left;
    *left = *right;
    *right = tmp;
}
