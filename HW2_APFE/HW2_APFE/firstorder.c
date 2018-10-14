#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firstorder.h"



int main(int argc, char **argv)
{
  int retcode = 0;
  portfolio myportfolio;
  FILE *optimal_solution;
  double *opt;
  char buffer[100];
  double F_x = 0;

  if (argc != 2){
	  printf("usage: qp1 filename\n");  retcode = 1;
	  goto BACK;
  }

  char filename[] = "example.txt";

  retcode = readit(&filename, &myportfolio);
  
  myportfolio.x = (double *)calloc(myportfolio.n, sizeof(double));
  if(myportfolio.x==NULL){
    printf("no memory for x \n"); 
    retcode = 1;
    goto BACK;
  }

  retcode = algo(&myportfolio);
  //printf("Optimal solution portfolio combination is -- \n");
  //for (int i = 0; i < myportfolio.n; i++)
  //  printf("%f\n", myportfolio.x[i]);
  opt = (double *)calloc(myportfolio.n, sizeof(double));
  optimal_solution = fopen("opt_sol.csv", "r");
  if (!optimal_solution) {
    printf("can read optimal solution\n");
    goto BACK;
  }
  for (int j = 0 ; j < myportfolio.n ; j++){
    fscanf(optimal_solution,"%s", buffer);
    opt[j] = atof(buffer);
  }

  printf("Objective function value for optimal solution from solver is\n");
  for (int i = 0; i < myportfolio.n; i++){
      F_x -= myportfolio.mu[i]*opt[i];
      for (int j = 0; j <= i; j++){
          if (j < i)
              F_x += 2*myportfolio.lambda*myportfolio.covariance[i*myportfolio.n + j]*opt[i]*opt[j];
          if (j == i)
              F_x += myportfolio.lambda*myportfolio.covariance[i*myportfolio.n + j]*opt[i]*opt[j];
      }
  }
  printf("%f\n", F_x);
  BACK:
  return retcode;
}

