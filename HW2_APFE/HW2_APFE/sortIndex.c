#include <stdio.h>
#include <stdlib.h>

double *base_arr;
int compar(const void *a, const void *b);

int sortIndex(double *gradient, int *index, int n){
    base_arr = gradient;
    
    /** initialize the index array values **/
    for (int i = 0; i < n; i++){
        index[i] = i;
    }
    qsort(index, n, sizeof(int), compar);
    return 0;
}

int compar(const void *a, const void *b){
    int aa = *((int*)a), bb = *((int*)b);
    if (base_arr[aa] < base_arr[bb]) 
        return 1;
    if (base_arr[aa] == base_arr[bb]) 
        return 0;
    if (base_arr[aa] > base_arr[bb]) 
        return -1;

    else return 100;
}