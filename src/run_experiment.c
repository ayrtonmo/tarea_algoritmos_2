/**
 * @file run_experiment.c
 * @brief Flujos interactivos de ordenamiento, busqueda y ranking.
 */

#include "base.h"

/**
 * @brief Carga el CSV actual en un arreglo.
 *
 * @param deportistas Direccion donde se entrega el arreglo creado.
 * @param count Direccion donde se entrega la cantidad de elementos.
 * @return int 1 si la carga fue exitosa, 0 en caso contrario.
 */
static int load_data(Deportista **deportistas, int *count)
{
    if(deportistas == NULL) {
        return 0;
    }

    if(count == NULL) {
        return 0;
    }

    *deportistas = load_deportistas_array(count);
    if(*deportistas == NULL) {
        print_error(ERROR_NO_DATA_LOADED, NULL);
        return 0;
    }

    if(*count == 0) {
        free_deportistas_array(*deportistas, *count);
        print_error(ERROR_NO_DATA_LOADED, NULL);
        return 0;
    }

    return 1;
}

/**
 * @brief Pregunta que ordenamiento usar.
 *
 * @return SortAlgorithm Opcion seleccionada.
 */
static SortAlgorithm ask_sort_algorithm(void)
{
    char option[16];
    int selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Algoritmo de ordenamiento ===\n" NORMAL);
        printf(" 1.- Insertion sort\n");
        printf(" 2.- Bubble sort\n");
        printf(" 3.- Selection sort\n");
        printf(" 4.- Cocktail shaker sort\n");
        printf(" 5.- Quick sort\n");
        printf(" 6.- Merge sort\n");
        printf(BOLD "Opcion: " NORMAL);


        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    while(selected < INSERTION_SORT || selected > MERGE_SORT);
    //while (1);

    return (SortAlgorithm)selected;
}

/**
 * @brief Pregunta que variante de pivote usar en Quick Sort.
 *
 * @return PivotType Variante de pivote seleccionada.
 */
static PivotType ask_quick_sort_pivot(void)
{
    char option[16];
    int selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Variante de pivote Quick Sort ===\n" NORMAL);
        printf("  1) Ultimo elemento\n");
        printf("  2) Primer elemento\n");
        printf("  3) Elemento aleatorio\n");
        printf("  4) Mediana de tres\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return PIVOT_LAST;
        }

        selected = atoi(option);
    }
    while(selected < PIVOT_LAST || selected > PIVOT_MEDIAN_OF_THREE);

    return (PivotType)selected;
}

/**
 * @brief Pregunta que busqueda usar.
 *
 * @return SearchAlgorithm Opcion seleccionada.
 */
