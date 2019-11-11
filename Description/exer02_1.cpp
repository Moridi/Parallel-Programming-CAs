#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define		N				1000000
#define		NO_THREADS		10

struct node{
	int data;
	struct node *next;
};

struct node *head;

void create_list ()
{
	struct node *newNode, *ptrNode;
	head = new node;
	head->data = 0;
	head->next = NULL;
	// Add N items to the list
	ptrNode = head;
	for (int i = 1; i < N; i++) {
		newNode = new node;
		newNode->data = i;
		newNode->next = NULL;
		ptrNode->next = newNode;
		ptrNode = ptrNode->next;
	}
}

void destroy_list ()
{
	struct node *ptrNode, *ptrHead;
	ptrHead = head;
	for ( ; ptrHead; ptrHead = ptrNode) {
		ptrNode = ptrHead->next;
		delete ptrHead;
	}
	head = NULL;
}

void show_list ()
{
	struct node *ptrNode;

	ptrNode = head;
	printf("the list contains:\n");
	for (; ptrNode; ptrNode = ptrNode->next)
		printf ("%d, ", ptrNode->data);
	printf ("\n\n");
}

void process_list_1 ()
{
	struct node *ptr = head;
	struct node **list_items;
	int num_elemnts = 0, i;
	double start, end;

	*list_items = (struct node *) malloc (sizeof(struct node *) * N);
	printf("\nrunning first version ...\n");
	start = omp_get_wtime();
	while (ptr) {
		*list_items [num_elemnts++] = *ptr;
		ptr = ptr->next;
	}

	#pragma omp parallel for num_threads (NO_THREADS)
		for (i = 0; i < num_elemnts; i++)
			*list_items[i]->data *= 2;
	end = omp_get_wtime();
	printf("first version run time = %f\n", (double)(end - start));
	free (*list_items);
}

void process_list_2 ()
{
	struct node *ptr;
	double start, end;

	printf("\nrunning second version ...\n");
	start = omp_get_wtime();
	#pragma omp parallel num_threads (NO_THREADS) private(ptr)
	{
		ptr = head;
		while (ptr) {
			#pragma omp single nowait
				ptr->data *= 2;
				ptr = ptr->next;
		}
	}
	end = omp_get_wtime();
	printf("second version run time = %f\n", (double)(end - start));
}

int main (int argc, char *argv[])
{

	create_list();
//	show_list();
	process_list_1();
	process_list_2();
//	show_list();
	destroy_list();

	return 0;
}
