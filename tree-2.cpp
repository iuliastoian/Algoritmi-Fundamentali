#include <iostream>

#define N 11

struct node {
	int key;
	int child[N];
	int no_children = 0;
	node* next;
};

struct binary_node {
	int key;
	binary_node* left_child;
	binary_node* right_brother;
};

void showList(node* list) {
	while (NULL != list) {
		std::cout << list->key << " ";
		list = list->next;
	}
	std::cout << "\n";
}

node* addToBackNode(node*& p, int value)
{
	if (NULL == p)
	{
		// primul nod nu se declara inca o data, doar se aloca memorie
		p = new node;
		p->key = value;
		p->next = NULL;

		return p;
	}
	else
	{
		node* r = p;

		for (; r->next; r = r->next);

		node* q = new node;
		q->key = value;
		q->next = NULL;

		// se facem legatura cu nodul precedent
		r->next = q;

		return q;
	}
}

void showArray(int arr[], int n) {
	std::cout << "\nparents:  ";
	for (int i = 0; i < n; ++i) {
		if (i > 9) {
			std::cout << " ";
		}
		else;
		std::cout << arr[i] << "  ";
	}

	std::cout << "\nchildren: ";
	for (int i = 0; i < n; ++i) {
		if (-1 == arr[i]) {
			std::cout << " ";
		}
		else;
		std::cout << i << "  ";
	}
	std::cout << "\n";
}

void swap(int* a, int* b) {
	int aux = *a;
	*a = *b;
	*b = aux;
}

void prettyPrint(int[], int, int);
void T1(int daddies[], node*& first_node);
void T2(node* multi_node, binary_node*& first_binary);

int daddies[N] = { 6, 4, -1, 7, 9, 2, 7, 2, 4, 5, 6 };

int main() {
	std::cout << "The input parent array is:\n";
	showArray(daddies, N);

	std::cout << "\nR1. Show parent array representation...\n";
	prettyPrint(daddies, 0, -1);

	node* first_node = new node;
	first_node = NULL;

	std::cout << "T1. Start creating multi-way tree from parent array...\n";
	T1(daddies, first_node);
	showList(first_node);

	std::cout << "\nR2. Show multi-way tree representation...\n";
	prettyPrint(daddies, 0, -1);

	/*while (first_node) {
		std::cout << first_node->key << " : ";
		for (int i = 0; i < first_node->no_children; ++i) {
			std::cout << first_node->child[i] << " ";
		}
		first_node = first_node->next;

		std::cout << "\n";
	}*/


	binary_node* first_binary = new binary_node;
	first_binary = NULL;

	std::cout << "T2. Start creating binary tree from multi-way tree...\n";
	T2(first_node, first_binary);

	std::cout << "\nProgram terminated.\n";

	return 0;
}

void prettyPrint(int daddies[], int ident, int daddy) {
	std::cout << "\n";
		for (int i = 0; i < N; ++i) {
			if (daddies[i] == daddy) {
				for (int i = 0; i < ident; i++) {
					std::cout << "  ";
				}
				std::cout << i << "\n\n";
				prettyPrint(daddies, ident + 2, i);
			}
		}
}

void T1(int daddies[], node*& first_node) {
	short int mark[100] = { 0 };

	for (int i = 0; i < N; ++i) {
		if (0 == mark[daddies[i] + 1]  && -1 != daddies[i]) {
			node* current_node = addToBackNode(first_node, daddies[i]); //adaug
			mark[daddies[i] + 1] = 1; //marchez

			std::cout << "\n" << daddies[i] << " has children: ";
			for (int j = 0; j < N; ++j) {
				if (daddies[i] == daddies[j]) {
					current_node->child[current_node->no_children++] = j;
					std::cout << j << " ";
				}
			}
			std::cout << " so a total of: " << current_node->no_children << " children\n";
		}
		else {
			continue;
		}
	}
	std::cout << "\nThe list of parents is: ";
}

void T2(node* multi_node, binary_node*& first_binary) {
	for (int i = 0; i < N; ++i) {
		if (daddies[i] == -1) {
			first_binary = new binary_node;
			first_binary->key = i;
			first_binary->left_child = NULL;
			first_binary->right_brother = NULL;
			break;
		}
	}

	binary_node* current_binary = first_binary;

	node* p = multi_node;

	while (p->key != current_binary->key) {
		p = p->next;
	}

	short int mark[100] = { 0 };
	mark[current_binary->key + 1] = 1; // inutil pentru ca 2 nu este copilul nimanui
	
	bool done = 1;

	while (done == 1) {
		if (p->no_children > 0) {
			done = 0;
			for (int i = 0; i < p->no_children; ++i) {
				if (mark[p->child[i] + 1] == 0) {

					binary_node* aux = new binary_node;
					aux->key = p->child[i];
					aux->left_child = NULL;
					aux->right_brother = NULL;

					current_binary->left_child = aux;
					current_binary->right_brother = NULL;
					current_binary = current_binary->left_child;

					mark[p->child[i] + 1] = 1;
					done = 1;

					break;
				}
			}

			node* search = multi_node;
			int skey = current_binary->key;

			while (search->next != NULL) {
				if (search->key == skey)
					break;
				search = search->next;
			}

			p = search;

		}
		else {
			break;
		}
	}

	binary_node* rep = first_binary;

	while (rep != NULL) {
		std::cout << rep->key << " ";
		rep = rep->left_child;
	}
	
	done = 1;
	current_binary = first_binary;
	p = multi_node;
	node* plus = multi_node;

	while (done == 1)
	{
		if (p->no_children > 0) {
			done = 0;
			for (int i = 0; i < p->no_children; ++i) {
				if (mark[p->child[i] + 1] == 0) {

					binary_node* aux = new binary_node;
					aux->key = p->child[i];
					aux->left_child = NULL;
					aux->right_brother = NULL;

					current_binary->left_child = NULL;
					current_binary->right_brother = aux;
					current_binary = current_binary->right_brother;

					mark[p->child[i] + 1] = 1;
					done = 1;

					break;
				}
			}

			node* search = multi_node;
			int skey = current_binary->key;

			while (search->next != NULL) {
				if (search->key == skey)
					break;
				search = search->next;
			}

			p = search;
		}
		else {
			break;
		}
	}

	std::cout << '\n';
	rep = first_binary;

	while (rep != NULL) {
		std::cout << rep->key << " ";
		if (rep->right_brother == NULL)
			rep = rep->left_child;
		rep = rep->right_brother;
	}

}