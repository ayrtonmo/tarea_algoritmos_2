/**
 * @file benchmarks.c
 * @brief Implementacion de benchmarks de busqueda y ordenamiento.
 */

#include "base.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    BENCHMARK_CASE_BEST = 0,
    BENCHMARK_CASE_AVERAGE = 1,
    BENCHMARK_CASE_WORST = 2
} BenchmarkCase;

/**
 * @brief Limpia la linea actual en la consola.
 */
static void progress_clear_line(void)
{
    printf(CURSOR_HOME CLEAR_LINE);
    fflush(stdout);
}

/**
 * @brief Actualiza la barra de progreso de un benchmark.
 *
 * @param benchmarkName Nombre del benchmark (ej: "search" o "sort")
 * @param intervalIndex Indice del intervalo actual
 * @param intervalCount Cantidad de intervalos
 * @param n Tamano de los datos
 * @param repeatIndex Indice de la repeticion actual
 * @param repeatCount Cantidad de repeticiones
 * @param stage Etapa del proceso
 */
static void progress_update_line(const char *benchmarkName, int intervalIndex, int intervalCount, int n, int repeatIndex, int repeatCount, const char *stage)
{
    printf(
        CURSOR_HOME CLEAR_LINE DIM"%s | %d/%d | n=%d | rep %d/%d | %s" RESET,
        benchmarkName,
        intervalIndex,
        intervalCount,
        n,
        repeatIndex,
        repeatCount,
        stage
    );
    fflush(stdout);
}

/**
 * @brief Devuelve el nombre legible de un caso experimental.
 *
 * @param benchmarkCase Caso experimental.
 * @return const char* Nombre asociado.
 */
static const char *get_case_name(BenchmarkCase benchmarkCase)
{
    switch(benchmarkCase) {
        case BENCHMARK_CASE_BEST:
            return "mejor";
        case BENCHMARK_CASE_AVERAGE:
            return "promedio";
        case BENCHMARK_CASE_WORST:
            return "peor";
        default:
            return "desconocido";
    }
}

/**
 * @brief Mezcla un arreglo de deportistas usando Fisher-Yates.
 *
 * @param deportistas Arreglo a mezclar.
 * @param count Cantidad de elementos.
 */
static void shuffle_deportistas_array(Deportista *deportistas, int count)
{
    if(deportistas == NULL || count < 2) {
        return;
    }

    for(int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        swap_deportistas(&deportistas[i], &deportistas[j]);
    }
}

/**
 * @brief Prepara un arreglo para medir un caso experimental de ordenamiento.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param benchmarkCase Caso experimental requerido.
 */
static void prepare_sort_case(Deportista *deportistas, int count, BenchmarkCase benchmarkCase)
{
    if(deportistas == NULL || count < 2) {
        return;
    }

    switch(benchmarkCase) {
        case BENCHMARK_CASE_BEST:

            insertion_sort_deportistas(deportistas, count, SORT_BY_ID, ASCENDING);

            break;
        case BENCHMARK_CASE_AVERAGE:
            shuffle_deportistas_array(deportistas, count);
            break;
        case BENCHMARK_CASE_WORST:

            insertion_sort_deportistas(deportistas, count, SORT_BY_ID, DESCENDING);

            break;
        default:
            break;
    }
}

/**
 * @brief Obtiene un ID extremo para medir el peor caso de busqueda.
 * Usa el ultimo elemento del arreglo ordenado para evitar atajos de "no encontrado".
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de elementos.
 * @return int ID a buscar en el peor caso.
 */
static int get_worst_target_id(Deportista *deportistas, int count)
{
    if(deportistas == NULL || count <= 0) {
        return -1;
    }

    if(deportistas[count - 1] != NULL) {
        return deportistas[count - 1]->id;
    }

    for(int i = count - 2; i >= 0; i--) {
        if(deportistas[i] != NULL) {
            return deportistas[i]->id;
        }
    }

    return -1;
}

/**
 * @brief Obtiene el ID del penultimo elemento para el peor caso de interpolation search.
 * Cambia temporalmente el ID del ultimo elemento para romper la distribucion uniforme.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de elementos.
 * @return int ID a buscar en el peor caso de interpolation search.
 */
static int get_worst_target_id_interpolation(Deportista *deportistas, int count)
{
    if(deportistas == NULL || count <= 2) {
        return -1;
    }

    int penultimo_id = -1;

    // Obtener el ID del penultimo elemento
    if(deportistas[count - 2] != NULL) {
        penultimo_id = deportistas[count - 2]->id;
    }

    if(penultimo_id < 0) {
        return -1;
    }

    // Cambiar el ID del ultimo elemento a 1000 * penultimo_id
    // para que no haya distribucion uniforme
    if(deportistas[count - 1] != NULL) {
        deportistas[count - 1]->id = 1000 * penultimo_id;
    }

    return penultimo_id;
}

