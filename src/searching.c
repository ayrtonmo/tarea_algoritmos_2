/**
 * @file searching.c
 * @brief Implementacion de algoritmos de busqueda sobre deportistas.
 */

#include "searching.h"
#include "sorting.h"

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

int interpolation_search_by_id_with_indexs(Deportista* deportistas, int start, int end)
{


}

int interpolation_search_by_id(Deportista* deportistas, int count)
{
	if(deportistas == NULL || count <= 0 || deportistas[0] == NULL) {
		return -1;
	}

	


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