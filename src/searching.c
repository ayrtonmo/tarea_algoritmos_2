/**
 * @file searching.c
 * @brief Implementacion de algoritmos de busqueda sobre deportistas.
 */

#include "searching.h"
#include "sorting.h"

#include <math.h>


/**
 * @brief Realiza una busqueda binaria recursiva por ID.
 *
 * Esta funcion requiere que el arreglo haya sido ordenado previamente por ID ascendente.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID.
 * @param left Indice inicial del rango de busqueda.
 * @param right Indice final del rango de busqueda.
 * @param targetId ID del deportista que se desea buscar.
 *
 * @return Indice donde se encuentra el deportista si existe.
 * @return -1 si el deportista no fue encontrado.
 */
int binary_search_by_id_recursive(Deportista *deportistas,int left,int right,int targetId)
{
	if(deportistas == NULL || left > right) {
		return -1;
	}

	int mid = left + (right - left) / 2;

	if(deportistas[mid] == NULL) {
		return -1;
	}

	if(deportistas[mid]->id == targetId) {
		return mid;
	}

	if(deportistas[mid]->id < targetId) {
		return binary_search_by_id_recursive(deportistas, mid + 1, right, targetId);
	}

	return binary_search_by_id_recursive(deportistas, left, mid - 1, targetId);
}

/**
 * @brief Busca recursivamente un puntaje exacto en un arreglo ordenado por puntaje ascendente.
 *
 * @param deportistas Arreglo ordenado por puntaje ascendente.
 * @param left Indice izquierdo del rango.
 * @param right Indice derecho del rango.
 * @param targetScore Puntaje a buscar.
 * @return Indice donde se encuentra un elemento con puntaje == targetScore, -1 si no existe.
 */
int binary_search_by_score(Deportista *deportistas, int left, int right, float targetScore)
{
	if(deportistas == NULL || left > right) {
		return -1;
	}

	int mid = left + (right - left) / 2;

	if(deportistas[mid] == NULL) {
		return -1;
	}

	if(deportistas[mid]->puntaje == targetScore) {
		return mid;
	}

	if(deportistas[mid]->puntaje < targetScore) {
		return binary_search_by_score(deportistas, mid + 1, right, targetScore);
	}

	return binary_search_by_score(deportistas, left, mid - 1, targetScore);
}


/**
 * @brief Busca recursivamente un puntaje exacto en un arreglo ordenado por puntaje ascendente.
 *
 * @param deportistas Arreglo ordenado por puntaje ascendente.
 * @param left Indice izquierdo del rango.
 * @param right Indice derecho del rango.
 * @param tagetId ID a buscar.
 * @return Indice donde se encuentra un elemento con puntaje == targetScore, -1 si no existe.
 */
int binary_search_by_id(Deportista *deportistas, int left, int right, int targetId)
{
	if(deportistas == NULL || left > right) {
		return -1;
	}

	int mid = left + (right - left) / 2;

	if(deportistas[mid] == NULL) {
		return -1;
	}

	if(deportistas[mid]->id == targetId) {
		return mid;
	}

	if(deportistas[mid]->id < targetId) {
		return binary_search_by_id(deportistas, mid + 1, right, targetId);
	}

	return binary_search_by_id(deportistas, left, mid - 1, targetId);
}


/**
 * @brief Busca el primer indice con puntaje mayor o igual al limite inferior.
 *
 * En vez de buscar una coincidencia exacta, conserva el mejor candidato sigue 
 * buscando hacia la izquierda para ubicar la primera posicion valida dentro del rango.
 *
 * @param deportistas Arreglo de deportistas ordenado por puntaje ascendente.
 * @param left Indice inicial del rango de busqueda.
 * @param right Indice final del rango de busqueda.
 * @param lowScore Limite inferior inclusivo del rango.
 * @param bestIndex Mejor indice candidato encontrado hasta el momento.
 * @return int Primer indice con puntaje mayor o igual a lowScore, o -1 si no existe.
 */
