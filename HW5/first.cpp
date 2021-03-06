
#include <stdio.h>
#include <stdlib.h>
#include "impHeaders.h"
#include <gurobi_c++.h>



int main(int argc, char **argv)
{
	double *obj = NULL;
	double *x;
	int n, K, N;
	double r;

	ifstream fin("arb.dat");
	string num;
	std::vector<string> vec;

	while (fin >> num)
	{
		if (num != " ") {
			vec.push_back(num);
		}

	}

	n = stoi(vec[1]);
	K = stoi(vec[3]);
	r = stod(vec[5]);
	N = stoi(vec[7]);

	obj = (double *)calloc(n*(K + 1), sizeof(double));
	x = (double *)calloc(n, sizeof(double));

	for (int i = 0; i <= K; i++)
	{
		for (int j = 0; j < n; j++)
		{
			obj[j + i * n] = stod(vec[9 + j + i * (n + 1)]);
		}
	}

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
	char *vtype;
	double *obj;

	retcode = GRBloadenv(&env, "first.log");

	vtype = (char *)calloc(2 * n_assets + 1, sizeof(char));
	obj = (double *)calloc(2 * n_assets + 1, sizeof(double));

	for (i = 0; i < 2 * n_assets + 1; i++) {

		if (i <= n_assets) {
			if (i == 0) {
				obj[i] = 1;
			}
			else {
				obj[i] = prices[i - 1];
			}
			vtype[i] = GRB_CONTINUOUS;
		}
		else {
			obj[i] = 0;
			vtype[i] = GRB_BINARY;
		}

	}

	/* Create initial model */
	retcode = GRBnewmodel(env, &model, "first", 2 * n_assets + 1, obj, NULL, NULL, vtype, NULL);

	/** now we will add one constraint at a time **/
	/** we need to have a couple of auxiliary arrays **/

	cind = (int *)calloc(n_assets + 1, sizeof(int));
	cval = (double *)calloc(n_assets + 1, sizeof(double));


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
	}

	// add constraints x <= y or y - x >= 0 and sum(y) <= N

	for (j = 1; j <= n_assets; j++) {

		int ind[] = { j, j + n_assets };
		double val[] = { -1, 1 };

		string name_temp = to_string(j + n_scenarios) + "_constraint";
		const char *c = name_temp.c_str();
		retcode = GRBaddconstr(model, 2, ind, val, GRB_GREATER_EQUAL, 0, c);

	}

	// add constraints x <= y or y - x >= 0 and sum(y) <= N

	for (j = 0; j < n_assets; j++) {

		cval[j] = -1;
		cind[j] = j + n_assets + 1;

	}

	string name_temp = to_string(n_scenarios + 1 + n_assets) + "_constraint";
	const char *c = name_temp.c_str();
	retcode = GRBaddconstr(model, n_assets, cind, cval, GRB_GREATER_EQUAL, -N, c);
	if (retcode) goto BACK;


	retcode = GRBupdatemodel(model);
	if (retcode) goto BACK;


	/** optional: write the problem **/

	retcode = GRBwrite(model, "myfirst.lp");
	if (retcode) goto BACK;


	retcode = GRBoptimize(model);
	if (retcode) goto BACK;


	/** get solution **/


	retcode = GRBgetdblattrarray(model,
		GRB_DBL_ATTR_X, 0, n_assets + 1,
		x);
	if (retcode) goto BACK;

	/** now let's see the values **/

	for (j = 0; j <= n_assets; j++) {
		printf("%s = %g\n", "x" + to_string(j), x[j]);
	}


	GRBfreeenv(env);


BACK:
	printf("\nexiting with retcode %d\n", retcode);
	return retcode;

}