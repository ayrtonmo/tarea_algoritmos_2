/**
 * @file main.c
 * @brief Punto de entrada y parseo de argumentos del programa.
 */

#include "base.h"

/**
 * @brief Ejecuta el programa a partir de los argumentos entregados.
 *
 * @param argc Cantidad de argumentos.
 * @param argv Arreglo de argumentos.
 * @return int EXIT_SUCCESS si finaliza correctamente, EXIT_FAILURE si falla.
 */
int main(int argc, char **argv)
{
    static struct option longOptions[] = {
        {"id", required_argument, 0, 'i'},
        {0, 0, 0, 0}
    };
    int opt;

    srand((unsigned int)time(NULL));

    if(argc == 1) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }

    if(argc >= 3 && strcmp(argv[1], "-id") == 0) {
        search_by_id(atoi(argv[2]));
        return EXIT_SUCCESS;
    }

    while((opt = getopt_long(argc, argv, "hg:tr:bs", longOptions, NULL)) != -1) {
        switch(opt) {
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;
            case 'g':
            {
                int dataAmount = atoi(optarg);

                if(MIN_DATA > dataAmount || dataAmount > MAX_DATA) {
                    char detail[128];

                    snprintf(detail, sizeof(detail), "Debe estar entre %d y %d", MIN_DATA, MAX_DATA);
                    print_error(ERROR_INVALID_DATA_AMOUNT, detail);
                    return EXIT_FAILURE;
                }

                printf("\nGenerando" BOLD ORANGE " %d " RESET "datos...\n\n", atoi(optarg));
                create_deportistas_csv(dataAmount);
                break;
            }
            case 't':
            {
                run_experiment();
                break;
            }
            case 'i':
            {
                search_by_id(atoi(optarg));
                break;
            }
            case 'r':
            {
                int rankingAmount = atoi(optarg);

                if(rankingAmount <= 0) {
                    print_error(ERROR_INVALID_RANKING_AMOUNT, NULL);
                    return EXIT_FAILURE;
                }

                show_ranking(rankingAmount);
                break;
            }
            case 'b':
            {
                run_search_benchmark();
                break;
            }
            case 's':
            {
                run_sort_benchmark();
                break;
            }
            case '?':
            {
                char detail[32];
                const char *detailPtr = NULL;

                if(optopt != 0) {
                    snprintf(detail, sizeof(detail), "-%c", optopt);
                    detailPtr = detail;
                } else if(optind > 0 && optind <= argc) {
                    detailPtr = argv[optind - 1];
                }

                print_error(ERROR_UNKNOWN_OPTION, detailPtr);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}
