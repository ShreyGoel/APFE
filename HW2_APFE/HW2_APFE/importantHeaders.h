typedef struct {
	int n, num_iter, iter_counter, *sorted_index;
    double *x, *lb, *ub, *covariance, *mu, lambda, *gradient, *y_step, step_size, grad_prod;
    FILE *output;
}dataStruct;

int algo(dataStruct *data);
int feasible(dataStruct *data);
int getData(char *filename, dataStruct *data);
int improve(dataStruct *data);

int sortIndex(double *gradient, int *index, int n);