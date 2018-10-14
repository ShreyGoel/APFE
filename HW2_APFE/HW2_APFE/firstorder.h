typedef struct {
	int n, num_iter, iter_counter, *sorted_index;
    double *x, *lb, *ub, *covariance, *mu, lambda, *gradient, *y_step, step_size, grad_prod;
    FILE *output;
}portfolio;

int readit(char *nameoffile, portfolio *portfolio);
int algo(portfolio *portfolio);
int feasible(portfolio *portfolio);
int improve(portfolio *portfolio);

int sortIndex(double *gradient, int *index, int n);

int lpsolver(portfolio *portfolio);