/**
 * @brief Prepara el arreglo para medir un caso de busqueda.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param algorithm Algoritmo de busqueda a ejecutar.
 * @param benchmarkCase Caso a preparar.
 * @return int ID a buscar.
 */
static int prepare_search_case(Deportista *deportistas, int count, SearchAlgorithm algorithm, BenchmarkCase benchmarkCase)
{
    if(deportistas == NULL || count <= 0) {
        return -1;
    }

    if(algorithm == BINARY_SEARCH || algorithm == EXPONENTIAL_SEARCH || algorithm == INTERPOLATION_SEARCH) {
        insertion_sort_deportistas(deportistas, count, SORT_BY_ID, ASCENDING);
    }

    if(benchmarkCase == BENCHMARK_CASE_WORST) {
        // Para interpolation search, usar el penultimo ID y cambiar el ultimo para romper la distribucion uniforme
        if(algorithm == INTERPOLATION_SEARCH) {
            return get_worst_target_id_interpolation(deportistas, count);
        }
        return get_worst_target_id(deportistas, count);
    } else if(benchmarkCase == BENCHMARK_CASE_AVERAGE) {
        if(count > 0) {
            return deportistas[count / 2]->id;
        }
    }

    return -1;
}

/**
 * @brief Duplica una cadena para los benchmarks.
 *
 * @param srcString String a duplicar.
 * @return char* Cadena duplicada o NULL si hubo un error.
 */
static char *duplicate_string_bench(const char *srcString)
{
    size_t len;
    char *copy;

    if(srcString == NULL) {
        return NULL;
    }

    len = strlen(srcString);
    copy = malloc(len + 1);
    if(copy == NULL) {
        return NULL;
    }

    memcpy(copy, srcString, len + 1);
    return copy;
}

/**
 * @brief Maneja un error de memoria durante la ejecucion de benchmarks.
 *
 * @param baseArray Arreglo base cargado desde el CSV.
 * @param count Cantidad de deportistas del arreglo base.
 * @param out Archivo de salida del benchmark.
 */
static void handle_benchmark_memory_error(Deportista *baseArray, int count, FILE *out)
{
    progress_clear_line();
    printf(SHOW_CURSOR);

    if(out != NULL) {
        fclose(out);
    }

    free_deportistas_array(baseArray, count);
    print_error(ERROR_MEMORY_ALLOCATION_FAILED, "durante el benchmark");
    exit(EXIT_FAILURE);
}

/**
 * @brief Clona un arreglo de deportistas para ejecutar benchmarks.
 *
 * @param srcArray Arreglo a clonar.
 * @param count Cantidad de elementos a clonar
 * @return Deportista* Arreglo de deportistas clonados o NULL si hubo un error.
 */
static Deportista *clone_deportistas_array(Deportista *srcArray, int count)
{
    Deportista *deportistas;

    if(srcArray == NULL || count < 0) {
        return NULL;
    }

    deportistas = malloc(sizeof(Deportista) * count);
    if(deportistas == NULL) {
        return NULL;
    }

    for(int i = 0; i < count; i++) {
        char *nombre;
        char *equipo;

        if(srcArray[i] == NULL) {
            deportistas[i] = NULL;
            continue;
        }

        nombre = duplicate_string_bench(srcArray[i]->nombre);
        equipo = duplicate_string_bench(srcArray[i]->equipo);

        if(nombre == NULL || equipo == NULL) {
            free(nombre);
            free(equipo);

            for(int j = 0; j < i; j++) {
                if(deportistas[j] != NULL) {
                    delete_deportista(deportistas[j]);
                }
            }

            free(deportistas);
            return NULL;
        }

        deportistas[i] = create_deportista(
            srcArray[i]->id,
            nombre,
            equipo,
            srcArray[i]->puntaje,
            srcArray[i]->competencias
        );

        if(deportistas[i] == NULL) {
            free(nombre);
            free(equipo);

            for(int j = 0; j < i; j++) {
                if(deportistas[j] != NULL) {
                    delete_deportista(deportistas[j]);
                }
            }

            free(deportistas);
            return NULL;
        }
    }

    return deportistas;
}

/**
 * @brief Ejecuta el benchmark de busqueda y guarda sus resultados en CSV.
 */
