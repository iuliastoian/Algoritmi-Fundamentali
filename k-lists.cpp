#include "Profiler.h"
#include <iostream>

#define INTMIN (1 << 31) * (-1) 
#define INTMAX 1 << 30

Profiler profiler;

struct value_source {
	int value;
	int source;
};

struct node {
	int data;
	int source;
	node* next;
};

struct listOfNodes {
	int arr_size;
	struct node* head;
};

struct listOfLists {
	struct listOfNodes* array;
}k_lists;

inline int parent(int index) { return ((index - 1) >> 1); }
inline int leftChild(int index) { return ((index << 1) + 1); }
inline int rightChild(int index) { return ((index << 1) + 2); }

struct listOfLists* createListOfLists(int k, int n);
void generateRandomSortedArrays(int k, int n, listOfLists*& k_lists);
void updateList(node*& p, int arr[], int arr_size, int noSource);
void getMinValues(node*& min_heap, int k, listOfLists* k_lists);
struct node* getNodeAt(int index, node* root);
void buildMinHeap(int k, node* &min_heap, int size);
void heapify(node*& min_heap, node* index_node, int index, int heap_size, int k, int size);
void swapNodes(node* p, int p_index, node* q, int q_index, node** min_heap);
struct node* createFinalList(node* min_heap, listOfLists* k_lists, int k, int size);
struct value_source peek(node*& min_heap);
void deleteNodeBeginning(node*& aux_node);
void deleteNodeEnd(node*& aux_node);
void addToBeginning(node*& p, value_source value);
void addToBack(node*& p, value_source value);
void showList(node* p);
void showListSource(node* p);
void createAverageGraph(int k);
void createIncreasingGraph();

bool increasing = false;

const char* getChar(int k) {
	if (!increasing)
	{
		if (k == 5) {
			const char* op = "k1";
			return op;
		}

		if (k == 10) {
			const char* op = "k2";
			return op;
		}

		if (k == 100) {
			const char* op = "k3";
			return op;
		}
	}

	const char* op = "operations";
	return op;
}

void test(int k, int n) {
	listOfLists* k_lists = createListOfLists(k, n);		// se creeaza si se aloca memorie pentu lista de liste si pentru cele k liste
	generateRandomSortedArrays(k, n, k_lists);			// se creeaza k tablouri sortate de elemente aleatoare si se copiaza in cele k liste

	for (int i = 0; i < k; ++i) { std::cout << "Lista sursa nr. " << i << " cu " << k_lists->array[i].arr_size << " elemente: "; showList(k_lists->array[i].head); }

	node* min_heap = NULL;										// se creeaza o lista din minimele (adica primul element) din fiecare lista
	getMinValues(min_heap, k, k_lists);							// min_heap este nodul root al min heap-ului

	std::cout << "Elemente cu care se va forma Min Heap-ul: ";  showList(min_heap);

	buildMinHeap(k, min_heap, n);							
	std::cout << "Min Heap: "; showList(min_heap);				// se creeaza min heap-ul 

	node* final = createFinalList(min_heap, k_lists, k, n);
	std::cout << "Lista finala interclasata: "; showList(final); showListSource(min_heap);	// se creeaza lista finala
}

int main() {
	test(4, 20);

	/*createAverageGraph(5);
	createAverageGraph(10);
	createAverageGraph(100);
	
	profiler.createGroup("Operations", "k1", "k2", "k3");
	profiler.showReport();
	profiler.reset();*/

	/*createIncreasingGraph();
	profiler.createGroup("k varies", "operations");
	profiler.showReport();*/

	return 0;
}

struct listOfLists* createListOfLists(int k, int n) {
	/*
		Se creeaza si se aloca memorie pentru o lista care va contine k liste.
	*/

	struct listOfLists* k_lists = (struct listOfLists*) malloc(sizeof(struct listOfLists));

	/*
		Se creeaza si se aloca memorie pentru cele k liste din k_lists.
		Se specifica dimenziunea fiecarei liste: (k - 1) liste a cate (n / k) elemente
		si o lista cu (n / k + n % k) elemente.
	*/

	k_lists->array = (struct listOfNodes*) malloc(n * sizeof(struct listOfNodes));

	for (int i = 0; i < k; ++i)
		k_lists->array[i].arr_size = n / k;

	k_lists->array[k - 1].arr_size += (n % k);

	/*
		Se initializeaza fiecare lista ca fiind goala facandu-i head-ul null.
	*/

