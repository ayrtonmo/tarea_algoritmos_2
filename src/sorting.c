/**
 * @file sorting.c
 * @brief Utilidades compartidas por los algoritmos de ordenamiento.
 */

#include "sorting.h"
#include "deportista.h"

/**
 * @brief Selecciona el indice del pivote segun la variante de Quick Sort.
 *
 * Para mantener el esquema de particion Lomuto, la funcion solo elige
 * el indice del pivote. Luego, el pivote seleccionado se intercambia
 * con la posicion high antes de particionar.
 *
 * @param deportistas Arreglo de deportistas.
 * @param low Indice inicial del rango actual.
 * @param high Indice final del rango actual.
 * @param criteria Criterio utilizado para comparar deportistas.
 * @param pivotType Variante de pivote seleccionada.
 * @return int Indice del pivote seleccionado.
 */
static int choose_pivot_index(Deportista *deportistas, int low, int high, SortCriteria criteria, PivotType pivotType)
{
    switch(pivotType) {
        case PIVOT_FIRST:
            return low;

        case PIVOT_RANDOM:
            return low + rand() % (high - low + 1);

        case PIVOT_MEDIAN_OF_THREE:
        {
            int mid = low + (high - low) / 2;

            Deportista first = deportistas[low];
            Deportista middle = deportistas[mid];
            Deportista last = deportistas[high];

            int firstMiddle = compare_by_criteria(first, middle, criteria);
            int firstLast = compare_by_criteria(first, last, criteria);
            int middleLast = compare_by_criteria(middle, last, criteria);

            if((firstMiddle >= 0 && firstLast <= 0) ||
                (firstMiddle <= 0 && firstLast >= 0)) {
                return low;
            }

            if((firstMiddle <= 0 && middleLast >= 0) ||
                (firstMiddle >= 0 && middleLast <= 0)) {
                return mid;
            }

            return high;
        }

        case PIVOT_LAST:
        default:
            return high;
    }
}

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
        int comparison = compare_by_criteria(deportistas[j], pivot, criteria);

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
static void quick_sort_recursive(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order, PivotType pivotType)
{
    if(low < high) {
        int pivotIndexToUse = choose_pivot_index(deportistas, low, high, criteria, pivotType);

        swap_deportistas( &deportistas[pivotIndexToUse], &deportistas[high] );

        int pivotIndex = partition_lomuto( deportistas, low, high, criteria, order );

        quick_sort_recursive( deportistas, low, pivotIndex - 1, criteria, order, pivotType );

        quick_sort_recursive( deportistas, pivotIndex + 1, high, criteria, order, pivotType );
    }
}

/**
 * @brief Aplica Quick Select de forma recursiva sobre un rango del arreglo.
 *
 * TODO: explicar brevemente maybe?
 *
 * @param deportistas Arreglo de deportistas.
 * @param low Indice inicial del rango actual.
 * @param high Indice final del rango actual.
 * @param targetIndex Indice objetivo que se desea encontrar.
 * @param criteria Criterio utilizado para comparar deportistas.
 * @param order Orden de comparacion, ascendente o descendente.
 * @param pivotType Variante de pivote utilizada.
 * @return int Indice donde queda ubicado el elemento seleccionado.
 */
static int quick_select_recursive(Deportista *deportistas, int low, int high, int targetIndex, SortCriteria criteria, SortOrder order, PivotType pivotType)
{
    if(low == high) {
        return low;
    }

    int pivotIndexToUse = choose_pivot_index( deportistas, low, high, criteria, pivotType );

    swap_deportistas( &deportistas[pivotIndexToUse], &deportistas[high] );

    int pivotIndex = partition_lomuto( deportistas, low, high, criteria, order );

    if(pivotIndex == targetIndex) {
        return pivotIndex;
    }

    if(targetIndex < pivotIndex) {
        return quick_select_recursive( deportistas, low, pivotIndex - 1, targetIndex, criteria, order, pivotType );
    }

    return quick_select_recursive( deportistas, pivotIndex + 1, high, targetIndex, criteria, order, pivotType );
}