void run_search_benchmark()
{
    int count = 0;
    Deportista *baseArray = load_deportistas_array(&count);

    if(baseArray == NULL || count <= 0) {
        if(baseArray != NULL) {
            free_deportistas_array(baseArray, count);
        }

        print_error(ERROR_BENCHMARK_DATA_LOAD_FAILED, NULL);
        return;
    }

    int intervals = (count < INTERVAL_SIZE) ? count : INTERVAL_SIZE;
    int stepSize = count / intervals;

    if(stepSize <= 0) {
        stepSize = 1;
    }

    FILE *out = fopen(SEARCH_BENCHMARK_ROUTE, "w");

    if(out == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, SEARCH_BENCHMARK_ROUTE);
        free_deportistas_array(baseArray, count);
        return;
    }

    fprintf(out, "n,promedio_binaria_s,promedio_exponencial_s,promedio_interpolacion_s,peor_binaria_s,peor_exponencial_s,peor_interpolacion_s\n");

    printf(BOLD_BLUE "\n=== Search benchmark (promedio y peor caso) ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET, SEARCH_BENCHMARK_ROUTE, intervals, EXPERIMENT_REPEATS);

    printf("n \t | promedio binaria(s) | promedio exponencial(s) | promedio interpolacion(s) | peor binaria(s) | peor exponencial(s) | peor interpolacion(s)\n");
    printf(ASCII_HR "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double caseTotals[2][3] = {{0.0}};

        for(int caseIndex = BENCHMARK_CASE_AVERAGE; caseIndex <= BENCHMARK_CASE_WORST; caseIndex++) {
            BenchmarkCase benchmarkCase = (BenchmarkCase)caseIndex;

            for(int j = 0; j < EXPERIMENT_REPEATS; j++) {
                int targetId;
                clock_t start;
                clock_t end;
                Deportista *workBinary;
                Deportista *workExponential;
                Deportista *workInterpolation;
                volatile int result;
                int innerLoops = 100000;

                progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, get_case_name(benchmarkCase));

                // Binary Search
                workBinary = clone_deportistas_array(baseArray, n);
                if(workBinary == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                targetId = prepare_search_case(workBinary, n, BINARY_SEARCH, benchmarkCase);
                if(targetId < 0) {
                    free_deportistas_array(workBinary, n);
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                start = clock();
                for(int k = 0; k < innerLoops; k++) {
                    result = binary_search_by_id(workBinary, 0, n - 1, targetId);
                }
                end = clock();
                caseTotals[caseIndex - BENCHMARK_CASE_AVERAGE][0] += (double)(end - start) / CLOCKS_PER_SEC / innerLoops;
                free_deportistas_array(workBinary, n);
                (void)result;

                // Exponential Search
                workExponential = clone_deportistas_array(baseArray, n);
                if(workExponential == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                targetId = prepare_search_case(workExponential, n, EXPONENTIAL_SEARCH, benchmarkCase);
                if(targetId < 0) {
                    free_deportistas_array(workExponential, n);
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                start = clock();
                for(int k = 0; k < innerLoops; k++) {
                    result = exponential_search_by_id(workExponential, n, targetId);
                }
                end = clock();
                caseTotals[caseIndex - BENCHMARK_CASE_AVERAGE][1] += (double)(end - start) / CLOCKS_PER_SEC / innerLoops;
                free_deportistas_array(workExponential, n);
                (void)result;

                // Interpolation Search
                workInterpolation = clone_deportistas_array(baseArray, n);
                if(workInterpolation == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                targetId = prepare_search_case(workInterpolation, n, INTERPOLATION_SEARCH, benchmarkCase);
                if(targetId < 0) {
                    free_deportistas_array(workInterpolation, n);
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                start = clock();
                for(int k = 0; k < innerLoops; k++) {
                    result = interpolation_search_by_id(workInterpolation, n, targetId);
                }
                end = clock();
                caseTotals[caseIndex - BENCHMARK_CASE_AVERAGE][2] += (double)(end - start) / CLOCKS_PER_SEC / innerLoops;
                free_deportistas_array(workInterpolation, n);
                (void)result;
            }

            for(int algorithmIndex = 0; algorithmIndex < 3; algorithmIndex++) {
                caseTotals[caseIndex - BENCHMARK_CASE_AVERAGE][algorithmIndex] /= EXPERIMENT_REPEATS;
            }
        }

        fprintf(
            out,
            "%d,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n",
            n,
            caseTotals[0][0],
            caseTotals[0][1],
            caseTotals[0][2],
            caseTotals[1][0],
            caseTotals[1][1],
            caseTotals[1][2]
        );

        progress_clear_line();
        printf("%d \t | %.10f | %.10f | %.10f | %.10f | %.10f | %.10f\n", 
            n,
            caseTotals[0][0],
            caseTotals[0][1],
            caseTotals[0][2],
            caseTotals[1][0],
            caseTotals[1][1],
            caseTotals[1][2]
        );
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);
    printf(BG_GREEN "\nBenchmark guardado correctamente en %s\n" RESET, SEARCH_BENCHMARK_ROUTE);
}

/**
 * @brief Ejecuta el benchmark de ordenamiento y guarda sus resultados en CSV.
 */
void run_sort_benchmark()
{
    int count = 0;
    Deportista *baseArray = load_deportistas_array(&count);

    if(baseArray == NULL || count <= 0) {
        if(baseArray != NULL) {
            free_deportistas_array(baseArray, count);
        }

        print_error(ERROR_BENCHMARK_DATA_LOAD_FAILED, NULL);
        return;
    }

    int intervals = (count < INTERVAL_SIZE) ? count : INTERVAL_SIZE;
    int stepSize = count / intervals;

    if(stepSize <= 0) {
        stepSize = 1;
    }

    FILE *out = fopen(SORT_BENCHMARK_ROUTE, "w");

    if(out == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, SORT_BENCHMARK_ROUTE);
        free_deportistas_array(baseArray, count);
        return;
    }

    // fprintf(
    //     out,
    //     "n,"
    //     "mejor_insertion_s,mejor_quick_s,mejor_merge_s,mejor_merge_insertion_s,"
    //     "promedio_insertion_s,promedio_quick_s,promedio_merge_s,promedio_merge_insertion_s,"
    //     "peor_insertion_s,peor_quick_s,peor_merge_s,peor_merge_insertion_s\n"
    // );

    fprintf(
        out,
        "n,"
        "mejor_quick_last_s,mejor_quick_first_s,mejor_quick_random_s,mejor_quick_median_s,mejor_merge_s,mejor_merge_insertion_s,"
        "promedio_quick_last_s,promedio_quick_first_s,promedio_quick_random_s,promedio_quick_median_s,promedio_merge_s,promedio_merge_insertion_s,"
        "peor_quick_last_s,peor_quick_first_s,peor_quick_random_s,peor_quick_median_s,peor_merge_s,peor_merge_insertion_s\n"
    );

    printf(BOLD_BLUE "\n=== Sort benchmark ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET, SORT_BENCHMARK_ROUTE, intervals, EXPERIMENT_REPEATS);

    printf("n \t | caso \t | quick_last(s) | quick_first(s) | quick_random(s) | quick_median(s) | merge(s) | merge_insertion(s)\n");
    printf(ASCII_HR_WIDE "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double caseTotals[3][6] = {{0.0}};

        for(int caseIndex = BENCHMARK_CASE_BEST; caseIndex <= BENCHMARK_CASE_WORST; caseIndex++) {
            BenchmarkCase benchmarkCase = (BenchmarkCase)caseIndex;

            for(int r = 0; r < EXPERIMENT_REPEATS; r++) {
                Deportista *work;
                Deportista *temp;
                clock_t start;
                clock_t end;
                volatile int sortCheck;

                // Quick Sort - PIVOT_LAST
                progress_update_line("sort", i + 1, intervals, n, r + 1, EXPERIMENT_REPEATS, get_case_name(benchmarkCase));
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_LAST);
                sortCheck = (work[0] != NULL ? 1 : 0);
                end = clock();
                caseTotals[caseIndex][0] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                (void)sortCheck;

                // Quick Sort - PIVOT_FIRST
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_FIRST);
                sortCheck = (work[0] != NULL ? 1 : 0);
                end = clock();
                caseTotals[caseIndex][1] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                (void)sortCheck;

                // Quick Sort - PIVOT_RANDOM
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_RANDOM);
                sortCheck = (work[0] != NULL ? 1 : 0);
                end = clock();
                caseTotals[caseIndex][2] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                (void)sortCheck;

                // Quick Sort - PIVOT_MEDIAN_OF_THREE
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_MEDIAN_OF_THREE);
                sortCheck = (work[0] != NULL ? 1 : 0);
                end = clock();
                caseTotals[caseIndex][3] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                (void)sortCheck;

                // Merge Sort
                work = clone_deportistas_array(baseArray, n);
                temp = malloc(sizeof(Deportista) * n);
                if(work == NULL || temp == NULL) {
                    free(temp);
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                merge_sort(work, 0, n - 1, SORT_BY_ID, ASCENDING, temp);
                sortCheck = (work[0] != NULL ? 1 : 0);
                end = clock();
                caseTotals[caseIndex][4] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                free(temp);
                (void)sortCheck;

                // Merge Insertion (con threshold = 10)
                work = clone_deportistas_array(baseArray, n);
                temp = malloc(sizeof(Deportista) * n);
                if(work == NULL || temp == NULL) {
                    free(temp);
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                merge_insertion(work, 0, n - 1, SORT_BY_ID, ASCENDING, 10, temp);
                sortCheck = (work[0] != NULL ? 1 : 0);
                end = clock();
                caseTotals[caseIndex][5] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                free(temp);
                (void)sortCheck;
            }

            for(int algorithmIndex = 0; algorithmIndex < 6; algorithmIndex++) {
                caseTotals[caseIndex][algorithmIndex] /= EXPERIMENT_REPEATS;
            }
        }

        fprintf(
            out,
            "%d,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n",
            n,
            caseTotals[BENCHMARK_CASE_BEST][0],
            caseTotals[BENCHMARK_CASE_BEST][1],
            caseTotals[BENCHMARK_CASE_BEST][2],
            caseTotals[BENCHMARK_CASE_BEST][3],
            caseTotals[BENCHMARK_CASE_BEST][4],
            caseTotals[BENCHMARK_CASE_BEST][5],
            caseTotals[BENCHMARK_CASE_AVERAGE][0],
            caseTotals[BENCHMARK_CASE_AVERAGE][1],
            caseTotals[BENCHMARK_CASE_AVERAGE][2],
            caseTotals[BENCHMARK_CASE_AVERAGE][3],
            caseTotals[BENCHMARK_CASE_AVERAGE][4],
            caseTotals[BENCHMARK_CASE_AVERAGE][5],
            caseTotals[BENCHMARK_CASE_WORST][0],
            caseTotals[BENCHMARK_CASE_WORST][1],
            caseTotals[BENCHMARK_CASE_WORST][2],
            caseTotals[BENCHMARK_CASE_WORST][3],
            caseTotals[BENCHMARK_CASE_WORST][4],
            caseTotals[BENCHMARK_CASE_WORST][5]
        );

        progress_clear_line();
        printf(
            "%d \t | %s \t | %.10f | %.10f | %.10f | %.10f | %.10f | %.10f\n",
            n,
            get_case_name(BENCHMARK_CASE_BEST),
            caseTotals[BENCHMARK_CASE_BEST][0],
            caseTotals[BENCHMARK_CASE_BEST][1],
            caseTotals[BENCHMARK_CASE_BEST][2],
            caseTotals[BENCHMARK_CASE_BEST][3],
            caseTotals[BENCHMARK_CASE_BEST][4],
            caseTotals[BENCHMARK_CASE_BEST][5]
        );
        printf(
            "%d \t | %s \t | %.10f | %.10f | %.10f | %.10f | %.10f | %.10f\n",
            n,
            get_case_name(BENCHMARK_CASE_AVERAGE),
            caseTotals[BENCHMARK_CASE_AVERAGE][0],
            caseTotals[BENCHMARK_CASE_AVERAGE][1],
            caseTotals[BENCHMARK_CASE_AVERAGE][2],
            caseTotals[BENCHMARK_CASE_AVERAGE][3],
            caseTotals[BENCHMARK_CASE_AVERAGE][4],
            caseTotals[BENCHMARK_CASE_AVERAGE][5]
        );
        printf(
            "%d \t | %s \t | %.10f | %.10f | %.10f | %.10f | %.10f | %.10f\n",
            n,
            get_case_name(BENCHMARK_CASE_WORST),
            caseTotals[BENCHMARK_CASE_WORST][0],
            caseTotals[BENCHMARK_CASE_WORST][1],
            caseTotals[BENCHMARK_CASE_WORST][2],
            caseTotals[BENCHMARK_CASE_WORST][3],
            caseTotals[BENCHMARK_CASE_WORST][4],
            caseTotals[BENCHMARK_CASE_WORST][5]
        );
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);
    printf(BG_GREEN "\nBenchmark de ordenamiento guardado con exito en %s\n" RESET, SORT_BENCHMARK_ROUTE);
}

/**
 * @brief Ejecuta benchmark de merge_insertion variando threshold y guarda CSV.
 */
void run_merge_threshold_benchmark()
{
    int count = 0;
    Deportista *baseArray = load_deportistas_array(&count);
    const int thresholds[] = {10, 15, 20, 25, 30, 35, 40, 50};
    int thresholdCount = (int)(sizeof(thresholds) / sizeof(thresholds[0]));
    int intervals;
    int stepSize;

    if(baseArray == NULL || count <= 0) {
        if(baseArray != NULL) {
            free_deportistas_array(baseArray, count);
        }

        print_error(ERROR_BENCHMARK_DATA_LOAD_FAILED, NULL);
        return;
    }

    if(count < MIN_MERGE_THRESHOLD_BENCHMARK_SIZE) {
        char detail[160];

        snprintf(
            detail,
            sizeof(detail),
            "Se requiere un arreglo de al menos %d elementos (actual: %d)",
            MIN_MERGE_THRESHOLD_BENCHMARK_SIZE,
            count
        );

        print_error(ERROR_INVALID_DATA_AMOUNT, detail);
        free_deportistas_array(baseArray, count);
        return;
    }

    intervals = (count < INTERVAL_SIZE) ? count : INTERVAL_SIZE;
    stepSize = count / intervals;

    if(stepSize <= 0) {
        stepSize = 1;
    }

    FILE *out = fopen(MERGE_THRESHOLD_BENCHMARK_ROUTE, "w");

    if(out == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, MERGE_THRESHOLD_BENCHMARK_ROUTE);
        free_deportistas_array(baseArray, count);
        return;
    }

    fprintf(out, "n,merge_classic_s");
    for(int t = 0; t < thresholdCount; t++) {
        fprintf(out, ",threshold_%d_s", thresholds[t]);
    }
    fprintf(out, "\n");

    printf(BOLD_BLUE "\n=== Merge+Insertion threshold benchmark ===\n" RESET);
    printf(
        DIM "Archivo: %s | intervalos: %d | thresholds: %d | repeticiones: %d\n\n" RESET,
        MERGE_THRESHOLD_BENCHMARK_ROUTE,
        intervals,
        thresholdCount,
        EXPERIMENT_REPEATS
    );

    printf("n\t| tiempos promedio por threshold (s)\n");
    printf(ASCII_HR "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double thresholdTotals[16] = {0.0};
        double classicTotal = 0.0;

        /* Medir merge clásico para este n */
        for(int r = 0; r < EXPERIMENT_REPEATS; r++) {
            Deportista *workC;
            Deportista *tempC;
            clock_t startC;
            clock_t endC;

            progress_update_line("merge-threshold", i + 1, intervals, n, r + 1, EXPERIMENT_REPEATS, "merge-classic");

            workC = clone_deportistas_array(baseArray, n);
            tempC = malloc(sizeof(Deportista) * n);
            if(workC == NULL || tempC == NULL) {
                free(tempC);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            prepare_sort_case(workC, n, BENCHMARK_CASE_AVERAGE);

            startC = clock();
            merge_sort(workC, 0, n - 1, SORT_BY_ID, ASCENDING, tempC);
            endC = clock();

            classicTotal += (double)(endC - startC) / CLOCKS_PER_SEC;

            free_deportistas_array(workC, n);
            free(tempC);
        }

        classicTotal /= EXPERIMENT_REPEATS;

        /* Medir merge_insertion para cada threshold */
        for(int t = 0; t < thresholdCount; t++) {
            int threshold = thresholds[t];

            for(int r = 0; r < EXPERIMENT_REPEATS; r++) {
                Deportista *work;
                Deportista *temp;
                clock_t start;
                clock_t end;
                volatile int sortCheck;
                char stage[32];

                snprintf(stage, sizeof(stage), "th=%d", threshold);
                progress_update_line("merge-threshold", i + 1, intervals, n, r + 1, EXPERIMENT_REPEATS, stage);

                work = clone_deportistas_array(baseArray, n);
                temp = malloc(sizeof(Deportista) * n);
                if(work == NULL || temp == NULL) {
                    free(temp);
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, BENCHMARK_CASE_AVERAGE);

                start = clock();
                merge_insertion(work, 0, n - 1, SORT_BY_ID, ASCENDING, threshold, temp);
                sortCheck = (work[0] != NULL ? 1 : 0);
                end = clock();

                thresholdTotals[t] += (double)(end - start) / CLOCKS_PER_SEC;

                (void)sortCheck;
                free_deportistas_array(work, n);
                free(temp);
            }

            thresholdTotals[t] /= EXPERIMENT_REPEATS;
        }

        /* Escribir fila: n, merge_classic, thresholds... */
        fprintf(out, "%d,%.10f", n, classicTotal);
        for(int t = 0; t < thresholdCount; t++) {
            fprintf(out, ",%.10f", thresholdTotals[t]);
        }
        fprintf(out, "\n");

        progress_clear_line();
        printf("%d | merge_classic=%.10f", n, classicTotal);
        for(int t = 0; t < thresholdCount; t++) {
            printf(" th%d=%.10f", thresholds[t], thresholdTotals[t]);
        }
        printf("\n");
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);

    printf(
        BG_GREEN "\nBenchmark de thresholds de merge_insertion guardado con exito en %s\n" RESET,
        MERGE_THRESHOLD_BENCHMARK_ROUTE
    );
}

/**
 * @brief Ejecuta el benchmark de Quick Select y guarda sus resultados en CSV.
 */
void run_select_benchmark()
{
    int count = 0;
    Deportista *baseArray = load_deportistas_array(&count);

    if(baseArray == NULL || count <= 0) {
        if(baseArray != NULL) {
            free_deportistas_array(baseArray, count);
        }

        print_error(ERROR_BENCHMARK_DATA_LOAD_FAILED, NULL);
        return;
    }

    int intervals = (count < INTERVAL_SIZE) ? count : INTERVAL_SIZE;
    int stepSize = count / intervals;

    if(stepSize <= 0) {
        stepSize = 1;
    }

    FILE *out = fopen(SELECT_BENCHMARK_ROUTE, "w");

    if(out == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, SELECT_BENCHMARK_ROUTE);
        free_deportistas_array(baseArray, count);
        return;
    }

    fprintf(
        out,
        "n,"
        "mejor_last_s,mejor_first_s,mejor_random_s,mejor_median_s,"
        "promedio_last_s,promedio_first_s,promedio_random_s,promedio_median_s,"
        "peor_last_s,peor_first_s,peor_random_s,peor_median_s\n"
    );

    printf(BOLD_BLUE "\n=== Quick Select benchmark ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET,
        SELECT_BENCHMARK_ROUTE,
        intervals,
        EXPERIMENT_REPEATS
    );

    printf("n\t| caso\t| last(s) | first(s) | random(s) | median(s)\n");
    printf(ASCII_HR_WIDE "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double caseTotals[3][4] = {{0.0}};

        for(int caseIndex = BENCHMARK_CASE_BEST; caseIndex <= BENCHMARK_CASE_WORST; caseIndex++) {
            BenchmarkCase benchmarkCase = (BenchmarkCase)caseIndex;

            for(int r = 0; r < EXPERIMENT_REPEATS; r++) {
                PivotType pivots[4] = {
                    PIVOT_LAST,
                    PIVOT_FIRST,
                    PIVOT_RANDOM,
                    PIVOT_MEDIAN_OF_THREE
                };

                for(int p = 0; p < 4; p++) {
                    Deportista *work;
                    Deportista selected;
                    clock_t start;
                    clock_t end;
                    volatile int selectedId;
                    int k;

                    progress_update_line(
                        "quickselect",
                        i + 1,
                        intervals,
                        n,
                        r + 1,
                        EXPERIMENT_REPEATS,
                        get_case_name(benchmarkCase)
                    );

                    work = clone_deportistas_array(baseArray, n);
                    if(work == NULL) {
                        handle_benchmark_memory_error(baseArray, count, out);
                    }

                    if(benchmarkCase == BENCHMARK_CASE_BEST) {
                        insertion_sort_deportistas(work, n, SORT_BY_PUNTAJE, DESCENDING);
                        k = 1;
                    }
                    else if(benchmarkCase == BENCHMARK_CASE_AVERAGE) {
                        shuffle_deportistas_array(work, n);
                        k = (n / 2) + 1;
                    }
                    else {
                        insertion_sort_deportistas(work, n, SORT_BY_PUNTAJE, ASCENDING);
                        k = 1;
                    }

                    start = clock();

                    selected = quick_select_deportista(
                        work,
                        n,
                        k,
                        SORT_BY_PUNTAJE,
                        DESCENDING,
                        pivots[p]
                    );

                    if(selected != NULL) {
                        selectedId = selected->id;
                    } else {
                        selectedId = -1;
                    }

                    end = clock();

                    caseTotals[caseIndex][p] += (double)(end - start) / CLOCKS_PER_SEC;

                    (void)selectedId;
                    free_deportistas_array(work, n);
                }
            }

            for(int p = 0; p < 4; p++) {
                caseTotals[caseIndex][p] /= EXPERIMENT_REPEATS;
            }
        }

        fprintf(
            out,
            "%d,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n",
            n,
            caseTotals[BENCHMARK_CASE_BEST][0],
            caseTotals[BENCHMARK_CASE_BEST][1],
            caseTotals[BENCHMARK_CASE_BEST][2],
            caseTotals[BENCHMARK_CASE_BEST][3],
            caseTotals[BENCHMARK_CASE_AVERAGE][0],
            caseTotals[BENCHMARK_CASE_AVERAGE][1],
            caseTotals[BENCHMARK_CASE_AVERAGE][2],
            caseTotals[BENCHMARK_CASE_AVERAGE][3],
            caseTotals[BENCHMARK_CASE_WORST][0],
            caseTotals[BENCHMARK_CASE_WORST][1],
            caseTotals[BENCHMARK_CASE_WORST][2],
            caseTotals[BENCHMARK_CASE_WORST][3]
        );

        progress_clear_line();

        printf(
            "%d\t| mejor\t\t| %.10f | %.10f | %.10f | %.10f\n",
            n,
            caseTotals[BENCHMARK_CASE_BEST][0],
            caseTotals[BENCHMARK_CASE_BEST][1],
            caseTotals[BENCHMARK_CASE_BEST][2],
            caseTotals[BENCHMARK_CASE_BEST][3]
        );

        printf(
            "%d\t| promedio\t| %.10f | %.10f | %.10f | %.10f\n",
            n,
            caseTotals[BENCHMARK_CASE_AVERAGE][0],
            caseTotals[BENCHMARK_CASE_AVERAGE][1],
            caseTotals[BENCHMARK_CASE_AVERAGE][2],
            caseTotals[BENCHMARK_CASE_AVERAGE][3]
        );

        printf(
            "%d\t| peor\t\t| %.10f | %.10f | %.10f | %.10f\n",
            n,
            caseTotals[BENCHMARK_CASE_WORST][0],
            caseTotals[BENCHMARK_CASE_WORST][1],
            caseTotals[BENCHMARK_CASE_WORST][2],
            caseTotals[BENCHMARK_CASE_WORST][3]
        );
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);

    printf(BG_GREEN "\nBenchmark de Quick Select guardado con exito en %s\n" RESET,
        SELECT_BENCHMARK_ROUTE
    );
}

/**
 * @brief Ejecuta el benchmark de busqueda binaria por rango de puntaje.
 *
 * El arreglo se ordena previamente por puntaje ascendente antes de iniciar
 * la medicion, de modo que el tiempo medido corresponda solamente a la
 * busqueda binaria modificada para rangos.
 */
void run_range_benchmark()
{
    int count = 0;
    Deportista *baseArray = load_deportistas_array(&count);

    if(baseArray == NULL || count <= 0) {
        if(baseArray != NULL) {
            free_deportistas_array(baseArray, count);
        }

        print_error(ERROR_BENCHMARK_DATA_LOAD_FAILED, NULL);
        return;
    }

    int intervals = (count < INTERVAL_SIZE) ? count : INTERVAL_SIZE;
    int stepSize = count / intervals;

    if(stepSize <= 0) {
        stepSize = 1;
    }

    FILE *out = fopen(RANGE_BENCHMARK_ROUTE, "w");

    if(out == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, RANGE_BENCHMARK_ROUTE);
        free_deportistas_array(baseArray, count);
        return;
    }

    fprintf(out, "n,promedio_rango_s,peor_rango_s\n");

    printf(BOLD_BLUE "\n=== Range Search benchmark ===\n" RESET);
    printf(
        DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET,
        RANGE_BENCHMARK_ROUTE,
        intervals,
        EXPERIMENT_REPEATS
    );

    printf("n\t| promedio rango(s) | peor rango(s)\n");
    printf(ASCII_HR "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double averageTotal = 0.0;
        double worstTotal = 0.0;

        for(int r = 0; r < EXPERIMENT_REPEATS; r++) {
            Deportista *workAverage;
            Deportista *workWorst;
            clock_t start;
            clock_t end;
            volatile int found;
            int outLeft;
            int outRight;
            int innerLoops = 100000;

            progress_update_line(
                "range",
                i + 1,
                intervals,
                n,
                r + 1,
                EXPERIMENT_REPEATS,
                "promedio"
            );

            workAverage = clone_deportistas_array(baseArray, n);
            if(workAverage == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            insertion_sort_deportistas(workAverage, n, SORT_BY_PUNTAJE, ASCENDING);

            start = clock();

            for(int k = 0; k < innerLoops; k++) {
                found = binary_search_by_range_sorted(workAverage, n, 40.0f, 80.0f, &outLeft, &outRight);
            }

            end = clock();

            averageTotal += (double)(end - start) / CLOCKS_PER_SEC / innerLoops;

            (void)found;
            free_deportistas_array(workAverage, n);

            progress_update_line("range", i + 1, intervals, n, r + 1, EXPERIMENT_REPEATS, "peor");

            workWorst = clone_deportistas_array(baseArray, n);
            if(workWorst == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            insertion_sort_deportistas(workWorst, n, SORT_BY_PUNTAJE, ASCENDING);

            start = clock();

            for(int k = 0; k < innerLoops; k++) {
                found = binary_search_by_range_sorted(workWorst, n, MAX_SCORE + 1.0f, MAX_SCORE + 2.0f, &outLeft, &outRight);
            }

            end = clock();

            worstTotal += (double)(end - start) / CLOCKS_PER_SEC / innerLoops;

            (void)found;
            free_deportistas_array(workWorst, n);
        }

        averageTotal /= EXPERIMENT_REPEATS;
        worstTotal /= EXPERIMENT_REPEATS;

        fprintf( out, "%d,%.10f,%.10f\n", n, averageTotal, worstTotal );

        progress_clear_line();

        printf( "%d\t| %.10f | %.10f\n", n, averageTotal, worstTotal );
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);

    printf(
        BG_GREEN "\nBenchmark de busqueda por rango guardado con exito en %s\n" RESET,
        RANGE_BENCHMARK_ROUTE
    );
}