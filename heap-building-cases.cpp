/*
		Note 1: Both buildBottomUp and buildTopDown will create max heaps;
		Note 2: Worst Case: when the array is ascending.

	Average Case:	The Top-Down Method of Building an Array is more efficient
					than the Bottom-Up Method (it does almost half the operations).

	Worst Case:		Quite the opposite from the situation on the Average Case, the
					Top-Down Method does almost twice the Operations of the Bottom-Up
					Method.

	Time Complexity of building a binary tree is O(n). 
*/

#include "Profiler.h"
#include <iostream>
#include <cmath>

using namespace std;

Profiler profiler;

#define maxInt (1 << 30)
#define minInt (1 << 31) * (-1)

inline int parent(int index) { return index >> 1; }
inline int leftChild(int index) { return index << 1; };
inline int rightChild(int index) { return (index << 1) + 1; }
inline double lgN(int n) { return log10(n); }
void testBuild(void (*build)(int*, int));
void buildBottomUp(int heap[], int size);
void heapifyDown(int heap[], int size, int index);
void buildTopDown(int heap[], int size);
void heapifyUp(int heap[], int index, int size);
void swap(int* x, int* y);
void show(int heap[], int size);
void heapSort(int heap[], int& heapSize);
void createAverageGraph();
void createWorstGraph();

int main() {
	testBuild(buildBottomUp);
	testBuild(buildTopDown);
	
	cout << "Start Generating Graphs:\n";

	//createAverageGraph();
	//profiler.reset();
	//createWorstGraph();
	//profiler.showReport();

	return 0;
}

void testBuild(void (*build)(int*, int)) {
	//int testArray[11] = { 0, 13, 40, 24, 18, 27, 9, 36, 69, 21, 15 };
	int testArrSize = 30;
	int* testArray = (int*)malloc(31 * sizeof(int));
	FillRandomArray(testArray, 31, 10, 100, false, 1);
	build(testArray, 30);
	//show(testArray, testArrSize);
	heapSort(testArray, testArrSize);
	show(testArray, 30);
}

void buildBottomUp(int heap[], int size) {

	for (int i = size / 2; i >= 1; --i)
	{
		//profiler.countOperation("Bottom-Up", size, (int)lgN(size));
		heapifyDown(heap, size, i);
	}
}

void heapifyDown(int heap[], int size, int index) {
	int key = index; //key stores the index of the greater number between the parent, leftChild and rightChild

	int left = leftChild(index);
	int right = rightChild(index);
	
	if (left <= size)
	{
		profiler.countOperation("Bottom-Up Operations", size, 1);
		if (heap[left] > heap[index]) //if leftChild exists and its value is bigger than its parent's
			key = left;
	}

	if (right <= size)
	{
		profiler.countOperation("Bottom-Up Operations", size, 1);
		if (heap[right] > heap[key]) //if rightChild exists and its value is bigger than max(parent, leftChild)
			key = right;
	}

	if (key != index) //if the values are not in correct place
	{
		profiler.countOperation("Bottom-Up Operations", size, 3);
		swap(&heap[index], &heap[key]);
		heapifyDown(heap, size, key); //heapify from the index of the Child we swapped values with
	}
}

void buildTopDown(int heap[], int size) {
	for (int heapSize = 2; heapSize <= size; ++heapSize)
	{
		//profiler.countOperation("Top-Down Operations", size, (int)lgN(size));
		heapifyUp(heap, heapSize, size);
	}
}

void heapifyUp(int heap[], int index, int size) {
	/* 
		The while loop was replaced with a do...while loop because if the input data would not have
		passed the initial condition, then it wouldn't have been counted by count(1).
		This way, every possible comparison is taken in account, without repeating them.
	*/

	if (index > 1)
	{
		bool satisfiedCondition = false;

		profiler.countOperation("Top-Down Operations", size, 1);
		if (heap[parent(index)] < heap[index])
			satisfiedCondition = true;
		
		if (satisfiedCondition == true)
		{
			do {
				profiler.countOperation("Top-Down Operations", size, 4); // count(1) for comparison + count(3) for exchange
				swap(heap[parent(index)], heap[index]);
				index = parent(index);
			} while (index > 1 && heap[parent(index)] < heap[index]);
		}
	}
}

void swap(int* x, int* y) {
	int aux = *x;
	*x = *y;
	*y = aux;
}

void show(int heap[], int size) {
	for (int i = 1; i <= size; ++i)
		cout << heap[i] << " ";
	cout << "\n";
}

void heapSort(int heap[], int& heapSize) {
	/*
		After we build a max heap, it is obvious that the maximum value will be stored in the root of the heap.
		The maximum value will be swappped with the value of the last element of the heap.
		The size of the heap will be decremented by 1, and then the function heapify() will be called to maintain
		the max heap property.
	*/

	for (int index = heapSize; index > 1; --index)
	{
		swap(heap[index], heap[1]);
		--heapSize;
		heapifyDown(heap, heapSize, 1);
	}
}

void createAverageGraph() {
	for (int m = 1; m <= 5; ++m)
	{
		for (int n = 100; n <= 10000; n += 100)
		{
			cout << "Step: " << m << ".    Size of array: " << n << "\n";

			int* initialArr = (int*)malloc((n + 1) * sizeof(int));
			int* copyArr = (int*)malloc((n + 1) * sizeof(int));

			if (NULL == initialArr || NULL == copyArr)
			{
				cout << "No memory.\n";
				exit(1);
			}

			FillRandomArray(initialArr, n + 1);
			memcpy(copyArr, initialArr, n + 1);

			buildBottomUp(initialArr, n);
			buildTopDown(copyArr, n);
			
			/* error: the program freezes when free() function is used
			free(initialArr);
			free(copyArr);*/
		}
	}

	profiler.divideValues("Bottom-Up Operations", 5);
	profiler.divideValues("Top-Down Operations", 5);

	profiler.createGroup("Average Case of Building a Heap", "Bottom-Up Operations", "Top-Down Operations");
}

void createWorstGraph() {
	for (int m = 1; m <= 5; ++m)
	{
		for (int n = 100; n <= 10000; n += 100)
		{
			cout << "Step: " << m << ".    Size of array: " << n << "\n";

			int* initialArr = (int*)malloc((n + 1) * sizeof(int));
			int* copyArr = (int*)malloc((n + 1) * sizeof(int));

			if (NULL == initialArr || NULL == copyArr)
			{
				cout << "No memory.\n";
				exit(1);
			}

			FillRandomArray(initialArr, n + 1, minInt, maxInt, false, 1);
			memcpy(copyArr, initialArr, n + 1);

			buildBottomUp(initialArr, n);
			buildTopDown(copyArr, n);

			/* error: the program freezes when free() function is used
			free(initialArr);
			free(copyArr);*/
		}
	}

	profiler.divideValues("Bottom-Up Operations", 5);
	profiler.divideValues("Top-Down Operations", 5);

	profiler.createGroup("Worst Case of Building a Heap", "Bottom-Up Operations", "Top-Down Operations");
}