#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "importantHeaders.h"


int main(int argc, char **argv)
{
  int retcode = 0;
  dataStruct mydata;
  FILE *optimal_solution;
  FILE *inputName = "example.txt";
  double *opt;
  char buffer[100];
  double F_x = 0;



  /*
  if (argc != 2){
	  printf("usage: qp1 filename\n");  retcode = 1;
	  goto BACK;
  }*/

  retcode = getData(inputName, &mydata);
  
  mydata.x = (double *)calloc(mydata.n, sizeof(double));
  if(mydata.x==NULL){
    printf("no memory for x \n"); 
    retcode = 1;
    goto BACK;
  }

  retcode = algo(&mydata);

  printf("%f\n", F_x);
  BACK:
  return retcode;
}

int algo(dataStruct *data) {
	int retcode = 0;
	printf("executing the algorithm\n");

	retcode = feasible(data);
	if (retcode) {
		goto BACK;
	}

	data->num_iter = 100;
	data->output = fopen("output.dat", "w");
	if (!data->output) {
		printf("couldn't open output file\n");
		retcode = 100;
		goto BACK;
	}

	data->gradient = (double *)calloc(data->n, sizeof(double));
	if (!data->gradient) {
		retcode = 200;
		goto BACK;
	}
	retcode = improve(data);

BACK:
	return retcode;
}

int feasible(dataStruct *data) {
	int retcode = 0;
	double sum, increase, random;
	//srand(time(0));

	printf("computing a feasibile x vector\n");
	sum = 0;

	/*set variables to lower bounds*/
	for (int j = 0; j < data->n; j++) {
		data->x[j] = data->lb[j];
		sum += data->lb[j];
	}

	for (int j = 0; j < data->n; j++) {
		increase = 1.0 - sum;
		random = ((double)rand() / RAND_MAX);
		if ((increase > (data->ub[j] - data->x[j])) && (j < data->n - 1))
			increase = (data->ub[j] - data->x[j]);
		//if ((increase > data->ub[j] - data->x[j]) && (j == data->n - 1))
		//    increase = data->ub[j] - data->x[j];
		data->x[j] += increase;
		sum += increase;
		if (sum >= 1.0) break;
	}
	/** feasible solution debug prints
	printf("first feasible solution is\n");
	for (int j = 0; j < data->n; j++)
		printf("%f\n", data->x[j]);
	**/
BACK:
	return retcode;
}

int getData(char *filename, dataStruct *data)
{
	int readcode = 0, fscancode;
	FILE *datafile = NULL;
	char buffer[100];
	int n, i, j;
	double *lb = NULL, *ub = NULL, *mu = NULL, *covariance = NULL;

	datafile = fopen(filename, "r");
	if (!datafile) {
		printf("cannot open file %s\n", filename);
		readcode = 2;  goto BACK;
	}

	printf("reading data file %s\n", filename);

	fscanf(datafile, "%s", buffer);
	fscancode = fscanf(datafile, "%s", buffer);
	if (fscancode == EOF) {
		printf("problem: premature file end at ...\n");
		readcode = 4; goto BACK;
	}

	n = data->n = atoi(buffer);

	printf("n = %d\n", n);

	lb = (double *)calloc(n, sizeof(double));
	data->lb = lb;
	ub = (double *)calloc(n, sizeof(double));
	data->ub = ub;
	mu = (double *)calloc(n, sizeof(double));
	data->mu = mu;
	covariance = (double *)calloc(n*n, sizeof(double));
	data->covariance = covariance;

	if (!lb || !ub || !mu || !covariance) {
		printf("not enough memory for lb ub mu covariance\n"); readcode = 3; goto BACK;
	}

	fscanf(datafile, "%s", buffer);

	for (j = 0; j < n; j++) {
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
	data->lambda = atof(buffer);
	printf("Value of lambda is %f\n", data->lambda);


	fscanf(datafile, "%s", buffer); /* reading 'covariance'*/

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			fscanf(datafile, "%s", buffer);
			covariance[i*n + j] = atof(buffer);
		}
	}


	fscanf(datafile, "%s", buffer);
	if (strcmp(buffer, "END") != 0) {
		printf("possible error in data file: 'END' missing\n");
	}


	fclose(datafile);

BACK:

	return readcode;
}
