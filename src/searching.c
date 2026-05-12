/**
 * @file searching.c
 * @brief Implementacion de algoritmos de busqueda sobre deportistas.
 */

#include "searching.h"
#include "sorting.h"

#include <math.h>

/**
 * @brief Ordena un arreglo de deportistas por ID de forma ascendente con insertion sort.
 *
 * Paso previo para algoritmos de busqueda que requieren que los datos esten ordenados.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param count Cantidad de deportistas almacenados en el arreglo.
 */
void sort_deportistas_by_id_ascending(Deportista *deportistas, int count)
{
	if(deportistas == NULL || count < 2) {
		return;
	}

	for(int i = 1; i < count; i++) {
		Deportista current = deportistas[i];
		int j = i - 1;

		while( j >= 0 &&deportistas[j] != NULL &&
			current != NULL &&
			compare_by_criteria(deportistas[j], current, SORT_BY_ID) > 0) {
			deportistas[j + 1] = deportistas[j];
			j--;
		}

		deportistas[j + 1] = current;
	}
}

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
		return binary_search_by_id_recursive(
			deportistas,
			mid + 1,
			right,
			targetId
		);
	}

	return binary_search_by_id_recursive(
		deportistas,
		left,
		mid - 1,
		targetId
	);
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
 * @brief Busca un rango de deportistas cuyo puntaje este entre lowScore y highScore.
 *
 * El arreglo es ordenado por puntaje ascendente usando insertion_sort_deportistas
 * antes de aplicar las busquedas binarias modificadas.
 *
 * @param deportistas Arreglo de deportistas (modificado por la ordenacion).
 * @param count Cantidad de elementos.
 * @param lowScore Limite inferior inclusive.
 * @param highScore Limite superior inclusive.
 * @param outLeft Direccion donde se escribe el indice del primer elemento encontrado (o -1).
 * @param outRight Direccion donde se escribe el indice del ultimo elemento encontrado (o -1).
 * @return Cantidad de elementos en el rango.
 */
int binary_search_by_range(Deportista *deportistas, int count, float lowScore, float highScore, int *outLeft, int *outRight)
{
	if(deportistas == NULL || count <= 0 || outLeft == NULL || outRight == NULL) {
		return 0;
	}

	if(lowScore > highScore) {
		int tmp = (int)lowScore; lowScore = highScore; highScore = tmp;
	}

	insertion_sort_deportistas(deportistas, count, SORT_BY_PUNTAJE, ASCENDING);

	int leftIndex = binary_search_by_score(deportistas, 0, count - 1, lowScore);
	int rightIndex = binary_search_by_score(deportistas, 0, count - 1, highScore);

	if(leftIndex == -1 || rightIndex == -1 || leftIndex > rightIndex) {
		*outLeft = -1;
		*outRight = -1;
		return 0;
	}

	*outLeft = leftIndex;
	*outRight = rightIndex;

	return (rightIndex - leftIndex + 1);
}

int interpolation_search_by_id_with_indexs(Deportista* deportistas, int start, int end, int targetId)
{
	if (deportistas == NULL || start > end) {
		return -1;
	}

	if (targetId < deportistas[start]->id || targetId > deportistas[end]->id) {
		return -1;
	}

	// {2, 5, 8, 15, 22, 35, 40, 68}

	// target = 35
	// n = 8

	// interpolation_search_by_id_with_indexs(deportistas, 0, 7, 35)

	// supposedIdx = 0 + (35 - 2)/(68 - 2) * (7 - 0)
	// supposedIdx = 0 + 33/66 * 7 = 3.5 = 3

	int supposedIdx = start + (float) (targetId - deportistas[start]->id) / (deportistas[end]->id - deportistas[start]->id) * (end-start);

	if (deportistas[supposedIdx]->id == targetId) {
		return supposedIdx;
	}

	if (deportistas[supposedIdx]->id < targetId) {
		return interpolation_search_by_id_with_indexs(deportistas, supposedIdx+1, end, targetId);
	}

	return interpolation_search_by_id_with_indexs(deportistas, start, supposedIdx-1, targetId);
}

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