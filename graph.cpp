#include <iostream>
#include <cstdlib>
#include "Profiler.h"

#define N 10

Profiler profiler;

struct edge {
	int vertix1, vertix2;
	int cost;
};

int* rank;
int* parent;

void MAKE_SET(int value, int size);
void UNION(int value1, int value2, int size);
int FIND_SET(int value, int size);

void displayNodes() {
	for (int i = 1; i <= N; ++i) {
		printf("the parent of node %d is %d and its rank is %d\n", i, parent[i], rank[i]);
	}
	printf("\n");
}

void demo() {
	rank = (int*)calloc(N, sizeof(int));
	parent = (int*)calloc(N, sizeof(int));

	for (int i = 1; i <= N; ++i) {
		MAKE_SET(i, N);
	}

	printf("Before UNION...\n");
	displayNodes();
	
	for (int i = 1; i < 10; i += 2) {
		printf("After UNION(%d, %d)...\n",i, i + 1);
		UNION(i, i + 1, N);
		printf("the parent of node %d is %d and its rank is %d\n", i, parent[i], rank[i]);
		printf("the parent of node %d is %d and its rank is %d\n", i + 1, parent[i + 1], rank[i + 1]);
		printf("\n");
	}

	for (int i = 2; i < 10; i += 4) {
		printf("After UNION(%d, %d)...\n", i, i + 2);
		UNION(i, i + 2, N);
		printf("the parent of node %d is %d and its rank is %d\n", i, parent[i], rank[i]);
		printf("the parent of node %d is %d and its rank is %d\n", i + 2, parent[i + 2], rank[i + 2]);
		printf("\n");
	}

	displayNodes();

	UNION(4, 8, N);
	UNION(8, 10, N);


	for (int i = 1; i <= 10; ++i) {
		printf("the representant of set of %d is %d\n", i, FIND_SET(i, N));
	}

	free(rank);
	free(parent);
}

void swapEdges(edge* e1, edge* e2) {
	edge aux;
	aux.cost = e1->cost;
	aux.vertix1 = e1->vertix1;
	aux.vertix2 = e1->vertix2;

	*e1 = *e2;
	*e2 = aux;
}

void quickEdges(int inf, int sup, edge* list)
{
	int pivot, i, j;

	i = inf;
	j = sup;
	pivot = list[(i + j) / 2].cost;

	do {

		while ((i < sup) && (list[i].cost < pivot)) ++i;
		while ((j > inf) && (list[j].cost > pivot)) --j;

		if (i <= j)
		{
			swapEdges(&list[i], &list[j]);
			++i;
			--j;
		}

	} while (i <= j);

	if (inf < j) quickEdges(inf, j, list);
	if (i < sup) quickEdges(i, sup, list);
}

void createGraph() {
	for (int n = 100; n <= 10000; n += 100) {
		rank = (int*)malloc((n + 1) * sizeof(int));
		parent = (int*)malloc((n + 1) * sizeof(int));

		// make set for every node
		
		for (int i = 1; i <= n; ++i) {
			MAKE_SET(i, n);
		}

		// generating edges so that the graph is connected

		int no_edges = 0; // the maximum number of edges that an undirected connected graph can have is n * (n - 1) / 2

		edge* list = (edge*)malloc((4 * n + 1)* sizeof(edge));

		for (int i = 1; i < n; ++i) {
			list[no_edges].vertix1 = i;
			list[no_edges].vertix2 = i + 1;
			list[no_edges].cost = rand() % 10 + 1;
			++no_edges;
		} // am n - 1 muchii cu care am facut graful sa fie conex
		
		for (int i = 1; i <= 3 * n + 1; ++i) {
			list[no_edges].vertix1 = rand() % n + 1;
		vertix: list[no_edges].vertix2 = rand() % n + 1;
			if (list[no_edges].vertix2 == list[no_edges].vertix1) {
				goto vertix;
			}
			list[no_edges].cost = rand() % 10 + 1;
			++no_edges;
		} // creez inca 3* n + 1 muchii pt a avea 4 * n muchii in total

		// sorting edges
		
		quickEdges(0, no_edges - 1, list);
		
		//

		long long int min_cost = 0;
		
		for (int i = 0; i < no_edges; ++i) {
			int set1 = FIND_SET(list[i].vertix1, n);
			int set2 = FIND_SET(list[i].vertix2, n);

			if (set1 != set2) {
				min_cost += list[i].cost;
				UNION(list[i].vertix1, list[i].vertix2, n);
			}
			else {
				continue;
			}
		}
	
		printf("%d\n", no_edges);

		free(rank);
		free(parent);
		free(list);
	}

	profiler.createGroup("Total", "Operations");
	profiler.showReport();
}