/**
 * @brief Obtiene el k-esimo deportista segun un criterio usando Quick Select.
 *
 * Esta funcion permite encontrar el elemento que quedaria en la posicion k
 * si el arreglo estuviera completamente ordenado segun el criterio y orden
 * indicados.
 *
 * El parametro k se interpreta desde 1. Por ejemplo, k = 1 representa el
 * mejor deportista si se usa SORT_BY_PUNTAJE con orden DESCENDING.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de deportistas almacenados en el arreglo.
 * @param k Posicion buscada, comenzando desde 1.
 * @param criteria Criterio utilizado para comparar deportistas.
 * @param order Orden de comparacion, ascendente o descendente.
 * @param pivotType Variante de pivote utilizada.
 * @return Deportista Deportista encontrado en la posicion k.
 * @return NULL si los parametros son invalidos.
 */
Deportista quick_select_deportista( Deportista *deportistas, int count, int k, SortCriteria criteria, SortOrder order, PivotType pivotType )
{
    if(deportistas == NULL || count <= 0) {
        return NULL;
    }

    if(k < 1 || k > count) {
        return NULL;
    }

    int targetIndex = k - 1;

    int selectedIndex = quick_select_recursive( deportistas, 0, count - 1, targetIndex, criteria, order, pivotType );

    if(selectedIndex < 0) {
        return NULL;
    }

    return deportistas[selectedIndex];
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
    if(deportistas == NULL || count < 2) {
        return;
    }

    quick_sort_recursive( deportistas, 0, count - 1, criteria, order, pivotType );
}


void merge_sort(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order, Deportista *temp)
{
    if(deportistas == NULL || temp == NULL || low >= high) {
        return;
    }

    int mid = low + (high - low) / 2;
    // Izquierda
    merge_sort(deportistas, low, mid, criteria, order, temp);
    // Derecha
    merge_sort(deportistas, mid + 1, high, criteria, order, temp);

    merge(deportistas, low, mid, high, criteria, order, temp);

}

void merge(Deportista *deportistas, int low, int mid, int high, SortCriteria criteria, SortOrder order, Deportista *temp)
{
    int leftIndex = low;
    int rightIndex = mid + 1;
    int tempIndex = low;

    while(leftIndex <= mid && rightIndex <= high) {
        int comparison = compare_by_criteria(deportistas[leftIndex], deportistas[rightIndex], criteria);
        int takeLeft = (order == ASCENDING && comparison <= 0) || (order == DESCENDING && comparison >= 0);

        if(takeLeft) {
            temp[tempIndex++] = deportistas[leftIndex++];
        }
        else {
            temp[tempIndex++] = deportistas[rightIndex++];
        }
    }

    while(leftIndex <= mid) {
        temp[tempIndex++] = deportistas[leftIndex++];
    }

    while(rightIndex <= high) {
        temp[tempIndex++] = deportistas[rightIndex++];
    }

    for(int i = low; i <= high; i++) {
        deportistas[i] = temp[i];
    }

}

/**
 * @brief Ordena un arreglo de deportistas usando insertion sort.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio a utilizar para la comparacion.
 * @param order Orden de ordenamiento (ascendente o descendente).
 */
void insertion_sort_deportistas(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order){

    if(deportistas == NULL || length < 2) {
        return;
    }

    for(int i = 1; i < length; i++) {
        Deportista key = deportistas[i];
        int j = i - 1;

        while(j >= 0) {
            int cmp = compare_by_criteria(deportistas[j], key, criteria);
            int shouldShift;

            if(order == ASCENDING) {
                shouldShift = (cmp > 0);
            } else {
                shouldShift = (cmp < 0);
            }

            if(!shouldShift) {
                break;
            }

            deportistas[j + 1] = deportistas[j];
            j--;
        }

        deportistas[j + 1] = key;
    }
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


