/**
 * @file sorting.c
 * @brief Utilidades compartidas por los algoritmos de ordenamiento.
 */

#include "sorting.h"

/**
 * @brief Compara dos deportistas segun un criterio de ordenamiento.
 *
 * @param left Deportista izquierdo.
 * @param right Deportista derecho.
 * @param criteria Criterio a utilizar para la comparacion.
 * @return int Resultado de la comparacion: -1 si left < right, 1 si left > right, 0 si son iguales.
 */
int compare_by_criteria(Deportista left, Deportista right, SortCriteria criteria)
{
    if(left == NULL || right == NULL) {
        return 0;
    }

    switch(criteria){
        case SORT_BY_PUNTAJE:
            if(left->puntaje < right->puntaje) {
                return -1;
            }

            if(left->puntaje > right->puntaje) {
                return 1;
            }

            return 0;

        case SORT_BY_ID:
            if(left->id < right->id) {
                return -1;
            }

            if(left->id > right->id) {
                return 1;
            }

            return 0;

        case SORT_BY_COMPETENCIAS:
            if(left->competencias < right->competencias) {
                return -1;
            }

            if(left->competencias > right->competencias) {
                return 1;
            }

            return 0;

        case SORT_BY_NOMBRE:
            return strcmp(left->nombre, right->nombre);

        case SORT_BY_EQUIPO:
            return strcmp(left->equipo, right->equipo);

        default:
            return 0;
    }
}