void kruskal() {
	rank = (int*)calloc(10, sizeof(int));
	parent = (int*)calloc(10, sizeof(int));

	for (int i = 1; i <= 9; ++i) {
		MAKE_SET(i, 9);
	}

	edge* list = (edge*)malloc(15 * sizeof(edge));

	for (int i = 1; i <= 8; ++i) {
		list[i - 1].vertix1 = i;
		list[i - 1].vertix2 = i + 1;
	}

	list[0].cost = 4;
	list[1].cost = 8;
	list[2].cost = 7;
	list[3].cost = 9;
	list[4].cost = 10;
	list[5].cost = 2;
	list[6].cost = 1;
	list[7].cost = 7;

	list[8].vertix1 = 8;
	list[8].vertix2 = 1;
	list[8].cost = 8;

	list[9].vertix1 = 2;
	list[9].vertix2 = 8;
	list[9].cost = 11;

	list[10].vertix1 = 3;
	list[10].vertix2 = 9;
	list[10].cost = 2;

	list[11].vertix1 = 7;
	list[11].vertix2 = 9;
	list[11].cost = 6;

	list[12].vertix1 = 3;
	list[12].vertix2 = 6;
	list[12].cost = 4;

	list[13].vertix1 = 4;
	list[13].vertix2 = 6;
	list[13].cost = 14;

	quickEdges(0, 13, list);

	long long int min_cost = 0;

	int no_edges = 0;
	edge* final = (edge*)malloc(14 * sizeof(int));

	for (int i = 0; i <= 13; ++i) {
		int set1 = FIND_SET(list[i].vertix1, 9);
		int set2 = FIND_SET(list[i].vertix2, 9);

		if (set1 != set2) {
			min_cost += list[i].cost;
			final[no_edges].vertix1 = list[i].vertix1;
			final[no_edges].vertix2 = list[i].vertix2;
			final[no_edges].cost = list[i].cost;
			++no_edges;
			UNION(list[i].vertix1, list[i].vertix2, 9);
		}
		else {
			continue;
		}
	}

	
	for (int i = 0; i < no_edges; ++i) {
		printf("muchia: %d - %d cost: %d\n", final[i].vertix1, final[i].vertix2, final[i].cost);
	}

	printf("min cost: %d", min_cost);

	free(rank);
	free(parent);
	free(list);
	free(final);
}

int main() {
	demo();

	//kruskal();

	//createGraph();

	return 0;
}

// O(1)

void MAKE_SET(int value, int size) {
	Operation op = profiler.createOperation("Operations", size);

	op.count(2);
	rank[value] = 0;
	parent[value] = value;
}

/*
	UNION - by rank and path compression heuristic runs in time O(m*a(n)) for m disjoint set operations on n elements.
	a(n) is a function that grows very slowly
*/

void UNION(int value1, int value2, int size) {
	Operation op = profiler.createOperation("Operations", size);

	/*
		the representative node is the greater one
	*/

	op.count(2);
	value1 = FIND_SET(value1, size);
	value2 = FIND_SET(value2, size);

	op.count(1);
	if (rank[value1] > rank[value2]) {
		op.count(1);
		parent[value2] = value1;
	}
	else {
		op.count(1);
		parent[value1] = value2;

		op.count(1);
		if (rank[value1] == rank[value2]) {
			op.count(1);
			++rank[value2];
		}
	}
}

//O(1)

int FIND_SET(int value, int size) {
	Operation op = profiler.createOperation("Operations", size);

	op.count(1);
	if (value == parent[value]) {
		return value;
	}
	else {
		return (parent[value] = FIND_SET(parent[value], size));
	}
}

/*
	MAKE-SET and FIND-SET both take O(1) time, each is done O(m) times => O(m + nlogn)
*/