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
 * @brief Busca un ID inexistente para medir el peor caso de busqueda.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de elementos.
 * @return int ID que no esta presente en el arreglo.
 */
static int get_missing_target_id(Deportista *deportistas, int count)
{
    int maxId;

    if(deportistas == NULL || count <= 0 || deportistas[0] == NULL) {
        return -1;
    }

    maxId = deportistas[0]->id;

    for(int i = 1; i < count; i++) {
        if(deportistas[i] != NULL && deportistas[i]->id > maxId) {
            maxId = deportistas[i]->id;
        }
    }

    return maxId + 1;
}

/**
 * @brief Prepara el arreglo para medir el peor caso de busqueda.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param algorithm Algoritmo de busqueda a ejecutar.
 * @return int ID ausente para provocar el peor caso.
 */
static int prepare_search_worst_case(Deportista *deportistas, int count, SearchAlgorithm algorithm)
{
    if(deportistas == NULL || count <= 0) {
        return -1;
    }

    if(algorithm == BINARY_SEARCH || algorithm == EXPONENTIAL_SEARCH || algorithm == INTERPOLATION_SEARCH) {


        insertion_sort_deportistas(deportistas, count, SORT_BY_ID, ASCENDING);


    }

    return get_missing_target_id(deportistas, count);
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

    fprintf(out, "n,peor_binaria_s,peor_exponencial_s,peor_interpolacion_s\n");

    printf(BOLD_BLUE "\n=== Search benchmark (peor caso) ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET, SEARCH_BENCHMARK_ROUTE, intervals, EXPERIMENT_REPEATS);

    printf("n \t | \t peor binaria(s) \t | \t peor exponencial(s) \t | \t peor interpolacion(s)\n");
    printf(ASCII_HR "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double totalWorstBin = 0.0;
        double totalWorstExp = 0.0;
        double totalWorstInterp = 0.0;

        for(int j = 0; j < EXPERIMENT_REPEATS; j++) {
            int targetId;
            clock_t start;
            clock_t end;
            Deportista *workBinary;
            Deportista *workExponential;
            Deportista *workInterpolation;

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor binaria");

            workBinary = clone_deportistas_array(baseArray, n);
            if(workBinary == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workBinary, n, BINARY_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workBinary, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            binary_search_by_id(workBinary, 0, n - 1, targetId);
            end = clock();
            totalWorstBin += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workBinary, n);

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor exponencial");

            workExponential = clone_deportistas_array(baseArray, n);
            if(workExponential == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workExponential, n, EXPONENTIAL_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workExponential, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            exponential_search_by_id(workExponential, n, targetId);
            end = clock();
            totalWorstExp += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workExponential, n);

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor interpolacion");

            workInterpolation = clone_deportistas_array(baseArray, n);
            if(workInterpolation == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workInterpolation, n, INTERPOLATION_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workInterpolation, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            interpolation_search_by_id(workInterpolation, n, targetId);
            end = clock();
            totalWorstInterp += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workInterpolation, n);
        }

        double avgWorstBin = totalWorstBin / EXPERIMENT_REPEATS;
        double avgWorstExp = totalWorstExp / EXPERIMENT_REPEATS;
        double avgWorstInterp = totalWorstInterp / EXPERIMENT_REPEATS;

        fprintf(out, "%d,%.10f,%.10f,%.10f\n", n, avgWorstBin, avgWorstExp, avgWorstInterp);
        progress_clear_line();
        printf("%d \t | \t %.10f \t | \t %.10f \t | \t %.10f\n", n, avgWorstBin, avgWorstExp, avgWorstInterp);
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

                // Quick Sort - PIVOT_LAST
                progress_update_line("sort", i + 1, intervals, n, r + 1, EXPERIMENT_REPEATS, get_case_name(benchmarkCase));
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_LAST);
                end = clock();
                caseTotals[caseIndex][0] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                // Quick Sort - PIVOT_FIRST
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_FIRST);
                end = clock();
                caseTotals[caseIndex][1] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                // Quick Sort - PIVOT_RANDOM
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_RANDOM);
                end = clock();
                caseTotals[caseIndex][2] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                // Quick Sort - PIVOT_MEDIAN_OF_THREE
                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_deportistas(work, n, SORT_BY_ID, ASCENDING, PIVOT_MEDIAN_OF_THREE);
                end = clock();
                caseTotals[caseIndex][3] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

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
                end = clock();
                caseTotals[caseIndex][4] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                free(temp);

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
                end = clock();
                caseTotals[caseIndex][5] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
                free(temp);
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