static int find_first_score_at_least(Deportista *deportistas, int left, int right, float lowScore, int bestIndex)
{
	if(left > right) {
		return bestIndex;
	}

	int mid = left + (right - left) / 2;

	if(deportistas[mid] == NULL) {
		return bestIndex;
	}

	if(deportistas[mid]->puntaje >= lowScore) {
		return find_first_score_at_least(deportistas, left, mid - 1, lowScore, mid);
	}

	return find_first_score_at_least(deportistas, mid + 1, right, lowScore, bestIndex );
}

/**
 * @brief Busca el ultimo indice con puntaje menor o igual al limite superior.
 *
 * Exactamente mismo funcionamiento que find_first_score_at_least, pero busca hacia la derecha.
 *
 * @param deportistas Arreglo de deportistas ordenado por puntaje ascendente.
 * @param left Indice inicial del rango de busqueda.
 * @param right Indice final del rango de busqueda.
 * @param highScore Limite superior inclusive del rango.
 * @param bestIndex Mejor indice candidato encontrado hasta el momento.
 * @return int Ultimo indice con puntaje menor o igual a highScore, o -1 si no existe.
 */
static int find_last_score_at_most(Deportista *deportistas, int left, int right, float highScore, int bestIndex)
{
	if(left > right) {
		return bestIndex;
	}

	int mid = left + (right - left) / 2;

	if(deportistas[mid] == NULL) {
		return bestIndex;
	}

	if(deportistas[mid]->puntaje <= highScore) {
		return find_last_score_at_most(deportistas, mid + 1, right, highScore, mid);
	}

	return find_last_score_at_most(deportistas, left, mid - 1, highScore, bestIndex );
}

/**
 * @brief Busca un rango de deportistas cuyo puntaje este entre lowScore y highScore.
 *
 * Esta funcion ordena el arreglo por puntaje ascendente y luego delega
 * la busqueda a binary_search_by_range_sorted, que asume que los datos
 * ya se encuentran ordenados. De esta forma, el menu puede usar esta
 * funcion completa, mientras que los benchmarks pueden medir solo la
 * busqueda usando la version sorted.
 *
 * @param deportistas Arreglo de deportistas, modificado por la ordenacion.
 * @param count Cantidad de elementos.
 * @param lowScore Limite inferior inclusive.
 * @param highScore Limite superior inclusive.
 * @param outLeft Direccion donde se escribe el primer indice dentro del rango.
 * @param outRight Direccion donde se escribe el ultimo indice dentro del rango.
 * @return int Cantidad de elementos encontrados en el rango.
 */
int binary_search_by_range(Deportista *deportistas, int count, float lowScore, float highScore, int *outLeft, int *outRight)
{
	if(deportistas == NULL || count <= 0 || outLeft == NULL || outRight == NULL) {
		return 0;
	}

	insertion_sort_deportistas(deportistas, count, SORT_BY_PUNTAJE, ASCENDING);

	return binary_search_by_range_sorted(deportistas, count, lowScore, highScore, outLeft, outRight);
}

/**
 * @brief Busca deportistas dentro de un rango de puntaje en un arreglo ya ordenado (para benchmarking).
 *
 * @param deportistas Arreglo de deportistas ordenado por puntaje ascendente.
 * @param count Cantidad de deportistas.
 * @param lowScore Limite inferior inclusive.
 * @param highScore Limite superior inclusive.
 * @param outLeft Direccion donde se guarda el primer indice encontrado.
 * @param outRight Direccion donde se guarda el ultimo indice encontrado.
 * @return int Cantidad de deportistas encontrados dentro del rango.
 */
