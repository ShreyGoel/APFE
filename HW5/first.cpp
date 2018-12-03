
#include <stdio.h>
#include <stdlib.h>


#include <gurobi_c++.h>

using namespace std;

int arbitrage(double *prices, double n, double K, double r, int N, double *x);

int main(int argc, char **argv)
{
	double *obj = NULL;
	double *x;
	int n = 4, K = 3, N = 2;
	double r = 0.05;
	obj = (double *)calloc(n*(K+1), sizeof(double));
	x = (double *)calloc(n, sizeof(double));


	obj[0] = 2.0; obj[1] = 5.0; obj[2] = 7; obj[3] = 3.5;
	obj[4] = 1.0; obj[5] = -4; obj[6] = 1.0; obj[7] = 0.0;
	obj[8] = 2.0; obj[9] = 7.0; obj[10] = -5.0; obj[11] = 0.0;
	obj[12] = 2.0;  obj[13] = 9.0; obj[14] = -9.0; obj[15] = 5.0;

	arbitrage(obj, n, K, r, N, x);
	
}

int arbitrage(double *prices, double n, double K, double r, int N, double *x)
{
	int retcode = 0;
	GRBenv   *env = NULL;
	GRBmodel *model = NULL;
	const int n_assets = n;
	const int n_scenarios = K;
	int i, j;
	int *cind;
	double *cval;

	retcode = GRBloadenv(&env, "first.log");
	if (retcode) goto BACK;

	/* Create initial model */
	retcode = GRBnewmodel(env, &model, "first", n_assets + 1, NULL, NULL, NULL, NULL, NULL);
	if (retcode) goto BACK;


	/* initialize variables */
	for (j = 0; j <= n_assets; j++) {
		if (j == 0) {

			string p = "x" + to_string(j);
			const char *c = p.c_str();
			retcode = GRBsetstrattrelement(model, "VarName", j, c);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "Obj", j, 1);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "LB", j, 0.0);
			if (retcode) goto BACK;
			
			/*
			string q = "y" + to_string(j);
			const char *d = p.c_str();
			retcode = GRBsetstrattrelement(model, "VarName", j + n_assets + 1, c);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "Obj", j + n_assets + 1, 1);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "VType", j + n_assets + 1, 'B');
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "LB", j + n_assets + 1, 0.0);
			if (retcode) goto BACK;
			*/
		}
		else {


			string p = "x" + to_string(j);
			const char *c = p.c_str();
			retcode = GRBsetstrattrelement(model, "VarName", j, c);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "Obj", j, prices[j-1]);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "LB", j, 0.0);
			if (retcode) goto BACK;
			
			/*
			string q = "y" + to_string(j);
			const char *d = p.c_str();
			retcode = GRBsetstrattrelement(model, "VarName", j + n_assets + 1, c);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "Obj", j + n_assets + 1, 1);
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "VType", j + n_assets + 1, 'B');
			if (retcode) goto BACK;

			retcode = GRBsetdblattrelement(model, "LB", j + n_assets + 1, 0.0);
			if (retcode) goto BACK;
			*/
		}
	}

	/** now we will add one constraint at a time **/
	/** we need to have a couple of auxiliary arrays **/

	cind = (int *)calloc(n_assets+1, sizeof(int));
	cval = (double *)calloc(n_assets+1, sizeof(double));


	for (j = 0; j <= n_assets; j++) {
		cind[j] = j;
	}

	for (i = 1; i <= n_scenarios; i++) {
		cval[0] = 1 + r;

		for (j = 0; j < n_assets; j++) {

				cval[j + 1] = prices[i*n_assets + j];
		}

		string name_temp = to_string(i) + "_constraint";
		const char *c = name_temp.c_str();
		retcode = GRBaddconstr(model, n_assets + 1, cind, cval, GRB_GREATER_EQUAL, 0, c);
		if (retcode) goto BACK;
	}

	retcode = GRBupdatemodel(model);
	if (retcode) goto BACK;


	/** optional: write the problem **/

	retcode = GRBwrite(model, "myfirst.lp");
	if (retcode) goto BACK;


	retcode = GRBoptimize(model);
	if (retcode) goto BACK;


	/** get solution **/


	retcode = GRBgetdblattrarray(model,
		GRB_DBL_ATTR_X, 0, n_assets,
		x);
	if (retcode) goto BACK;

	/** now let's see the values **/

	for (j = 0; j < n_assets; j++) {
		printf("%s = %g\n", "x" + to_string(j), x[j]);
	}


	GRBfreeenv(env);


BACK:
	printf("\nexiting with retcode %d\n", retcode);
	return retcode;

}
