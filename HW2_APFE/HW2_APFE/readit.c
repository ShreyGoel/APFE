#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firstorder.h"

int readit(char *filename, portfolio *portfolio)
{
	int readcode = 0, fscancode;
	FILE *datafile = NULL;
	char buffer[100];
	int n, i, j;
	double *lb = NULL, *ub = NULL, *mu = NULL, *covariance = NULL;

	datafile = fopen(filename, "r");
	if (!datafile){
		printf("cannot open file %s\n", filename);
		readcode = 2;  goto BACK;
	}

	printf("reading data file %s\n", filename);

	fscanf(datafile, "%s", buffer);
	fscancode = fscanf(datafile, "%s", buffer);
	if (fscancode == EOF){
		printf("problem: premature file end at ...\n");
		readcode = 4; goto BACK;
	}

	n = portfolio->n = atoi(buffer);

	printf("n = %d\n", n);

	lb = (double *)calloc(n, sizeof(double));
	portfolio->lb = lb;
	ub = (double *)calloc(n, sizeof(double));
	portfolio->ub = ub;
	mu = (double *)calloc(n, sizeof(double));
	portfolio->mu = mu;
	covariance = (double *)calloc(n*n, sizeof(double));
	portfolio->covariance = covariance;

	if (!lb || !ub || !mu || !covariance){
		printf("not enough memory for lb ub mu covariance\n"); readcode = 3; goto BACK;
	}

	fscanf(datafile, "%s", buffer);

	for (j = 0; j < n; j++){
		fscanf(datafile, "%s", buffer);
		fscanf(datafile, "%s", buffer);
		lb[j] = atof(buffer);
		fscanf(datafile, "%s", buffer);
		ub[j] = atof(buffer);
		fscanf(datafile, "%s", buffer);
		mu[j] = atof(buffer);
		printf("j = %d lb = %g ub = %g mu = %g\n", j, lb[j], ub[j], mu[j]);
	}


	fscanf(datafile, "%s", buffer);
	fscanf(datafile, "%s", buffer);
    portfolio->lambda = atof(buffer);
    printf("Value of lambda is %f\n", portfolio->lambda);


	fscanf(datafile, "%s", buffer); /* reading 'covariance'*/

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){ 
			fscanf(datafile, "%s", buffer);
			covariance[i*n + j] = atof(buffer);
		}
	}


	fscanf(datafile, "%s", buffer);
	if (strcmp(buffer, "END") != 0){
		printf("possible error in data file: 'END' missing\n");
	}


	fclose(datafile);

BACK:

	return readcode;
}
