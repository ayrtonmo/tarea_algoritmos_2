/**
 * @file print_format.h
 * @brief Utilidades de formato y salida por consola.
 */

#ifndef PRINT_FORMAT_H
#define PRINT_FORMAT_H

#include "deportista.h"

/**
 * @name Secuencias ANSI (estilos)
 * @brief Macros de formato para terminales compatibles con ANSI.
 * @{
 */
#define BOLD "\033[1m"       /**< Texto en negrita. */
#define DIM "\033[2m"        /**< Texto atenuado. */
#define UNDERLINE "\033[4m"  /**< Texto subrayado. */
#define NORMAL "\033[0m"     /**< Restablece estilo (alias de RESET). */
#define RESET "\033[0m"      /**< Restablece estilo y colores. */
/** @} */

/**
 * @name Reset parcial
 * @brief Vuelve a colores por defecto sin tocar otras propiedades.
 * @{
 */
#define FG_DEFAULT "\033[39m" /**< Color de primer plano por defecto. */
#define BG_DEFAULT "\033[49m" /**< Color de fondo por defecto. */
#define NO_BG "\033[49m"      /**< Alias de BG_DEFAULT. */
/** @} */

/**
 * @name Control de terminal
 * @brief Secuencias utilitarias para control del cursor/linea.
 * @{
 */
#define CLEAR_LINE "\033[2K"  /**< Borra la linea actual. */
#define CURSOR_HOME "\r"      /**< Retorna al inicio de la linea. */
#define HIDE_CURSOR "\033[?25l" /**< Oculta el cursor. */
#define SHOW_CURSOR "\033[?25h" /**< Muestra el cursor. */
/** @} */

/**
 * @name Utilidades ASCII
 * @brief Separadores y etiquetas para salidas consistentes.
 * @{
 */
#define ASCII_HR "-------------------------------------------------------------------" /**< Separador horizontal. */
#define ASCII_HR_WIDE "---------------------------------------------------------------------------------------------------------------" /**< Separador horizontal ancho. */
#define ASCII_OK "[OK]"   /**< Etiqueta de exito. */
#define ASCII_FAIL "[FAIL]" /**< Etiqueta de fallo. */
/** @} */

/**
 * @name Colores ANSI (primer plano)
 * @{
 */
#define RED "\033[31m"     /**< Rojo. */
#define GREEN "\033[32m"   /**< Verde. */
#define YELLOW "\033[33m"  /**< Amarillo. */
#define BLUE "\033[34m"    /**< Azul. */
#define MAGENTA "\033[35m" /**< Magenta. */

#define CYAN "\033[36m"    /**< Cian. */
#define WHITE "\033[37m"   /**< Blanco. */
/** @} */

/**
 * @name Colores ANSI (brillantes)
 * @{
 */
#define BRIGHT_BLACK "\033[90m"   /**< Negro brillante. */
#define BRIGHT_RED "\033[91m"     /**< Rojo brillante. */
#define BRIGHT_GREEN "\033[92m"   /**< Verde brillante. */
#define BRIGHT_YELLOW "\033[93m"  /**< Amarillo brillante. */
#define BRIGHT_BLUE "\033[94m"    /**< Azul brillante. */
#define BRIGHT_MAGENTA "\033[95m" /**< Magenta brillante. */
#define BRIGHT_CYAN "\033[96m"    /**< Cian brillante. */
#define BRIGHT_WHITE "\033[97m"   /**< Blanco brillante. */
/** @} */

/**
 * @name Colores ANSI (fondo)
 * @{
 */
#define BG_RED "\033[41m"     /**< Fondo rojo. */
#define BG_GREEN "\033[42m"   /**< Fondo verde. */
#define BG_YELLOW "\033[43m"  /**< Fondo amarillo. */
#define BG_BLUE "\033[44m"    /**< Fondo azul. */
#define BG_MAGENTA "\033[45m" /**< Fondo magenta. */
#define BG_CYAN "\033[46m"    /**< Fondo cian. */
#define BG_WHITE "\033[47m"   /**< Fondo blanco. */
/** @} */

/**
 * @name Colores 256 (primer plano)
 * @{
 */
#define ORANGE "\033[38;5;208m" /**< Naranja (paleta 256). */
#define PINK "\033[38;5;205m"   /**< Rosado (paleta 256). */
#define PURPLE "\033[38;5;141m" /**< Purpura (paleta 256). */
#define TEAL "\033[38;5;37m"    /**< Verde azulado (paleta 256). */
#define GRAY "\033[38;5;245m"   /**< Gris (paleta 256). */
/** @} */

/**
 * @name Combinaciones frecuentes
 * @{
 */
#define BOLD_GREEN "\033[1m\033[32m"  /**< Negrita verde. */
#define BOLD_RED "\033[1m\033[31m"    /**< Negrita roja. */
#define BOLD_YELLOW "\033[1m\033[33m" /**< Negrita amarilla. */
#define BOLD_BLUE "\033[1m\033[34m"   /**< Negrita azul. */
/** @} */

/**
 * @brief Imprime una cantidad acotada de deportistas por consola.
 *
 * @param deportistas Arreglo de deportistas.
 * @param rankingAmount Cantidad de elementos a imprimir.
 */
void print_deportistas_array(Deportista *deportistas, int rankingAmount);

#endif
