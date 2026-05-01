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

        printf(BOLD "Opcion: " NORMAL);


        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    //while(selected < INSERTION_SORT || selected > COCKTAIL_SHAKER_SORT);
    while (1); // cambiar despues

    return (SortAlgorithm)selected;
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
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    //while(selected < SEQUENTIAL_SEARCH || selected > BINARY_SEARCH);
    while(1); // cambiar despues

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

    // Cambiar al mejor segun nosotros despues

    // Poner un switch con cada algoritmo y dentro de cada caso cargar datos, ordenar, imprimir resultado y liberar memoria
    switch(algorithmOption) {
        default:
            print_error(ERROR_NOT_IMPLEMENTED, NULL);
            break;
    }
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
    int index;
    char detail[32];

    // Cambiar al mejor segun nosotros despues

    algorithmOption = ask_search_algorithm();

    // Poner un switch con cada algoritmo y dentro de cada caso cargar datos, buscar, imprimir resultado y liberar memoria
    switch(algorithmOption) {
        default:
            print_error(ERROR_NOT_IMPLEMENTED, NULL);
            break;
    }
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
