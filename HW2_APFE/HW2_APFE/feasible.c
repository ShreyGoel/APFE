#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "firstorder.h"

int feasible(portfolio *portfolio){
    int retcode = 0;
    double sum, increase, random;
    srand(time(0));

    printf("computing a feasibile x vector\n");
    sum = 0;
    
    /*set variables to lower bounds*/
    for (int j = 0; j < portfolio->n; j++){
        portfolio->x[j] = portfolio->lb[j];
        sum += portfolio->lb[j];
    }

    for (int j = 0; j < portfolio->n; j++){
        increase = 1.0 - sum;
        random = ((double)rand()/RAND_MAX);
        if ((increase > (portfolio->ub[j] - portfolio->x[j])) && (j < portfolio->n - 1))
            increase = (portfolio->ub[j] - portfolio->x[j]);
        //if ((increase > portfolio->ub[j] - portfolio->x[j]) && (j == portfolio->n - 1))
        //    increase = portfolio->ub[j] - portfolio->x[j];
        portfolio->x[j] += increase;
        sum += increase;
        if (sum >= 1.0) break;
    }
    /** feasible solution debug prints
    printf("first feasible solution is\n");
    for (int j = 0; j < portfolio->n; j++)
        printf("%f\n", portfolio->x[j]);
    **/
    BACK:
    return retcode;
}