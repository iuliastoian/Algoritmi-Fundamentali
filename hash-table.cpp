#include <iostream>
#include <stdlib.h>
#include "Profiler.h"

#define INTMIN (1 << 31) * (-1) 
#define INTMAX 1 << 30

Profiler profiler;

#define SIZE 9973
#define C1 3
#define C2 5

struct effort {
	double MAXIMUM_FOUND_EFFORT;
	double MAXIMUM_NOT_FOUND_EFFORT;
	double TOTAL_NOT_FOUND_EFFORT;
	double TOTAL_FOUND_EFFORT;
};

struct HashNode {
	int key;
	bool empty;
};

struct HashTable {
	int size;
	struct HashNode* array;
};

struct HashTable* createHashTable(int rang);
int hash(int key, int rang);
int* createRandomArray(int arr_size);
void insertRandomArray(int* arr, int arr_size, HashTable*& hash_table);
int* copyHalfArray(int* arr, HashTable* hash_table, int arr_size);
int* createOutsidersArray(HashTable* hash_table, int arr_size);
void insert(int key, HashTable*& hash_table);
bool search(int key, HashTable* hash_table, effort& EFFORT_VALUES);
void computeEffort(int* found_arr, int* not_found_arr, int arr_size, HashTable* hash_table, effort& EFFORT_VALUES);
int getSize(int rang);
void showHashTable(HashTable* hash_table);
void showArray(int arr[], int arr_size);
void emptyEffort(effort& EFFORT_VALUES);
void generateAverageCase();

double LOAD_FACTOR[] = { 0.8, 0.85, 0.9, 0.95, 0.99 };

void test() {
	int arr_size = getSize(0);
	//double MAXIMUM_FOUND_EFFORT = 0.0, MAXIMUM_NOT_FOUND_EFFORT = 0.0, TOTAL_NOT_FOUND_EFFORT = 0.0, TOTAL_FOUND_EFFORT = 0.0;
	HashTable* hash_table = createHashTable(0); std::cout << "createHashTable successfully executed.\n";
	int* arr = createRandomArray(arr_size); std::cout << "createRandomArray successfully executed.\n";
	insertRandomArray(arr, arr_size, hash_table); std::cout << "insertRandomArray successfully executed.\n";
	showHashTable(hash_table);
	int* copy_arr = copyHalfArray(arr, hash_table, arr_size); std::cout << "copyHalfArray successfully executed.\n";
	free(arr);
	int* outsiders = createOutsidersArray(hash_table, arr_size); std::cout << "createOutsidersArray successfully executed.\n";
	effort EFFORT_VALUES;
	emptyEffort(EFFORT_VALUES);
	computeEffort(copy_arr, outsiders, arr_size / 2, hash_table, EFFORT_VALUES); std::cout << "computeEffort successfully executed.\n";
	
	free(copy_arr);
	free(outsiders);
	//free(hash_table->array);
	free(hash_table);

	std::cout << "LOAD FACTOR    ";
	std::cout << "MAXIMUM FOUND EFFORT    ";
	std::cout << "MEDIUM FOUND EFFORT    ";
	std::cout << "MAXIM NOT FOUND EFFORT     ";
	std::cout << "MEDIUM NOT FOUND EEFORT    ";
	std::cout << '\n';

	std::cout << LOAD_FACTOR[0] << "			";
	std::cout << EFFORT_VALUES.MAXIMUM_FOUND_EFFORT << "			";
	std::cout << EFFORT_VALUES.TOTAL_FOUND_EFFORT << "			";
	std::cout << EFFORT_VALUES.MAXIMUM_NOT_FOUND_EFFORT << "			";
	std::cout << EFFORT_VALUES.TOTAL_NOT_FOUND_EFFORT << "			";
	std::cout << '\n';
}

int main() {
	//test();

	generateAverageCase();

	return 0;
}

struct HashTable* createHashTable(int rang) {
	HashTable* hash_table = NULL;
	int i;

	if (SIZE < 1) return NULL;

