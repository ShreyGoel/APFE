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

  retcode = readit(argv[1], &myportfolio);
  
  myportfolio.x = (double *)calloc(myportfolio.n, sizeof(double));
  if(myportfolio.x==NULL){
    printf("no memory for x \n"); 
    retcode = 1;
    goto BACK;
  }

  retcode = algo(&myportfolio);

  printf("%f\n", F_x);
  BACK:
  return retcode;
}

