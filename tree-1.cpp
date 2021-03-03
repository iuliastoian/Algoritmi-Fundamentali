#include "Profiler.h"
#include<iostream>

Profiler profiler;

struct node
{
	int key;
	int size;
	struct node* left, *right, *daddy;
};

node* OS_SELECT(node* root, int i, int n);
node* OS_BUILD(int arr[], int inferior, int superior);
node* OS_DELETE(node* root, int key, int n);

struct node* minValueNode(node* node);
void inorder(node* root);

void createAverageGraph() {
	for (int m = 0; m < 5; ++m) {
		std::cout << "Start generating graphs for m: " << m << '\n';

		for (int n = 100; n <= 10000; n += 100) {
			std::cout << "m: " << m << " and n: " << n << '\n';
			int* arr = (int*)malloc(n * sizeof(int));

			FillRandomArray(arr, n, 10, 15000, true, 1);

			node* root = OS_BUILD(arr, 0, n - 1);
			
			for (int i = n; i > 0; --i) {
				int random = rand() % n + 1;

				node* aux = OS_SELECT(root, random, n);

				if (NULL != aux) {
					aux = OS_DELETE(root, random, n);
				}
				
			}
			free(root);
			free(arr); 
		}

	}

	profiler.divideValues("Delete Assignments", 5);
	profiler.divideValues("Delete Comparisons", 5);
	profiler.divideValues("Select Assignments", 5);
	profiler.divideValues("Select Comparisons", 5);

	profiler.addSeries("Total Delete", "Delete Assignments", "Delete Comparisons");
	profiler.addSeries("Total Select", "Select Assignments", "Select Comparisons");

	profiler.createGroup("Assignments", "Delete Assignments", "Select Assignments");
	profiler.createGroup("Comparisons", "Delete Comparisons", "Select Comparisons");

	profiler.createGroup("Total", "Total Delete", "Total Comparisons");

	profiler.showReport();
}

void test() {
	int arr[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 };

	node* root = OS_BUILD(arr, 0, 10);
	inorder(root);

	//std::cout << "\n-sterge nodul cu cheia 50:\n";
	root = OS_DELETE(root, 60, 11);
	inorder(root);

	/*std::cout << "\n-alege al 3-lea cel mai mic nod:\n";
	root = OS_SELECT(root, 1, 11);
	//
	inorder(root); std::cout << root->key;*/

	free(root);
}

int main()
{
	test();
	//createAverageGraph();
	
	return 0;
}

node* OS_BUILD(int arr[], int inferior, int superior) {
	if (inferior > superior) {
		return NULL;
	}
	node* newNode = (node*)calloc(1, sizeof(node));

	newNode->key = arr[(inferior + superior) / 2];
	newNode->left = OS_BUILD(arr, inferior, (inferior + superior) / 2 - 1);
	newNode->right = OS_BUILD(arr, (inferior + superior) / 2 + 1, superior);

	if (newNode->left) {
		newNode->left->daddy = newNode;
		newNode->size += newNode->left->size + 1;
	}

	if (newNode->right) {
		newNode->right->daddy = newNode;
		newNode->size += newNode->right->size + 1;
	}
	return newNode;
}

node* OS_SELECT(node* root, int i, int n) {
	Operation assignments = profiler.createOperation("Select Assignments", n);
	Operation comparisons = profiler.createOperation("Select Comparisons", n);

	comparisons.count(1);
	if (!root) {
		return NULL;
	}

	assignments.count();
	int rang = (root->left != NULL) ? (root->left->size + 2) : (1);

	if (i == rang) {
		return root;
	}
	else {
		if (i < rang) {
			return OS_SELECT(root->left, i, n);
		}
		else {
			return OS_SELECT(root->right, i - rang, n);
		}
	}
}

node* OS_DELETE(node* root, int key, int n)
{
	Operation assignments = profiler.createOperation("Delete Assignments", n);
	Operation comparisons = profiler.createOperation("Delete Comparisons", n);

	comparisons.count(1);
	if (root == NULL) {
		return root;
	}

	comparisons.count(1);
	if (key < root->key) {
		assignments.count(2);
		root->size = root->size - 1;
		root->left = OS_DELETE(root->left, key, n);
	}
	else {
		comparisons.count(1);
		if (key > root->key) {
			assignments.count(2);
			root->size = root->size - 1;
			root->right = OS_DELETE(root->right, key, n);
		}
		else
		{
			comparisons.count(1);
			if (root->left == NULL)
			{
				assignments.count(1);
				struct node* temp = root->right;
				free(root);
				return temp;
			}
			else
			{
				comparisons.count(1);
				if (root->right == NULL)
				{
					assignments.count(1);
					struct node* temp = root->left;
					free(root);
					return temp;
				}
			}

			struct node* temp = minValueNode(root->right);

			assignments.count(2);
			root->key = temp->key;
			root->right = OS_DELETE(root->right, temp->key, n);
		}
	}
	return root;
}

struct node* minValueNode(struct node* node)
{
	struct node* current = node;

	while (current && current->left != NULL)
		current = current->left;

	return current;
}

void inorder(struct node* root)
{
	if (root != NULL)
	{
		inorder(root->left);
		std::cout << root->key << "/" << root->size << " ";
		inorder(root->right);
	}
}