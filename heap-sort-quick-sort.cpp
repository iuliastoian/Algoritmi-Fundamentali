/*
				BEST CASE		AVERAGE CASE		WORST CASE
	HeapSort	O(n*logn)		O(n*logn)			O(n^2)
	QuickSort	O(n*logn)		O(n*logn)			O(n*logn)
*/

#include "Profiler.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

#define maxInt (1 << 30)
#define minInt (1 << 31) * (-1)

Profiler profiler;

inline int parent(int index) { return (index >> 1); }
inline int leftChild(int index) { return (index << 1); }
inline int rightChild(int index) { return (index << 1) + 1; }

inline int random(int inferior, int superior) {	return (inferior + rand() % (superior - inferior)); }

void heapSort(int heap[], int size);
void buildMaxHeap(int heap[], int size);
void heapify(int heap[], int heapSize, int index, int size);

void quickSort(int arr[], int inferior, int superior, int size);
int partition(int arr[], int inferior, int superior, int size);

void quickSortBest(int arr[], int inferior, int superior, int size);

void randomizedQuickSort(int arr[], int inferior, int superior, int size);
int randomizedPartition(int arr[], int inferior, int superior, int size);

void swap(int* x, int* y);
void show(int heap[], int size);

void createBestGraph();
void createAverageGraph();
void createWorstGraph();


void test(int sortCase, int size) {
	int* arr = (int*)malloc((size + 1) * sizeof(int));
	FillRandomArray(arr, size + 1, 1, 100, false, 2);

	if (sortCase == 1)
	{
		heapSort(arr, size);
	}
	else if (sortCase == 2)
	{
		quickSort(arr, 1, size, size);
	}
	else if (sortCase == 3)
	{
		randomizedQuickSort(arr, 1, size, size);
	}

	show(arr, size);
	free(arr);
}

int main() {
	//int n = 30;
	
	//test(1, n); // Heap Sort
	//test(2, n); // Quick Sort
	//test(3, n); // Randomized Quick Sort
	
	std::cout << "Start Generating Graphs:\n";

	//createAverageGraph();
	//profiler.reset();

	//createWorstGraph();
	//profiler.reset();

	createBestGraph();
	profiler.showReport();

	return 0;
}

void heapSort(int heap[], int size) {
	/*
		HeapSort is a comparison based sorting technique based on Binary Heap data structure.
		It is similar to Selection Sort where we first find the maximum element and place the maximum element at the end.
	*/
	
	buildMaxHeap(heap, size);
	
	/*
		After we build a max heap, it is obvious that the maximum value will be stored in the root of the heap.
		The maximum value will be swappped with the value of the last element of the heap.
		The size of the heap will be decremented by 1, and then the function heapify() will be called to maintain
		the max heap property.
	*/

	int heapSize = size;
	for (int index = heapSize; index > 1; --index)
	{
		profiler.countOperation("Heap-Sort Operations", size, 3);
		swap(&heap[index], &heap[1]);

		--heapSize;
		heapify(heap, heapSize, 1, size);
	}
}

void buildMaxHeap(int heap[], int size) {

	for (int i = size / 2; i >= 1; --i)
	{
		heapify(heap, size, i, size);
	}
}

void heapify(int heap[], int heapSize, int index, int size) {
	/*
		Heapify O(logn) * buildMaxHeap O(n) => HeapSort O(n*logn)
	*/

	Operation heapSortOperations = profiler.createOperation("Heap-Sort Operations", size);

	int key = index; //key stores the index of the greater number between the parent, leftChild and rightChild

	int left = leftChild(index);
	int right = rightChild(index);

	if (left <= heapSize)
	{
		heapSortOperations.count(1);
		if (heap[left] > heap[index]) //if leftChild exists and its value is bigger than its parent's
			key = left;
	}

	if (right <= heapSize)
	{
		heapSortOperations.count(1);
		if (heap[right] > heap[key]) //if rightChild exists and its value is bigger than max(parent, leftChild)
			key = right;
	}

	if (key != index) //if the values are not in correct place
	{
		heapSortOperations.count(3);
		swap(&heap[index], &heap[key]);
		heapify(heap, heapSize, key, size); //heapify from the index of the Child we swapped values with
	}
}

void quickSort(int arr[], int inferior, int superior, int size) {
	if (inferior < superior)
	{
		int pivot = partition(arr, inferior, superior, size);
		quickSort(arr, inferior, pivot - 1, size);
		quickSort(arr, pivot + 1, superior, size);
	}
}

int partition(int arr[], int inferior, int superior, int size) {
	Operation quickSortOperations = profiler.createOperation("Quick-Sort Operations", size);
	
	int pivot = arr[superior];
	int i = (inferior - 1);

	for (int j = inferior; j <= (superior - 1); ++j)
	{
		quickSortOperations.count(1);
		if (arr[j] < pivot)
		{
			++i;
			quickSortOperations.count(3);
			swap(&arr[i], &arr[j]);
		}
	}
	quickSortOperations.count(3);
	swap(&arr[i + 1], &arr[superior]);
	return (i + 1);
}