static SearchAlgorithm ask_search_algorithm(void)
{
    char option[16];
    int selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Algoritmo de busqueda ===\n" NORMAL);
        printf(" 1.- Busqueda secuencial\n");
        printf(" 2.- Busqueda binaria recursiva\n");
        printf(" 3.- Busqueda exponencial\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    while(selected < SEQUENTIAL_SEARCH || selected > EXPONENTIAL_SEARCH);
    //while(1);

    return (SearchAlgorithm)selected;
}

/**
 * @brief Ejecuta una operacion de ordenamiento o ranking.
 *
 * @param criteria Campo por el cual se ordena.
 * @param rankingAmount Cantidad de elementos a mostrar.
 * @param order Sentido de ordenamiento.
 */
static void run_sort_operation(SortCriteria criteria, int rankingAmount, SortOrder order)
{
    Deportista *deportistas = NULL;
    SortAlgorithm algorithmOption = ask_sort_algorithm();
    int count = 0;

    if(!load_data(&deportistas, &count)) {
        return;
    }

    // Poner un switch con cada algoritmo y dentro de cada caso cargar datos, ordenar, imprimir resultado y liberar memoria
    switch(algorithmOption) {
        case QUICK_SORT:
        {
            PivotType pivotType = ask_quick_sort_pivot();

            quick_sort_deportistas(deportistas, count, criteria, order, pivotType);
            break;
        }

        case MERGE_SORT:
        {
            Deportista *temp = malloc(sizeof(Deportista) * count);

            if(temp == NULL) {
                print_error(ERROR_MEMORY_ALLOCATION_FAILED, NULL);
                free_deportistas_array(deportistas, count);
                return;
            }

            merge_sort(deportistas, 0, count - 1, criteria, order, temp);
            free(temp);
            break;
        }

        default:
            print_error(ERROR_NOT_IMPLEMENTED, NULL);
            break;
    }

    if(rankingAmount > count) {
        rankingAmount = count;
    }

    print_deportistas_array(deportistas, rankingAmount);
    free_deportistas_array(deportistas, count);

}

/**
 * @brief Busca un deportista por ID e imprime el resultado.
 *
 * @param targetId ID buscado.
 */
void search_by_id(int targetId)
{
    Deportista *deportistas = NULL;
    SearchAlgorithm algorithmOption;
    int count = 0;
    int index = -1;
    char detail[32];

    algorithmOption = ask_search_algorithm();
    if(!load_data(&deportistas, &count)) {
        return;
    }

    switch(algorithmOption) {
        case EXPONENTIAL_SEARCH:
            sort_deportistas_by_id_ascending(deportistas, count);
            index = exponential_search_by_id(deportistas, count, targetId);
            break;

        case BINARY_SEARCH:
            sort_deportistas_by_id_ascending(deportistas, count);
            index = binary_search_by_id_recursive(
                deportistas,
                0,
                count - 1,
                targetId
            );
            break;

        case SEQUENTIAL_SEARCH:
        default:
            print_error(ERROR_NOT_IMPLEMENTED, NULL);
            free_deportistas_array(deportistas, count);
            return;
    }

    if(index == -1) {
        snprintf(detail, sizeof(detail), "%d", targetId);
        print_error(ERROR_DEPORTISTA_NOT_FOUND, detail);
    } else {
        print_deportista(deportistas[index]);
    }

    free_deportistas_array(deportistas, count);
}
/**
 * @brief Muestra el ranking de los mejores N deportistas segun puntaje.
 *
 * @param rankingAmount Cantidad de deportistas a mostrar.
 */
void show_ranking(int rankingAmount)
{
    run_sort_operation(SORT_BY_PUNTAJE, rankingAmount, DESCENDING);
}

/**
 * @brief Muestra el k-esimo mejor deportista segun puntaje usando Quick Select.
 *
 * Carga los deportistas desde el CSV y aplica Quick Select para encontrar
 * el deportista que quedaria en la posicion k si el arreglo estuviera
 * ordenado por puntaje de forma descendente.
 *
 * @param k Posicion buscada, comenzando desde 1.
 */
void show_kth_best(int k)
{
    Deportista *deportistas = NULL;
    Deportista selected = NULL;
    int count = 0;
    char detail[64];

    if(k <= 0) {
        print_error(ERROR_INVALID_RANKING_AMOUNT, NULL);
        return;
    }

    if(!load_data(&deportistas, &count)) {
        return;
    }

    if(k > count) {
        snprintf(detail, sizeof(detail), "k debe estar entre 1 y %d", count);

        print_error(ERROR_INVALID_RANKING_AMOUNT, detail);
        free_deportistas_array(deportistas, count);
        return;
    }

    selected = quick_select_deportista( deportistas, count, k, SORT_BY_PUNTAJE, DESCENDING, PIVOT_MEDIAN_OF_THREE );

    if(selected == NULL) {
        print_error(ERROR_NOT_IMPLEMENTED, "No se pudo aplicar Quick Select");
        free_deportistas_array(deportistas, count);
        return;
    }

    printf(
        BOLD BLUE "\n=== %d-esimo mejor deportista por puntaje ===\n" NORMAL,
        k
    );

    print_deportista(selected);

    free_deportistas_array(deportistas, count);
}

/**
 * @brief Imprime la ayuda de uso del programa.
 *
 * @param programName Nombre del ejecutable.
 */
void print_help(const char *programName)
{
    printf("Uso: %s [opciones]\n", programName);
    printf("  -h                 Muestra esta ayuda\n");
    printf("  -g [cantidad]      Genera datos aleatorios\n");
    printf("  -t                 Ejecuta el flujo interactivo de ordenamiento\n");
    printf("  -id [valor]        Busca un deportista por ID\n");
    printf("  -r [cantidad]      Muestra el top N por puntaje\n");
    printf("  -k [posicion]      Muestra el k-esimo mejor deportista por puntaje\n");
    printf("  -b                 Ejecuta benchmark de busqueda\n");
    printf("  -s                 Ejecuta benchmark de ordenamiento\n");
}

/**
 * @brief Pregunta por el campo de ordenamiento.
 *
 * @return SortCriteria Opcion seleccionada.
 */
static SortCriteria ask_sort_criteria(void)
{
    char option[16];
    SortCriteria criteria;

    do {
        system("clear");

        printf(BOLD BLUE "=== Campo de ordenamiento ===\n" NORMAL);
        printf("  1) ID\n");
        printf("  2) Puntaje\n");
        printf("  3) Competencias\n");
        printf("  4) Nombre\n");
        printf("  5) Equipo\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        criteria = (SortCriteria)atoi(option);
    }
    while(criteria < SORT_BY_ID || criteria > SORT_BY_EQUIPO);

    return criteria;
}

/**
 * @brief Pregunta por el sentido del ordenamiento.
 *
 * @return SortOrder Opcion seleccionada.
 */
static SortOrder ask_sort_order(void)
{
    char option[16];
    int selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Orden ===\n" NORMAL);
        printf("  1) Descendente\n");
        printf("  2) Ascendente\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    while(selected < 1 || selected > 2);

    return (selected == 1) ? DESCENDING : ASCENDING;
}

/**
 * @brief Ejecuta el flujo interactivo de ordenamiento.
 */
void run_experiment(void)
{
    SortCriteria criteria = ask_sort_criteria();
    SortOrder order = ask_sort_order();

    run_sort_operation(criteria, MAX_DATA, order);
}
