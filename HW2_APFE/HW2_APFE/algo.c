#include <stdio.h>
#include <stdlib.h>
#include "firstorder.h"

int algo(portfolio *portfolio){
    int retcode = 0;
    printf("executing the algorithm\n");

    retcode = feasible(portfolio);
    if (retcode){
        goto BACK;
    }

    portfolio->num_iter = 100;
    portfolio->output = fopen("output.dat", "w");
    if (!portfolio->output){
        printf("couldn't open output file\n");
        retcode = 100;
        goto BACK;
    }
    
    portfolio->gradient = (double *)calloc(portfolio->n, sizeof(double));
    if(!portfolio->gradient){
        retcode = 200;
        goto BACK; 
    }
    retcode = improve(portfolio);

    BACK:
    return retcode;
}

