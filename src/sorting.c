/**
 * @file sorting.c
 * @brief Utilidades compartidas por los algoritmos de ordenamiento.
 */

#include "sorting.h"
#include "deportista.h"

/**
 * @brief Particiona un arreglo de deportistas usando el esquema de Lomuto.
 *
 * Toma como pivote el ultimo elemento del rango indicado y reorganiza
 * los elementos para que queden a la izquierda o derecha del pivote
 * segun el criterio y orden recibidos.
 *
 * @param deportistas Arreglo de deportistas a particionar.
 * @param low Indice inicial del rango a particionar.
 * @param high Indice final del rango a particionar, usado como pivote.
 * @param criteria Criterio de ordenamiento utilizado para comparar deportistas.
 * @param order Orden de ordenamiento, ascendente o descendente.
 * @return int Posicion final del pivote dentro del arreglo.
 */
static int partition_lomuto(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order)
{
    Deportista pivot = deportistas[high];
    int i = low - 1;

    for(int j = low; j < high; j++) {
        int comparison = compare_by_criteria(
            deportistas[j],
            pivot,
            criteria
        );

        int shouldSwap = (order == ASCENDING && comparison <= 0) || (order == DESCENDING && comparison >= 0);

        if(shouldSwap) {
            i++;
            swap_deportistas(&deportistas[i], &deportistas[j]);
        }
    }

    swap_deportistas(&deportistas[i + 1], &deportistas[high]);

    return i + 1;
}

/**
 * @brief Aplica Quick Sort de forma recursiva sobre un rango del arreglo.
 *
 * Divide el arreglo usando la particion de Lomuto y luego ordena
 * recursivamente los subarreglos ubicados antes y despues del pivote.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param low Indice inicial del rango a ordenar.
 * @param high Indice final del rango a ordenar.
 * @param criteria Criterio de ordenamiento utilizado para comparar deportistas.
 * @param order Orden de ordenamiento, ascendente o descendente.
 */
static void quick_sort_recursive(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order)
{
    if(low < high) {
        int pivotIndex = partition_lomuto(deportistas, low, high, criteria, order);

        quick_sort_recursive(deportistas, low, pivotIndex - 1, criteria, order);

        quick_sort_recursive(deportistas, pivotIndex + 1, high, criteria, order);
    }
}

/**
 * @brief Ordena un arreglo de deportistas utilizando Quick Sort.
 *
 * Valida que el arreglo tenga datos suficientes y luego llama a la
 * funcion recursiva de Quick Sort para ordenar todos los deportistas.
 * Actualmente utiliza particion de Lomuto con pivote en el ultimo elemento.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param count Cantidad de deportistas almacenados en el arreglo.
 * @param criteria Criterio de ordenamiento utilizado para comparar deportistas.
 * @param order Orden de ordenamiento, ascendente o descendente.
 * @param pivotType Tipo de pivote solicitado para Quick Sort.
 */
void quick_sort_deportistas(Deportista *deportistas, int count, SortCriteria criteria, SortOrder order, PivotType pivotType)
{
    (void)pivotType; //momentaneo

    if(deportistas == NULL || count < 2) {
        return;
    }

    quick_sort_recursive(deportistas, 0, count - 1, criteria, order);
}

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
