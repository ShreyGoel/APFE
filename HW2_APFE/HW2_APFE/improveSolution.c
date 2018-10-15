#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "importantHeaders.h"

int gradient(dataStruct *data);
int stepdirection(dataStruct * data);
int stepsize(dataStruct * data);
int move(dataStruct * data);
double *base_arr;
int compar(const void *a, const void *b);

int improve(dataStruct *data){
    int retcode = 0;
    int counter;
    double F_x = 0;
    printf("Objective function value is\n");
    for (int i = 0; i < data->n; i++){
        F_x -= data->mu[i]*data->x[i];
        for (int j = 0; j <= i; j++){
            if (j < i)
                F_x += 2*data->lambda*data->covariance[i*data->n + j]*data->x[i]*data->x[j];
            if (j == i)
                F_x += data->lambda*data->covariance[i*data->n + j]*data->x[i]*data->x[j];
        }
    }
    printf("%f\n", F_x);
    printf("improving the feasible solution\n");

    for (counter = 0; counter < data->num_iter; counter++){
        data->iter_counter = counter;
        fprintf(data->output, "iteration # %d\n", data->iter_counter);
        printf("iteration # %d\n", data->iter_counter);

        /** compute gradient **/
        retcode = gradient(data);
        if (retcode) goto BACK; 

        /** compute step direction **/
        retcode = stepdirection(data);
        if (retcode) goto BACK;

        /** compute step size **/
        printf("going into stepsize\n");
        retcode = stepsize(data);
        if (retcode == 400) counter = data->num_iter;

        /** move in the descent direction by stepsize **/
        retcode = move(data);
        if (retcode) goto BACK;

        F_x = 0;
        printf("Objective function value is\n");
        for (int i = 0; i < data->n; i++){
            F_x -= data->mu[i]*data->x[i];
            for (int j = 0; j <= i; j++){
                if (j < i)
                    F_x += 2*data->lambda*data->covariance[i*data->n + j]*data->x[i]*data->x[j];
                if (j == i)
                    F_x += data->lambda*data->covariance[i*data->n + j]*data->x[i]*data->x[j];
            }
        }
        printf("%f\n", F_x);
        fprintf(data->output, "done with iteration # %d\n\n", data->iter_counter);
		printf("done with iteration # %d\n\n", data->iter_counter);
    }
    fprintf(data->output, "Optimal solution is \n");
    //printf("Optimal solution is \n");
    for (int i = 0; i < data->n; i++){
        fprintf(data->output, "%f\n", data->x[i]);
        //printf("%f\n", data->x[i]);
    }
    BACK:
    return retcode;
}

int gradient(dataStruct *data){
    int retcode = 0;
    int first, second,i,j;
    int n = data->n;
    double lambda = data->lambda;

    fprintf(data->output, "gradient iteration # %d\n", data->iter_counter);
    printf("gradient iteration # %d\n", data->iter_counter);

    for (j = 0; j < n; j++){
        first = 0;
        for (i = 0; i < n; i++)
            first += 2*lambda*data->x[i]*data->covariance[j*n + i];
        
        second = -data->mu[j];

        data->gradient[j] = first + second;
        //printf("%f\n",data->gradient[j]);
    }


    BACK:
    return retcode;
}

