/**
 * @file run_experiment.c
 * @brief Flujos interactivos de ordenamiento, busqueda y ranking.
 */

#include "base.h"

typedef enum {
    SEARCH_TYPE_BY_ID = 1,
} SearchType;

typedef enum {
    MAIN_MENU_SORTING = 1,
    MAIN_MENU_SEARCH = 2
} MainMenuOption;

typedef enum {
    ORDER_MENU_DESCENDING = 1,
    ORDER_MENU_ASCENDING = 2
} SortOrderMenuOption;

static const int MERGE_INSERTION_THRESHOLD = 20;
static const int MIN_ELEMENTS_FOR_MERGE_INSERTION = 100;

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
 * @brief Pregunta el tipo de busqueda interactiva a realizar.
 *
 * @return int 1=ID
 */
static SearchType ask_search_type(void)
{
    char option[16];

    system("clear");

    printf(BOLD BLUE "=== Tipo de busqueda ===\n" NORMAL);
    printf(" 1.- Buscar por ID\n\n");
    printf(BOLD "Opcion: " NORMAL);

    if(fgets(option, sizeof(option), stdin) == NULL) {
        return SEARCH_TYPE_BY_ID;
    }

    return SEARCH_TYPE_BY_ID;
}

static void run_search_interactive(void)
{
    Deportista *deportistas = NULL;
    int count = 0;
    SearchType type = ask_search_type();

    if(!load_data(&deportistas, &count)) {
        return;
    }

    if(type == SEARCH_TYPE_BY_ID) {
        char input[32];
        int targetId;

        printf(BOLD "Ingrese ID: " NORMAL);
        if(fgets(input, sizeof(input), stdin) == NULL) {
            free_deportistas_array(deportistas, count);
            return;
        }

        targetId = atoi(input);

        quick_sort_deportistas(deportistas, count, SORT_BY_ID, ASCENDING, PIVOT_LAST);

        int index = interpolation_search_by_id(deportistas, count, targetId);

        if(index == -1) {
            char detail[32];
            snprintf(detail, sizeof(detail), "%d", targetId);
            print_error(ERROR_DEPORTISTA_NOT_FOUND, detail);
        } else {
            print_deportista(deportistas[index]);
        }

        free_deportistas_array(deportistas, count);
        return;
    }
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
    int count = 0;

    if(!load_data(&deportistas, &count)) {
        return;
    }

    if(count <= MIN_ELEMENTS_FOR_MERGE_INSERTION) {
        print_error(
            ERROR_INVALID_DATA_AMOUNT,
            "Merge-Insertion requiere mas de 100 elementos"
        );
        free_deportistas_array(deportistas, count);
        return;
    }

    Deportista *temp = malloc(sizeof(Deportista) * count);

    if(temp == NULL) {
        print_error(ERROR_MEMORY_ALLOCATION_FAILED, NULL);
        free_deportistas_array(deportistas, count);
        return;
    }

    merge_insertion(
        deportistas,
        0,
        count - 1,
        criteria,
        order,
        MERGE_INSERTION_THRESHOLD,
        temp
    );

    free(temp);

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
    int count = 0;
    int index = -1;
    char detail[32];

    if(!load_data(&deportistas, &count)) {
        return;
    }

    quick_sort_deportistas(deportistas, count, SORT_BY_ID, ASCENDING, PIVOT_LAST);
    index = interpolation_search_by_id(deportistas, count, targetId);

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
    printf("  -t                 Ejecuta el flujo interactivo (ordenamiento y busqueda)\n");
    printf("  -id [valor]        Busca un deportista por ID\n");
    printf("  -r [cantidad]      Muestra el top N por puntaje\n");
    printf("  -k [posicion]      Muestra el k-esimo mejor deportista por puntaje\n");
    printf("  -b                 Ejecuta benchmark de busqueda\n");
    printf("  -s                 Ejecuta benchmark de ordenamiento\n");
    printf("  -q                 Ejecuta benchmark de Quick Select\n");
    printf("  -x                 Ejecuta benchmark de busqueda por rango de puntaje\n");
    printf("  -m                 Ejecuta benchmark de merge_insertion variando threshold\n");
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
    SortOrderMenuOption selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Orden ===\n" NORMAL);
        printf("  1) Descendente\n");
        printf("  2) Ascendente\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return ASCENDING;
        }

        selected = (SortOrderMenuOption)atoi(option);
    }
    while(selected < ORDER_MENU_DESCENDING || selected > ORDER_MENU_ASCENDING);

    return (selected == ORDER_MENU_DESCENDING) ? DESCENDING : ASCENDING;
}

/**
 * @brief Ejecuta el flujo interactivo de ordenamiento y busqueda.
 */
void run_experiment(void)
{
    char option[16];
    MainMenuOption selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Menu principal ===\n" NORMAL);
        printf(" 1.- Ordenamiento\n");
        printf(" 2.- Busqueda (ID, puntaje exacto, rango)\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return;
        }

        selected = (MainMenuOption)atoi(option);
    }
    while(selected < MAIN_MENU_SORTING || selected > MAIN_MENU_SEARCH);

    if(selected == MAIN_MENU_SORTING) {
        SortCriteria criteria = ask_sort_criteria();
        SortOrder order = ask_sort_order();

        run_sort_operation(criteria, MAX_DATA, order);
    } else {
        run_search_interactive();
    }
}
