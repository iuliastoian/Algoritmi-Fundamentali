#include "Profiler.h"
#include <iostream>

using namespace std;

Profiler profiler("Best Case");

void showArr(int arr[], int arrSize);
void swap(int* value1, int* value2);
void insertionSort(int arr[], int arrSize);
void selectionSort(int arr[], int arrSize);
void bubbleSort(int arr[], int arrSize);
void testSort(int (*sort)(int *, int));
void createChart(int caseNo);

int main() {
	createChart(1);
	profiler.reset("Average Case");

	createChart(2);
	profiler.reset("Worst Case");

	createChart(3);

	profiler.showReport();
	return 0;
}

void showArr(int arr[], int arrSize){
	for (int i = 0; i < arrSize; ++i)
		cout << arr[i] << ' ';
	cout << '\n';
}

void swap(int* value1, int* value2) {
	int aux = *value1;
	*value1 = *value2;
	*value2 = aux;
}

void testSort(int (* sort)(int *, int)) {
	int arr[5] = { 2, 8, 3, 7, 1 };

	sort(arr, 5);
	showArr(arr, 5);
}

void insertionSort(int arr[], int arrSize) {
	Operation opComparisons = profiler.createOperation("Insertion_sort_comparisons", arrSize);
	Operation opAssignments = profiler.createOperation("Insertion_sort_assignments", arrSize);

	int aux, j;
	for (int i = 2; i < arrSize; ++i)
	{
		aux = arr[i];
		opAssignments.count(1);
		j = i - 1;
		opComparisons.count(1);
		while (j > 0 && arr[j] > aux)
		{
			arr[j + 1] = arr[j];
			opAssignments.count(1);
			--j;
		}
		if (arr[j + 1] != aux)
		{
			arr[j + 1] = aux;
			opAssignments.count(1);
		}
	}
}

void selectionSort(int arr[], int arrSize) {
	Operation opComparisons = profiler.createOperation("Selection_sort_comparisons", arrSize);
	Operation opAssignments = profiler.createOperation("Selection_sort_assignments", arrSize);

	int k;
	for (int i = 1; i < arrSize; ++i)
	{
		k = i;
		for (int j = i + 1; j <= arrSize; ++j)
		{
			opComparisons.count(1);
			if (arr[k] > arr[j])
				k = j;

			if (k != i)
			{
				swap(arr[k], arr[i]);
				opAssignments.count(3);
			}
		}
	}
}

void bubbleSort(int arr[], int arrSize)
{
	Operation opComparisons = profiler.createOperation("Bubble_sort_comparisons", arrSize);
	Operation opAssignments = profiler.createOperation("Bubble_sort_assignments", arrSize);

	bool sorted = false;

	while (!sorted)
	{
		sorted = true;
		for (int i = 1; i < arrSize; ++i)
		{
			opComparisons.count(1);
			if (arr[i] > arr[i + 1])
			{
				swap(arr[i], arr[i + 1]);
				opAssignments.count(3);
				sorted = false;
			}
		}
	}
}

void createChart(int caseNo) {
	int* x1 = NULL, * x2 = NULL, * x3 = NULL, *aux = NULL;

	for (int m = 0; m < 5; ++m)
	{
		for (int n = 1; n <= 10; n += 1)
		{
			cout << "Size of array: " << n << '\n';

			x1 = (int*)malloc(n * sizeof(int));
			x2 = (int*)malloc(n * sizeof(int));
			x3 = (int*)malloc(n * sizeof(int));

			if (NULL == x1 || NULL == x2 || NULL == x3)
			{
				cout << "No memory.\n";
				exit(0);
			}

			if (caseNo == 1)
			{
				FillRandomArray(x1, n, 1, 100, false, 1);
				x2 = x3 = x1;
				//showArr(x3, n);
			}

			if (caseNo == 2)
			{
				if (m == 0)
				{
					FillRandomArray(x1, n);
					aux = x2 = x3 = x1;
				}

				x1 = x2 = x3 = aux;
				//showArr(x3, n);
			}

			if (caseNo == 3)
			{
				FillRandomArray(x1, n, 1, 100, false, 2);
				x2 = x3 = x1;
			}

			bubbleSort(x1, n);
			insertionSort(x1, n);
			selectionSort(x3, n);
			

			// free(x1); free(x2); free(x3);
			//free(x); eroare???
		}
	}

	profiler.divideValues("Insertion_sort_operations", 5);
	profiler.divideValues("Selection_sort_operations", 5);
	profiler.divideValues("Bubble_sort_operations", 5);
	profiler.divideValues("Insertion_sort_comparisons", 5);
	profiler.divideValues("Selection_sort_comparisons", 5);
	profiler.divideValues("Bubble_sort_comparisons", 5);
	profiler.divideValues("Insertion_sort_assignments", 5);
	profiler.divideValues("Selection_sort_assignments", 5);
	profiler.divideValues("Bubble_sort_assignments", 5);

	profiler.addSeries("Insertion_sort_operations", "Insertion_sort_comparisons", "Insertion_sort_assignments");
	profiler.addSeries("Selection_sort_operations", "Selection_sort_comparisons", "Selection_sort_assignments");
	profiler.addSeries("Bubble_sort_operations", "Bubble_sort_comparisons", "Bubble_sort_assignments");

	profiler.createGroup("Operations", "Insertion_sort_operations", "Selection_sort_operations", "Bubble_sort_operations");
	profiler.createGroup("Comparisons", "Insertion_sort_comparisons", "Selection_sort_comparisons", "Bubble_sort_comparisons");
	profiler.createGroup("Assignments", "Insertion_sort_assignments", "Selection_sort_assignments", "Bubble_sort_assignments");
}