	for (int i = 0; i < k; ++i)
		k_lists->array[i].head = NULL;

	return k_lists;
}

void generateRandomSortedArrays(int k, int n, listOfLists*& k_lists) {

	/*
		Se genereaza k tablouri a cate [n/k] elemente random sortate, dupa
		se copiaza valoarea lor in cele k liste.
	*/

	int noSource = 0;

	for (int i = 0; i < k; ++i)
	{
		int* arr = (int*)malloc(n * sizeof(int));
		FillRandomArray(arr, n, 100, 10000, false, 1);
		
		updateList(k_lists->array[i].head, arr, k_lists->array[i].arr_size, noSource);
		free(arr);
		++noSource;
	}
}

void updateList(node* &p, int arr[], int arr_size, int noSource) {
	p = new node;		// primul nod nu se declara inca o data, doar se aloca memorie
	p->data = arr[0];
	p->next = NULL;
	p->source = noSource;

	node* p_copy = new node;
	p_copy = p;

	for (int i = 1; i < arr_size; ++i)
	{
		node* temp = new node;
		temp->data = arr[i];
		temp->next = NULL;
		temp->source = noSource;

		p_copy->next = temp; // se face legatura cu nodul precedent
		p_copy = p_copy->next; // se actualizeaza pozitia unde se vor adauga urmatoarele valori
	}
}

void getMinValues(node*& min_heap, int k, listOfLists* k_lists) {
	for (int i = 0; i < k; ++i)
	{
		value_source value;
		value.value = k_lists->array[i].head->data;
		value.source = k_lists->array[i].head->source;

		addToBack(min_heap, value);
	}
}

struct node* getNodeAt(int index, node* root) {
	node* wanted = root;

	int cnt = 0;

	while (cnt <= index)
	{
		if (cnt == index)
		{
			return wanted;
		}
		wanted = wanted->next;
		++cnt;
	}

	return NULL;
}

void buildMinHeap(int k, node* &min_heap, int size) {

	for (int aux = k / 2 - 1; aux >= 0; --aux)
	{
		node* p = getNodeAt(aux, min_heap);
		heapify(min_heap, p, aux, k, k, size);
	}
}

void heapify(node*& min_heap, node* index_node, int index, int heap_size, int k, int size) {
	Operation operation = profiler.createOperation(getChar(k), size);

	node* key = index_node;	//key will store the pointer that has the smaller number between the parent, leftChild and rightChild
	int key_index = index;

	node* left = NULL;
	int left_index = leftChild(index);

	node* right = NULL;
	int right_index = rightChild(index);

	if (left_index < heap_size)
	{
		left = getNodeAt(leftChild(index), min_heap);

		operation.count(1); // comparatie
		if (left->data < index_node->data)
		{
			key = left;
			key_index = left_index;
		}
	}

	if (right_index < heap_size)
	{
		right = getNodeAt(rightChild(index), min_heap);

		operation.count(1); // comparatie
		if (right->data < key->data)
		{
			key = right;
			key_index = right_index;
		}
	}

	if (index != key_index)
	{
		operation.count(3); // swap
		swapNodes(index_node, index, key, key_index, &min_heap);
		heapify(min_heap, index_node, key_index, heap_size, k, size);
	}
}

void swapNodes(node* p, int p_index, node* q, int q_index, node** min_heap) {
	node* prev_p = new node;
	node* prev_q = new node;
	
	if (p_index == 0)
		prev_p = NULL;
	else prev_p = getNodeAt(p_index - 1, *min_heap);

	if (q_index == 0)
		prev_q = NULL;
	else prev_q = getNodeAt(q_index - 1, *min_heap);

	if (NULL != prev_p)	// daca p nu este inceput de lista
	{
		prev_p->next = q;
	}
	else { *min_heap = q; }

	if (NULL != prev_q)	// daca q nu este inceput de lista
	{
		prev_q->next = p;
	}
	else { *min_heap = p; }

	node* temp = q->next;
	q->next = p->next;
	p->next = temp;
}

struct node* createFinalList(node* min_heap, listOfLists* k_lists, int k, int size) {
	Operation operation = profiler.createOperation(getChar(k), size);
	
	int noOfLists = k;
	int heap_size = k;

	node* final = NULL;

