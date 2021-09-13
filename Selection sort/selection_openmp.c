#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"

clock_t t, end;
double cpu_time_used;

// Structure for enabling reduction on the index of elements
struct Compare { int val; int index; };

// Custom reduction for finding the index of the max element.
//#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)
#pragma omp declare reduction(maximum : \
                              struct Compare : \
                              omp_out = omp_in.val > omp_out.val ? omp_in : omp_out) \
                              initializer(omp_priv=omp_orig)

void swap(int* a, int* b);
void selectionSort(int* A, int n);
void verify(int* A, int n);

int main(){

	int number, iter;

	printf("Enter no. of elements: ");
	scanf("%d", &number);

	int* Arr;
	Arr = (int *)malloc( number * sizeof(int));
	
	printf("\nEnter Elements: ");
	for(iter=0; iter<number; iter++){
		scanf("%d", &Arr[iter]);
	}
    
    t = clock();
	selectionSort(Arr, number);
    t = clock()-t;
	
	printf("\nSorted array: ");
	for(iter=0; iter<number;iter++){
		printf("%d ", Arr[iter]);
	}

	cpu_time_used = ((double)t)/CLOCKS_PER_SEC;
	
	// Verify if the algorithm works as advised
	verify(Arr, number);

	printf("\nTime taken for sort: %f sec.", cpu_time_used);
	return 0;

}



void selectionSort(int* A, int n){
	int startpos, i, tno;

	for(startpos =0; startpos < n; startpos++){
		// Declare the structure required for reduction
		struct Compare max;
        // Initialize the variables
        max.val = A[startpos];
        max.index = startpos;

        // Parallel for loop with custom reduction, at the end of the loop, max will have the max element and its index.
        #pragma omp parallel for reduction(maximum:max)
		for(i=startpos +1; i< n; ++i){
			if(A[i] > max.val){
				max.val = A[i];
				max.index = i;
			}
			//int id=omp_get_thread_num();
			//printf("ID: %d\n",id);
			tno=omp_get_num_threads();
		}

		swap(&A[startpos], &A[max.index]);
	}

	printf("No. of threads: %d\n",tno);
}

// Verification function
void verify(int* A, int n){
	int failcount = 0, iter;
	for(iter = 0; iter < n-1; iter++){
		if(A[iter] < A[iter+1]){
			failcount++;
		}
	}
	printf("\nFail count: %d\n", failcount);
}

//Swap function
void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

