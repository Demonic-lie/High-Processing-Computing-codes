// C program to store time taken by bubble sort,
// insertion sort and selection sort
// for sorting exactly same arrays.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"

struct Compare { int val; int index; };

// Custom reduction for finding the index of the max element.
//#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)
#pragma omp declare reduction(min : \
                              struct Compare : \
                              omp_out = omp_in.val < omp_out.val ? omp_in : omp_out)

// Swap utility
void swap(long int* a, long int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// Vanilla Selection sort
void VanillaSelection(long int arr[], long int n)
{
	long int i, j, midx;

	for (i = 0; i < n - 1; i++) {

		// Find the minimum element in unsorted array
		midx = i;

		for (j = i + 1; j < n; j++)
			if (arr[j] < arr[midx])
				midx = j;

		// Swap the found minimum element
		// with the first element
		swap(&arr[midx], &arr[i]);
	}
}

void ParallelSelection(long int A[],long int n)
{
	int startpos, i, tno;

	for(startpos =0; startpos < n; startpos++){
		// Declare the structure required for reduction
		struct Compare max;
        // Initialize the variables
        max.val = A[startpos];
        max.index = startpos;

        // Parallel for loop with custom reduction, at the end of the loop, max will have the max element and its index.
        #pragma omp parallel for reduction(min:max)
		for(i=startpos +1; i< n; ++i){
			if(A[i] < max.val){
				max.val = A[i];
				max.index = i;
			}
			//int id=omp_get_thread_num();
			//printf("ID: %d\n",id);
			tno=omp_get_num_threads();
		}

		swap(&A[startpos], &A[max.index]);
	}

	//printf("No. of threads: %d\n",tno);
}





// Driver code
int main()
{
	long int n = 5000;
	int it = 0;

	// Arrays to store time duration
	// of sorting algorithms
	double tim1, tim2;

	printf("A_size, Selection, ParallelSelection\n");

	// Performs 10 iterations
	while (it++ < 20) {
		long int a[n], b[n];
		int i;

		// generating n random numbers
		// storing them in arrays a, b, c
		for (i = 0; i < n; i++) {
			long int no = rand() % n + 1;
			a[i] = no;
			b[i] = no;
		}

		
		// using clock_t to store time
		clock_t start, end;


		//Vanilla Selection sort
		start = clock();
		VanillaSelection(a, n);
		end = clock();

		tim1 = ((double)(end - start))/CLOCKS_PER_SEC;
	
		
		//Parallel Selection sort
		start = clock();
		ParallelSelection(b, n);
		end = clock();

		tim2 = ((double)(end - start))/CLOCKS_PER_SEC;
		
		
		
		
		
		printf("%li,\t %lf,\t %lf\n",
			n,
			tim2,
			tim1);

		// increases the size of array by 1000
		n += 1000;
	}

	return 0;
}