	while (noOfLists > 0)
	{
		value_source min_value = peek(min_heap);							// vedem si memoram valoare minima din heap
		addToBack(final, min_value);										// adaugam in lista finala valoarea minima din heap
		deleteNodeBeginning(k_lists->array[min_value.source].head);			// stergem valoarea din lista sursa
		operation.count(3);
		
			// adaugam in heap urmatoarea valoare din lista sursa, daca exista facem pop() de varf
			// daca lista sursa este goala, atunci punem ultimul element pe prima pozitie, micsoram
			// dimensiunea min heap-ului
			// apelam heapify()
		
		if (k_lists->array[min_value.source].head == NULL) // daca lista sursa este goala
		{
			node* last_node = getNodeAt(heap_size - 1, min_heap);				// memoram ultima valoare din min heap
			value_source last_node_aux;
			last_node_aux.value = last_node->data;
			last_node_aux.source = last_node->source;

			deleteNodeBeginning(min_heap);										// stergem primul nod din min heap
			addToBeginning(min_heap, last_node_aux);							// adaugam ultima valoare memorata pe prima pozitie in min heap
			deleteNodeEnd(min_heap);

			--noOfLists;
			--heap_size;
		}
		else
		{
			value_source aux;
			aux.source = min_value.source;
			aux.value = k_lists->array[min_value.source].head->data; // urmatoarea valoare din lista sursa

			deleteNodeBeginning(min_heap); // stergem prima valoare din min heap
			addToBeginning(min_heap, aux); // adaugam urmatoarea valoare in heap
		}
		heapify(min_heap, min_heap, 0, heap_size, k, size);
	}
	return final;
}

void addToBeginning(node*& p, value_source value) {
	if (p == NULL)
	{
		// primul nod nu se declara inca odata, doar se aloca memorie
		p = new node;
		p->data = value.value;
		p->source = value.source;
		p->next = NULL;
	}
	else
	{
		node* q = new node;
		q->data = value.value;
		q->source = value.source;
		q->next = p;

		// facem legatura cu nodul precedent
		p = q;
	}
}

void addToBack(node*& p, value_source value)
{
	if (NULL == p)
	{
		// primul nod nu se declara inca o data, doar se aloca memorie
		p = new node;
		p->data = value.value;
		p->source = value.source;
		p->next = NULL;

	}
	else
	{
		node* r = p;

		for (; r->next; r = r->next);

		node* q = new node;
		q->data = value.value;
		q->source = value.source;
		q->next = NULL;

		// se facem legatura cu nodul precedent
		r->next = q;
	}
}

struct value_source peek(node*& min_heap) {
	value_source min;
	min.value = min_heap->data;
	min.source = min_heap->source;

	return min;
}

void deleteNodeBeginning(node*& aux_node) {
	node* aux = aux_node;

	aux_node = aux_node->next;

	delete aux;
}

void deleteNodeEnd(node*& aux_node) {
	if (NULL == aux_node)
		return;

	if (NULL == aux_node->next) {
		delete aux_node;
		return;
	}

	// Find the second last node 
	node* second_last = aux_node;
	while (second_last->next->next != NULL)
		second_last = second_last->next;

	// Delete last node 
	delete (second_last->next);

	// Change next of second last 
	second_last->next = NULL;
}

void showList(node* p) {
	while (NULL != p) {
		std::cout << p->data << ' ';
		p = p->next;
	}
	std::cout << '\n';
}

void showListSource(node* p) {
	while (NULL != p) {
		std::cout << p->source << ' ';
		p = p->next;
	}
	std::cout << '\n';
}

void createAverageGraph(int k) {
	for (int n = 500; n <= 10000; n += 500)
	{
		std::cout << "for k: " << k << " step: " << n << '\n';

		listOfLists* k_lists = createListOfLists(k, n);
		generateRandomSortedArrays(k, n, k_lists);

		node* min_heap = NULL; getMinValues(min_heap, k, k_lists);
		buildMinHeap(k, min_heap, n);

		node* final = createFinalList(min_heap, k_lists, k, n);
	}
}

void createIncreasingGraph() {
	int n = 10000; increasing = true;

	for (int k = 10; k <= 500; k += 10)
	{
		std::cout << "for k: " << k << '\n';

		listOfLists* k_lists = createListOfLists(k, n);
		generateRandomSortedArrays(k, n, k_lists);

		node* min_heap = NULL; getMinValues(min_heap, k, k_lists);
		buildMinHeap(k, min_heap, k);

		node* final = createFinalList(min_heap, k_lists, k, k);
	}
}