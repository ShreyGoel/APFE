
#include <stdio.h>
#include <stdlib.h>


#include <gurobi_c++.h>

int main(void)
{
  int retcode = 0;
  GRBenv   *env = NULL;
  GRBmodel *model = NULL;
  int n, j;
  double *obj      = NULL;
  double *lb       = NULL;
  double *ub       = NULL;
  double *x;
  int *qrow, *qcol, Nq;
  double *qval;
  int *cind;
  double rhs;
  char sense;
  double *cval;
  int numnonz;

  const char *mynames[] = { "x0", "x1", "x2", "x3" };


  n = 4; /*bad practice but it's only an example */


  retcode = GRBloadenv(&env, "second.log");
  if (retcode) goto BACK;

 /* Create initial model */
  retcode = GRBnewmodel(env, &model, "second", 4, /* four variables */
                      NULL, NULL, NULL, NULL, NULL);
  if (retcode) goto BACK;

  /** next we create the 4 columns **/
  obj     = (double *) calloc (n, sizeof(double));
  ub     = (double *) calloc (n, sizeof(double));
  lb     = (double *) calloc (n, sizeof(double));
  x     = (double *) calloc (n, sizeof(double));

  obj[0] = 2.0; obj[1] = 5.0; obj[2] = 7; obj[3] = 3.5;
  ub[0] = 23.0; ub[1] = 0.9; ub[2] = 10.9; ub[3] = 10.0;
  lb[0] = 0; lb[1] = 0.4; lb[2] = 0; lb[3] = 0;


  /* initialize variables */
  for(j = 0; j < n; j++){
    retcode = GRBsetstrattrelement(model, "VarName", j, mynames[j]);
    if (retcode) goto BACK;

    retcode = GRBsetdblattrelement(model, "Obj", j, obj[j]);
    if (retcode) goto BACK;

    retcode = GRBsetdblattrelement(model, "LB", j, lb[j]);
    if (retcode) goto BACK;

    retcode = GRBsetdblattrelement(model, "UB", j, ub[j]);
    if (retcode) goto BACK;
  }

  /** next the quadratic objective **/
  Nq = 4;  /** 4 nonzeros in quadratic matrix **/
  qrow = (int *) calloc(Nq, sizeof(int));  /** row indices **/
  qcol = (int *) calloc(Nq, sizeof(int));  /** column indices **/
  qval = (double *) calloc(Nq, sizeof(double));  /** values **/

  qrow[0] = 2;   qrow[1] = 2;   qrow[2] = 3;    qrow[3] = 3; 
  qcol[0] = 2;   qcol[1] = 3;   qcol[2] = 3;    qcol[3] = 2; 
  qval[0] = 5.0; qval[1] = 0.4; qval[2] = 10.0; qval[3] = 0.4;

  retcode = GRBaddqpterms(model, Nq, qrow, qcol, qval);
  if (retcode) goto BACK;



  /** now we will add one constraint at a time **/
  /** we need to have a couple of auxiliary arrays **/

  cind = (int *)calloc(n, sizeof(int));
  cval= (double *)calloc(n, sizeof(double));


  /** first constraint is: x0 -4 x1 + x2 >= 10 **/
  cind[0] = 0; cind[1] = 1; cind[2] = 2;
  cval[0] = 1.0; cval[1] = -4; cval[2] = 1.0;

  numnonz = 3;
  rhs = 10;
  sense = GRB_GREATER_EQUAL;

  retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "first_constraint");
  if (retcode) goto BACK;

  /** second constraint: 2 x1 + 7 x2 -5 x3 = 8 **/

  cind[0] = 1; cind[1] = 2; cind[2] = 3;
  cval[0] = 2.0; cval[1] = 7.0; cval[2] = -5.0;

  numnonz = 3;
  rhs = 8;
  sense = GRB_EQUAL;

  retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "second_constraint");
  if (retcode) goto BACK;


  /** third constraint: 2 x0 + 9 x1 - 9 x2 + 5 x3 <= 17 **/

  cind[0] = 0; cind[1] = 1; cind[2] = 2; cind[3] = 3;
  cval[0] = 2.0;  cval[1] = 9.0; cval[2] = -9.0; cval[3] = 5.0;

  numnonz = 4;
  rhs = 17;
  sense = GRB_LESS_EQUAL;

  retcode = GRBaddconstr(model, numnonz, cind, cval, sense, rhs, "third_constraint");
  if (retcode) goto BACK;


  retcode = GRBupdatemodel(model);
  if (retcode) goto BACK;


  /** optional: write the problem **/

  retcode = GRBwrite(model, "mysecond.lp");
  if (retcode) goto BACK;


  retcode = GRBoptimize(model);
  if (retcode) goto BACK;


  /** get solution **/


  retcode = GRBgetdblattrarray(model,
                               GRB_DBL_ATTR_X, 0, n,
                               x);
  if(retcode) goto BACK;

  /** now let's see the values **/

  for(j = 0; j < n; j++){
    printf("%s = %g\n", mynames[j], x[j]);
  }


  GRBfreeenv(env);


 BACK:
  printf("\nexiting with retcode %d\n", retcode);
  return retcode;
}
