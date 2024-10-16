//	CS 370
//	Show address of code, data and stack sections
//	as well as BSS and dynamic memory.

//	Compile:
//		gcc -g -Wall -o memAddr memAddr.c 


#include <stdio.h>
#include <malloc.h>			// for definition of ptrdiff_t
#include <unistd.h>
#include <alloca.h>			// for demonstration only

extern void	afunc(void);		// a function for showing stack growth

int	bss_var;			// will be in BSS
int	data_var = 42;			// init to nonzero, should be data
const	int	SIZE=1000000;



// ***********************************************************

int main(int argc, char **argv)
{
	int	bogusArr[1000000];

	int * bogusArrDynamic = (int *)malloc(SIZE * sizeof(int));

	void	*p, *b, *nb;

	for (int i=0; i < SIZE; i++)
		bogusArr[i] = 0;

	if (bogusArr[0] != 0)			// prevent warning msg
		printf("Array initilaization error.\n");


	printf("Text Locations:\n");
	printf("\tAddress of main:  %p\n", main);
	printf("\tAddress of afunc: %p\n", afunc);

	printf("Stack Locations:\n");
	afunc();

	p = (char *) alloca(32);
	if (p != NULL) {
		printf("\tStart of alloca()'ed array: %p\n", p);
		printf("\tEnd of alloca()'ed array: %p\n", (void*)((char*)p + 31));
	}

	printf("Data Locations:\n");
	printf("\tAddress of data_var: %p\n", &data_var);

	printf("BSS Locations:\n");
	printf("\tAddress of bss_var: %p\n", &bss_var);

	b = sbrk((ptrdiff_t) 32);		// grow address space
	nb = sbrk((ptrdiff_t) 0);

	printf("Heap Locations:\n");
	printf("\tInitial end of heap: %p\n", b);
	printf("\tNew end of heap: %p\n", nb);

	b = sbrk((ptrdiff_t) -16);		// shrink it
	nb = sbrk((ptrdiff_t) 0);
	printf("\tFinal end of heap: %p\n", nb);

	/* infinite loop */
	while (1)
		{ }
}

// **********************************************************************
//	Useless function.
//	Does nothing, but shows stack address

void afunc(void)
{
	static int	level = 0;		// recursion level
	int	stack_var;		// automatic variable, on stack

	if (++level == 6)			// avoid infinite recursion
	    return;

	printf("\tStack level %d: address of stack_var: %p\n",
		  level, &stack_var);
	afunc();				// recursive call
}