int binary_search_by_range_sorted(Deportista *deportistas, int count, float lowScore, float highScore, int *outLeft, int *outRight)
{
    if(deportistas == NULL || count <= 0 || outLeft == NULL || outRight == NULL) {
        return 0;
    }

    *outLeft = -1;
    *outRight = -1;

    if(lowScore > highScore) {
        float tmp = lowScore;
        lowScore = highScore;
        highScore = tmp;
    }

    int leftIndex = find_first_score_at_least(deportistas, 0, count - 1, lowScore, -1);

    int rightIndex = find_last_score_at_most(deportistas, 0, count - 1, highScore, -1);

    if(leftIndex == -1 || rightIndex == -1 || leftIndex > rightIndex) {
        return 0;
    }

    *outLeft = leftIndex;
    *outRight = rightIndex;

    return rightIndex - leftIndex + 1;
}

/**
 * @brief Realiza una busqueda por interpolación lineal recursiva por ID.
 *
 * Requiere que el arreglo haya sido ordenado previamente por ID ascendente.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID.
 * @param start Indice inicial del rango de busqueda.
 * @param end Indice final del rango de busqueda.
 * @param targetId ID del deportista que se desea buscar.
 *
 * @return Indice donde se encuentra el deportista si existe.
 * @return -1 si el deportista no fue encontrado.
 */
int interpolation_search_by_id_with_indexs(Deportista* deportistas, int start, int end, int targetId)
{
	if (deportistas == NULL || start > end) {
		return -1;
	}

	if (targetId < deportistas[start]->id || targetId > deportistas[end]->id) {
		return -1;
	}

	int supposedIdx = start + (float) (targetId - deportistas[start]->id) / (deportistas[end]->id - deportistas[start]->id) * (end-start);

	if (deportistas[supposedIdx]->id == targetId) {
		return supposedIdx;
	}

	if (deportistas[supposedIdx]->id < targetId) {
		return interpolation_search_by_id_with_indexs(deportistas, supposedIdx+1, end, targetId);
	}

	return interpolation_search_by_id_with_indexs(deportistas, start, supposedIdx-1, targetId);
}


/**
 * @brief Realiza una busqueda por interpolacion por ID.
 *
 * Esta funcion inicializa la busqueda sobre el arreglo completo utilizando
 * interpolacion recursiva.
 *
 * Requiere que el arreglo haya sido ordenado previamente por ID ascendente.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID.
 * @param count Cantidad de deportistas almacenados en el arreglo.
 * @param targetId ID del deportista que se desea buscar.
 *
 * @return int Indice donde se encuentra el deportista si existe.
 * @return -1 si el deportista no fue encontrado.
 */
int interpolation_search_by_id(Deportista* deportistas, int count, int targetId)
{
	return interpolation_search_by_id_with_indexs(deportistas, 0, count-1, targetId);
}


/**
 * @brief Realiza una busqueda exponencial por ID.
 *
 * Revisa si el elemento buscado se encuentra en la primera posicion. Si no es asi, aumenta
 * el limite superior de busqueda de forma exponencial, duplicando el valor de bound hasta
 * encontrar un rango donde podria estar el ID buscado. Se aplica b. binaria recursiva dentro
 * del rango encontrado.
 *
 * Requiere que el arreglo haya sido ordenado previamente por ID ascendente.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID.
 * @param count Cantidad de deportistas almacenados en el arreglo.
 * @param targetId ID del deportista que se desea buscar.
 *
 * @return Indice donde se encuentra el deportista si existe.
 * @return -1 si el deportista no fue encontrado.
 */
int exponential_search_by_id(Deportista *deportistas,int count,int targetId)
{
	if(deportistas == NULL || count <= 0 || deportistas[0] == NULL) {
		return -1;
	}

	if(deportistas[0]->id == targetId) {
		return 0;
	}

	int bound = 1;

	while(
		bound < count &&
		deportistas[bound] != NULL &&
		deportistas[bound]->id < targetId
	) {
		bound *= 2;
	}

	int left = bound / 2;
	int right = bound;

	if(right >= count) {
		right = count - 1;
	}

	return binary_search_by_id_recursive(
		deportistas,
		left,
		right,
		targetId
	);
}