void quickSortBest(int arr[], int inferior, int superior, int size) {
	Operation quickSortOperations = profiler.createOperation("Quick-Sort Operations", size);
	
	int pivot, i, j;

	i = inferior;
	j = superior;
	pivot = arr[(i + j) / 2];
	quickSortOperations.count(1);

	do {
		quickSortOperations.count(1);
		while ((i < superior) && (arr[i] < pivot)) ++i;
		quickSortOperations.count(1);
		while ((j > inferior) && (arr[j] > pivot)) --j;

		if (i <= j)
		{
			quickSortOperations.count();
			swap(&arr[i], &arr[j]);
			++i;
			--j;
		}

	} while (i <= j);

	if (inferior < j) quickSortBest(arr, inferior, j, size);
	if (i < superior) quickSortBest(arr, i, superior, size);
}

void randomizedQuickSort(int arr[], int inferior, int superior, int size) {
	if (inferior < superior)
	{
		int pivot = randomizedPartition(arr, inferior, superior, size);
		randomizedQuickSort(arr, inferior, pivot - 1, size);
		randomizedQuickSort(arr, pivot + 1, superior, size);
	}
	else return;
}

int randomizedPartition(int arr[], int inferior, int superior, int size) {
	int pivot = random(inferior, superior);
	swap(&arr[pivot], &arr[superior]);

	return partition(arr, inferior, superior, size);
}

void swap(int* x, int* y) {
	int aux = *x;
	*x = *y;
	*y = aux;
}

void show(int heap[], int size) {
	for (int i = 1; i <= size; ++i)
		std::cout << heap[i] << " ";
	std::cout << "\n";
}

void createAverageGraph() {
	for (int m = 1; m <= 5; ++m)
	{
		for (int n = 100; n <= 10000; n += 100)
		{
			std::cout << "Step: " << m << ".    Size of array: " << n << "\n";

			int* initialArr = (int*)malloc((n + 1) * sizeof(int));
			int* copyArr = (int*)malloc((n + 1) * sizeof(int));

			if (NULL == copyArr  || NULL == initialArr)
			{
				std::cout << "No memory.\n";
				exit(1);
			}

			FillRandomArray(initialArr, n + 1);
			
			for (int i = 1; i <= n; ++i)
				copyArr[i] = initialArr[i];
			
			heapSort(initialArr, n);
			free(initialArr);
			quickSort(copyArr, 1, n, n);
			free(copyArr);
		}
	}

	profiler.divideValues("Heap-Sort Operations", 5);
	profiler.divideValues("Quick-Sort Operations", 5);

	profiler.createGroup("Average Case", "Heap-Sort Operations", "Quick-Sort Operations");
}

void createWorstGraph() {
	/*
		HeapSort WORST CASE occurs when the array that is used to create the Max Heap is sorted in ascending order.
		
		QuickSort WORST CASE occurs when the partition process always chooses as the pivot the smallest or the biggest element and the array
		is sorted in ascending or descending order. (in this case ascending order)
	*/


	for (int n = 100; n <= 10000; n += 100)
	{
		std::cout << "Size of array: " << n << "\n";

		int* initialArr = (int*)malloc((n + 1) * sizeof(int));
		int* copyArr = (int*)malloc((n + 1) * sizeof(int));

		if (NULL == initialArr || NULL == copyArr)
		{
			std::cout << "No memory.\n";
			exit(1);
		}

		FillRandomArray(initialArr, n + 1, 1, 100, false, 1);
		
		for (int i = 1; i <= n; ++i)
			copyArr[i] = initialArr[i];

		heapSort(initialArr, n);
		free(initialArr);
		quickSort(copyArr, 1, n, n);
		free(copyArr);
	}

	profiler.createGroup("Worst Case", "Heap-Sort Operations", "Quick-Sort Operations");
}

void createBestGraph() {
	/*
		HeapSort BEST CASE occurs when the array that is used to build the Max Heap is sorted in descending order.

		QuickSort BEST CASE occus when the partition process always chooses as the pivot the middle element.
	*/

	for (int n = 100; n <= 10000; n += 100)
	{
		std::cout << "Size of array: " << n << "\n";

		int* initialArr = (int*)malloc((n + 1) * sizeof(int));
		int* copyArr = (int*)malloc((n + 1) * sizeof(int));

		if (NULL == copyArr || NULL == initialArr)
		{
			std::cout << "No memory.\n";
			exit(1);
		}

		FillRandomArray(initialArr, n + 1, minInt, maxInt, false, 2);
		FillRandomArray(copyArr, n + 1, minInt, maxInt, false, 1);

		//for (int i = 1; i <= n; ++i)
		//	copyArr[i] = i;

		heapSort(initialArr, n);
		free(initialArr);
		quickSortBest(copyArr, 1, n, n);
		free(copyArr);
	}

	profiler.createGroup("Best Case", "Heap-Sort Operations", "Quick-Sort Operations");
}