int stepdirection(dataStruct *data){
    int retcode = 0, n = data->n, j, m_opt, *index;
    double *y, *g, *lb, *ub, *x, min_prod;
    data->grad_prod = 0;
    printf("step direction iteration # %d\n", data->iter_counter);

    //allocate memory for sorted gradient
    g = (double *)calloc(n, sizeof(double));
    //allocate memory for sorted lb
    lb = (double *)calloc(n, sizeof(double));
    //allocate memory for sorted ub
    ub = (double *)calloc(n, sizeof(double));
    //allocate memory for sorted x
    x = (double *)calloc(n, sizeof(double));
    
//    retcode = lpsolver(data);
    // allocate memory to hold the sorted gradient index
    
	index = (int *)calloc(n, sizeof(int));
    data->sorted_index = index; 
        
    retcode = sortIndex(data->gradient, index, data->n); 
        
    /** debug prints to check if sorted index is returned correctly
    for (int i = 0; i < data->n; i++)
        printf("%d\n", index[i]);
    **/
  
    // compute sorted gradient

    for(int i = 0; i < n; i++ ){
        g[i] = data->gradient[index[i]];
        lb[i] = data->lb[index[i]];
        ub[i] = data->ub[index[i]];
        x[i] = data->x[index[i]];
    }

    // allocate memory for y vector
    y = (double *)calloc(n, sizeof(double));
   
    /**loop through m values and check if the y is feasible,
     * i.e. l[m]-x[m] <= y[m] <= u[m] - x[m]
     * if y is feasible, calculate the minimum of g*y
     * here the gradient g is the sorted gradient
    **/
    // here min_prod is the current minimum g*y
   
    j = 0;
    m_opt = 0;

    for (int m = 0; m < n; m++){
        //printf("m is %d\n", m);
        double prod = 0;
        double sum_y = 0;
        for (int i = 0; i < n; i++){
            //printf("i is %d\n", i);
            if (i < m) {
                y[i] = lb[i] - x[i];
                sum_y +=y[i];

                //printf("current sum is %f\n",sum_y);
                //prod += g[i]*y[i];
            }
            if (i > m) {
                y[i] = ub[i] - x[i];
                sum_y +=y[i];

                //printf("current sum is %f\n",sum_y);
                //prod += g[i]*y[i];
            }
        }
        // as summation y(i) = 0; y(m) = -sum(y(i)) where i!=m
        y[m] = -1*sum_y;

        //prod += g[m]*y[m];
        //printf("y[m] is %f\n",y[m]);
        //printf("bounds are %f   %f \n",(lb[m]-x[m]), (ub[m]-x[m]));
        //printf("product is %f\n", prod);

        // if feasible y, compute g*y and calculate min_prod
        if ((y[m] >= lb[m] - x[m] - 0.00005) && (y[m] <= ub[m] - x[m] + 0.00005)){
            printf("feasible y #  %d with m value %d\n", j+1, m);
            for (int k = 0; k < n; k++) prod += g[k]*y[k];
            if ((j == 0) || (prod <= min_prod)){ 
                min_prod = prod;
                //printf("min_prod is %f\n", min_prod);
                m_opt = m;
                //printf("m is %d\n", m);
                //printf("m_opt is %d\n", m_opt);
            }
            //printf("min prod is %f\n", min_prod);           
            j++;
        }
    }
    data->grad_prod = min_prod;
    printf("grad product is %f\n", data->grad_prod);
    double sum_y = 0;
    if (j==0){
        printf("no feasible y found. please debug further\n");
        retcode = 400;
        goto BACK;
    }
    
    //allocate y_opt
    data->y_step = (double *)calloc(n, sizeof(double));
    printf("m_opt is %d\n", m_opt);
    //printf("psuedo break point 3\n");

    for (int i = 0; i < n; i++){
        if (i < m_opt) {
            data->y_step[index[i]] = lb[i] - x[i];
            sum_y +=data->y_step[index[i]];
        }
        if (i > m_opt) {
            data->y_step[index[i]] = ub[i] - x[i];
            sum_y +=data->y_step[index[i]]; 
        }
    }
   
    data->y_step[index[m_opt]] = -1*sum_y;

    //printf("optimum y is \n");
    /**debug prints to check for y_opt is returned correctly 
    for (int i = 0; i < n; i++)
        printf("%f\n", data->y_step[i]);
    **/

    BACK:
    free(g);
    free(lb);
    free(ub);
    free(y);
    free(x);

//    BACK:    
    printf("exiting step direction\n");
    return retcode;
}

int stepsize(dataStruct *data){
    printf("stepsize iteration # %d\n", data->iter_counter);
    int retcode = 0;
    double first = 0, second = 0, third = 0;

    /** step size s is determined by differentiating G(s) = F(X + sY)
     *  and equating G'(s) = 0;
     * solving we get -- s = (sum(mu(i)*y(i)) - lambda*sum(cov(j)[x(i)y(j)+x(j)y(i)]))/(2*lambda*sum(cov(j)y(i)y(j)))
     * s = (first - second)/(third)
    **/
   
    for (int i = 0; i < data->n; i++){
        first += data->mu[i]*data->y_step[i];
        for (int j = 0; j < data->n; j++){
            second += data->covariance[i*data->n + j]*(data->x[i]*data->y_step[j] + data->x[j]*data->y_step[i]);
            third += data->y_step[j]*data->y_step[i]*data->covariance[i*data->n + j];
            
        }
    }
    //printf("first is %f\n", first);
    data->step_size = (first - data->lambda*second)/(2*third*data->lambda);
    
    if (data->step_size < 0 ) data->step_size = 0;
    if (data->step_size > 1 ) data->step_size = 1;
    /**
    if (data->step_size < 0 || data->step_size > 1) {
        G_prime_0 = second - first;
        G_prime_1 = second + third - first; 
    }
    **/
    printf("step size is %f\n", data->step_size); 
    
    // get out of improve loop if step size is very small 
    if (fabs(data->step_size) < 0.00005) retcode = 400;
    
    BACK:
    return retcode;
}

int move(dataStruct *data){
    int retcode = 0, j = 0;
    for (int i =0; i < data->n; i++){
        data->x[i] += data->step_size*data->y_step[i];
        if ((data->x[i] >= data->lb[i] - 0.0000005) && (data->x[i] <= data->ub[i] + 0.0000005))
            j++;
    }
    if (j == data->n) printf("improved solution is feasible\n");
    else printf("%d\n",j);

    BACK:
    return retcode;
}

int sortIndex(double *gradient, int *index, int n) {
	base_arr = gradient;

	/** initialize the index array values **/
	for (int i = 0; i < n; i++) {
		index[i] = i;
	}
	qsort(index, n, sizeof(int), compar);
	return 0;
}

int compar(const void *a, const void *b) {
	int aa = *((int*)a), bb = *((int*)b);
	if (base_arr[aa] < base_arr[bb])
		return 1;
	if (base_arr[aa] == base_arr[bb])
		return 0;
	if (base_arr[aa] > base_arr[bb])
		return -1;

	else return 100;
}