	/* Allocate the table itself. */
	if ((hash_table = (HashTable*)malloc(sizeof(HashTable))) == NULL) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if ((hash_table->array = (HashNode*)malloc(SIZE * sizeof(HashNode))) == NULL) {
		return NULL;
	}
	for (i = 0; i < SIZE; i++) {
		hash_table->array[i].key = NULL;
		hash_table->array[i].empty = true;
	}

	hash_table->size = SIZE;

	return hash_table;


	/*HashTable* hash_table = new HashTable;
	//HashTable* hash_table = (HashTable*)malloc(1 * sizeof(HashTable));
	hash_table->size = hash_size;

	hash_table->array = new HashNode[hash_table->size];
	//hash_table->array = (HashNode*)malloc(hash_size * sizeof(HashNode));
	
	for (int i = 0; i < SIZE; ++i) {
		hash_table->array[i].key = 0;
		hash_table->array[i].empty = true;
	}
	
	std::cout << "Hash Table created.\n";
	return hash_table;*/
}

int* createRandomArray(int arr_size) {
	//std::cout << "Number of elements: " << arr_size << '\n';

	int* arr = (int*)malloc(arr_size * sizeof(int));
	FillRandomArray(arr, arr_size, 10, 20000, true);

	//showArray(arr, arr_size);

	return arr;
}

int* createOutsidersArray(HashTable* hash_table, int arr_size) {
	int outsiders_size = arr_size / 2;
	
	int* outsiders = (int*)malloc(outsiders_size * sizeof(int));
	FillRandomArray(outsiders, outsiders_size, 20001, 40000, true);

	//showArray(outsiders, outsiders_size);

	return outsiders;
}

void insertRandomArray(int* arr, int arr_size, HashTable*& hash_table) {
	for (int i = 0; i < arr_size; ++i)
		insert(arr[i], hash_table);
}

int* copyHalfArray(int* arr, HashTable* hash_table, int arr_size) {
	int copy_arr_size = arr_size / 2;

	int* copy_arr = (int*)malloc(copy_arr_size * sizeof(int));

	for (int i = 0; i < copy_arr_size; ++i)
		copy_arr[i] = arr[i];

	//showArray(copy_arr, copy_arr_size);

	return copy_arr;
}

void computeEffort(int* found_arr, int* not_found_arr, int arr_size, HashTable* hash_table, effort& EFFORT_VALUES) {

	for (int i = 0; i < arr_size; ++i) {
		if (search(found_arr[i], hash_table, EFFORT_VALUES) == false)
			;// std::cout << "Existing key not found. Error!\n";
		if (search(not_found_arr[i], hash_table, EFFORT_VALUES) == true)
			;// std::cout << "Non-existeng key found. Error!\n";
	}
	
	double aux = (double)arr_size;

	EFFORT_VALUES.TOTAL_FOUND_EFFORT = EFFORT_VALUES.TOTAL_FOUND_EFFORT / aux;
	EFFORT_VALUES.TOTAL_NOT_FOUND_EFFORT = EFFORT_VALUES.TOTAL_NOT_FOUND_EFFORT / aux;
}

void insert(int key, HashTable*& hash_table) {
	for (int rang = 0; rang < SIZE; ++rang) {
		int index = hash(key, rang);

		if (hash_table->array[index].empty == true) {
				hash_table->array[index].key = key;
				hash_table->array[index].empty = false;
				break;
			}
	}
}

