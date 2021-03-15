#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *sorter(void *params);
void *merger(void *params);

#define unsortedSize 10

int unsorted[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int sorted[unsortedSize];

typedef struct arrStruct{
    int *half1;
    int *half2;
};

int main(int argc, const char *argv[]) {

    pthread_t firstHalf; //will house first half of unsorted array
    pthread_t secondHalf; //will house second half of unsorted array
    pthread_t mergeThreads; //will house the two threads merged once they are sorted

    struct arrStruct arr; //split the array into two halfs
    //allocate memory to both halfs
    arr.half1 = (int *)malloc((unsortedSize/2) * sizeof(int));
    arr.half2 = (int *)malloc((unsortedSize/2) * sizeof(int));

    //split the two halfs
    int i;
    for (i = 0; i < unsortedSize / 2; i++){
        arr.half1[i] = unsorted[i];
    }

    int j;
    for (j = 0; j < unsortedSize / 2; j++){
        arr.half2[j] = unsorted[j + 5];
    }

    //create threads for the first and second half of the array using sorter method
    pthread_create(&(firstHalf), NULL, &sorter, (void*) arr.half1);
    pthread_create(&(secondHalf), NULL, &sorter, (void*) arr.half2);

    //join first and second half threads
    pthread_join(firstHalf, NULL);
    pthread_join(secondHalf, NULL);

    //create thread for merged first and second half using merger method
    pthread_create(&(mergeThreads), NULL, &merger, (void*) &arr);

    //join merged thread
    pthread_join(mergeThreads, NULL);

    //print unsorted, first half sorted, second half sorted, and finally sorted array
    printf("UNSORTED ARRAY: ");
    int k;
    for(k = 0; k < unsortedSize; k++){
        printf("%d ", unsorted[k]);
    }
    printf("\n");

    printf("FIRST HALF SORTED: ");
    int l;
    for(l = 0; l < unsortedSize / 2; l++){
        printf("%d ", arr.half1[l]);
    }
    printf("\n");

    printf("SECOND HALF SORTED: ");
    int x;
    for(x = 0; x < unsortedSize / 2; x++){
        printf("%d ", arr.half2[x]);
    }
    printf("\n");

    printf("SORTED ARRAY: ");
    int y;
    for(y = 0; y < unsortedSize; y++){
        printf("%d ", sorted[y]);
    }
    printf("\n");



    return 0;
}

/**
 * Sorting thread.
 */

int cmpfunc (const void *a, const void *b){
    return ( *(int*)a - *(int*)b);
}
void *sorter(void *params){
    qsort((void*)params, unsortedSize/2, sizeof(int), cmpfunc);
}

/**
 * Merge thread
 */
void *merger(void *params){
    struct arrStruct *arr = params;
    int i = 0;
    int j = 0;
    int k = 0;

    //if element in first half of array is smaller, then put into sorted array first. if not, then element from second array goes in sorted array
    while (i < unsortedSize / 2 && j < unsortedSize / 2){
        if(arr->half1[i] < arr->half2[j]){
            sorted[k++] = arr->half1[i++];
        } else {
            sorted[k++] = arr->half2[j++];
        }
    }

    //last two conditions to store the remaining elements in halfs (if any) into the sorted array
    while (i < unsortedSize / 2){
        sorted[k++] = arr->half1[i++];
    }

    while (j < unsortedSize / 2){
        sorted[k++] = arr->half2[j++];
    }
}