bool search(int key, HashTable* hash_table, effort& EFFORT_VALUES) {
	int rang = 0;
	double aux = (double)rang;

	for (rang = 0; rang < SIZE; ++rang) {
		int index = hash(key, rang);
		aux = aux + 1.0;

		if (hash_table->array[index].key == key) {
			if (EFFORT_VALUES.MAXIMUM_FOUND_EFFORT < aux)
				EFFORT_VALUES.MAXIMUM_FOUND_EFFORT = aux;
			EFFORT_VALUES.TOTAL_FOUND_EFFORT = EFFORT_VALUES.TOTAL_FOUND_EFFORT + aux;
			//std::cout << "Key: " << key << " found with: " << rang << " steps.\n";
			return true;
		}
		else if (hash_table->array[index].empty == true) {
			if (EFFORT_VALUES.MAXIMUM_NOT_FOUND_EFFORT < aux)
				EFFORT_VALUES.MAXIMUM_NOT_FOUND_EFFORT = aux;
			EFFORT_VALUES.TOTAL_NOT_FOUND_EFFORT = EFFORT_VALUES.TOTAL_NOT_FOUND_EFFORT + aux;
			//std::cout << "Key: " << key << " not found with: " << rang << " steps.\n";
			return false;
		}
	}
	//std::cout << "TOTAL FOUND EFFORT IN FUNCTION" << EFFORT_VALUES.TOTAL_FOUND_EFFORT << '\n';
	if (EFFORT_VALUES.MAXIMUM_NOT_FOUND_EFFORT < aux)
		EFFORT_VALUES.MAXIMUM_NOT_FOUND_EFFORT = aux;
	return false;
}

int getSize(int rang) {
	return (LOAD_FACTOR[rang] * SIZE);
}

int hash(int key, int rang) {
	return ((key + C1 * rang + C2 * rang * rang) % SIZE);
}

void showHashTable(HashTable* hash_table) {
	std::cout << "Hash Table of elements:\n";

	for (int i = 0; i < SIZE; ++i) {
		if (hash_table->array[i].empty == false)
			std::cout << "Element at position: " << i << " : " << hash_table->array[i].key << '\n';
		else std::cout << "Element at position: " << i << " : NULL element.\n";
	}
}

void showArray(int arr[], int arr_size) {
	std::cout << "Array of elements: ";

	for (int i = 0; i < arr_size; ++i)
		std::cout << arr[i] << ' ';
	std::cout << '\n';
}

void emptyEffort(effort& EFFORT_VALUES) {
	EFFORT_VALUES.MAXIMUM_FOUND_EFFORT = 0.0;
	EFFORT_VALUES.MAXIMUM_NOT_FOUND_EFFORT = 0.0;
	EFFORT_VALUES.TOTAL_FOUND_EFFORT = 0.0;
	EFFORT_VALUES.TOTAL_NOT_FOUND_EFFORT = 0.0;
}

void generateAverageCase() {
	std::cout << "\nLOAD FACTOR    ";
	std::cout << "MAXIMUM FOUND EFFORT    ";
	std::cout << "MEDIUM FOUND EFFORT    ";
	std::cout << "MAXIM NOT FOUND EFFORT     ";
	std::cout << "MEDIUM NOT FOUND EFFORT";
	std::cout << '\n';

	effort EFFORT_VALUES;
	emptyEffort(EFFORT_VALUES);

	for (int m = 0; m < 5; ++m) {
		emptyEffort(EFFORT_VALUES);
		for (int i = 0; i < 5; ++i) {
			int arr_size = getSize(i);
			HashTable* hash_table = createHashTable(0); //std::cout << "createHashTable successfully executed.\n";
			int* arr = createRandomArray(arr_size); //std::cout << "createRandomArray successfully executed.\n";
			insertRandomArray(arr, getSize(i), hash_table); //std::cout << "insertRandomArray successfully executed.\n";
			int* copy_arr = copyHalfArray(arr, hash_table, arr_size); //std::cout << "copyHalfArray successfully executed.\n";
			free(arr);
			int* outsiders = createOutsidersArray(hash_table, arr_size); //std::cout << "createOutsidersArray successfully executed.\n";
			computeEffort(copy_arr, outsiders, arr_size / 2, hash_table, EFFORT_VALUES); //std::cout << "computeEffort successfully executed.\n";

			free(copy_arr);
			free(outsiders);
			free(hash_table);
		}

		std::cout << LOAD_FACTOR[m] << "			";
		std::cout << EFFORT_VALUES.MAXIMUM_FOUND_EFFORT << "			";
		std::cout << EFFORT_VALUES.TOTAL_FOUND_EFFORT << "			";
		std::cout << EFFORT_VALUES.MAXIMUM_NOT_FOUND_EFFORT << "			";
		std::cout << EFFORT_VALUES.TOTAL_NOT_FOUND_EFFORT << "			";
		std::cout << '\n';
	}
}