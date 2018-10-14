#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "firstorder.h"

int gradient(portfolio *portfolio);
int stepdirection(portfolio * portfolio);
int stepsize(portfolio * portfolio);
int move(portfolio * portfolio);
int stepsize_1(portfolio *portfolio);

int improve(portfolio *portfolio){
    int retcode = 0;
    int counter;
    double F_x = 0;
    printf("Objective function value is\n");
    for (int i = 0; i < portfolio->n; i++){
        F_x -= portfolio->mu[i]*portfolio->x[i];
        for (int j = 0; j <= i; j++){
            if (j < i)
                F_x += 2*portfolio->lambda*portfolio->covariance[i*portfolio->n + j]*portfolio->x[i]*portfolio->x[j];
            if (j == i)
                F_x += portfolio->lambda*portfolio->covariance[i*portfolio->n + j]*portfolio->x[i]*portfolio->x[j];
        }
    }
    printf("%f\n", F_x);
    printf("improving the feasible solution\n");

    for (counter = 0; counter < portfolio->num_iter; counter++){
        portfolio->iter_counter = counter;
        fprintf(portfolio->output, "iteration # %d\n", portfolio->iter_counter);
        printf("iteration # %d\n", portfolio->iter_counter);

        /** compute gradient **/
        retcode = gradient(portfolio);
        if (retcode) goto BACK; 

        /** compute step direction **/
        retcode = stepdirection(portfolio);
        if (retcode) goto BACK;

        /** compute step size **/
        printf("going into stepsize\n");
        retcode = stepsize(portfolio);
        if (retcode == 400) counter = portfolio->num_iter;
        //retcode = stepsize_1(portfolio);

        /** move in the descent direction by stepsize **/
        retcode = move(portfolio);
        if (retcode) goto BACK;

        F_x = 0;
        printf("Objective function value is\n");
        for (int i = 0; i < portfolio->n; i++){
            F_x -= portfolio->mu[i]*portfolio->x[i];
            for (int j = 0; j <= i; j++){
                if (j < i)
                    F_x += 2*portfolio->lambda*portfolio->covariance[i*portfolio->n + j]*portfolio->x[i]*portfolio->x[j];
                if (j == i)
                    F_x += portfolio->lambda*portfolio->covariance[i*portfolio->n + j]*portfolio->x[i]*portfolio->x[j];
            }
        }
        printf("%f\n", F_x);
        fprintf(portfolio->output, "done with iteration # %d\n\n", portfolio->iter_counter);
		printf("done with iteration # %d\n\n", portfolio->iter_counter);
    }
    fprintf(portfolio->output, "Optimal solution is \n");
    //printf("Optimal solution is \n");
    for (int i = 0; i < portfolio->n; i++){
        fprintf(portfolio->output, "%f\n", portfolio->x[i]);
        //printf("%f\n", portfolio->x[i]);
    }
    BACK:
    return retcode;
}

int gradient(portfolio *portfolio){
    int retcode = 0;
    int first, second,i,j;
    int n = portfolio->n;
    double lambda = portfolio->lambda;

    fprintf(portfolio->output, "gradient iteration # %d\n", portfolio->iter_counter);
    printf("gradient iteration # %d\n", portfolio->iter_counter);

    for (j = 0; j < n; j++){
        first = 0;
        for (i = 0; i < n; i++)
            first += 2*lambda*portfolio->x[i]*portfolio->covariance[j*n + i];
        
        second = -portfolio->mu[j];

        portfolio->gradient[j] = first + second;
        //printf("%f\n",portfolio->gradient[j]);
    }


    BACK:
    return retcode;
}

int stepdirection(portfolio *portfolio){
    int retcode = 0, n = portfolio->n, j, m_opt, *index;
    double *y, *g, *lb, *ub, *x, min_prod;
    portfolio->grad_prod = 0;
    printf("step direction iteration # %d\n", portfolio->iter_counter);

    //allocate memory for sorted gradient
    g = (double *)calloc(n, sizeof(double));
    //allocate memory for sorted lb
    lb = (double *)calloc(n, sizeof(double));
    //allocate memory for sorted ub
    ub = (double *)calloc(n, sizeof(double));
    //allocate memory for sorted x
    x = (double *)calloc(n, sizeof(double));
    
//    retcode = lpsolver(portfolio);
    // allocate memory to hold the sorted gradient index
    

    portfolio->sorted_index = index; 
    index = (int *)calloc(n, sizeof(int));
        
    retcode = sortIndex(portfolio->gradient, index, portfolio->n); 
        
    /** debug prints to check if sorted index is returned correctly
    for (int i = 0; i < portfolio->n; i++)
        printf("%d\n", index[i]);
    **/
  
    // compute sorted gradient

    for(int i = 0; i < n; i++ ){
        g[i] = portfolio->gradient[index[i]];
        lb[i] = portfolio->lb[index[i]];
        ub[i] = portfolio->ub[index[i]];
        x[i] = portfolio->x[index[i]];
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
    portfolio->grad_prod = min_prod;
    printf("grad product is %f\n", portfolio->grad_prod);
    double sum_y = 0;
    if (j==0){
        printf("no feasible y found. please debug further\n");
        retcode = 400;
        goto BACK;
    }
    
    //allocate y_opt
    portfolio->y_step = (double *)calloc(n, sizeof(double));
    printf("m_opt is %d\n", m_opt);
    //printf("psuedo break point 3\n");

    for (int i = 0; i < n; i++){
        if (i < m_opt) {
            portfolio->y_step[index[i]] = lb[i] - x[i];
            sum_y +=portfolio->y_step[index[i]];
        }
        if (i > m_opt) {
            portfolio->y_step[index[i]] = ub[i] - x[i];
            sum_y +=portfolio->y_step[index[i]]; 
        }
    }
   
    portfolio->y_step[index[m_opt]] = -1*sum_y;

    //printf("optimum y is \n");
    /**debug prints to check for y_opt is returned correctly 
    for (int i = 0; i < n; i++)
        printf("%f\n", portfolio->y_step[i]);
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

int stepsize(portfolio *portfolio){
    printf("stepsize iteration # %d\n", portfolio->iter_counter);
    int retcode = 0;
    double first = 0, second = 0, third = 0;

    /** step size s is determined by differentiating G(s) = F(X + sY)
     *  and equating G'(s) = 0;
     * solving we get -- s = (sum(mu(i)*y(i)) - lambda*sum(cov(j)[x(i)y(j)+x(j)y(i)]))/(2*lambda*sum(cov(j)y(i)y(j)))
     * s = (first - second)/(third)
    **/
   
    //printf("first second third is %f %f %f\n", first, second, third);
    for (int i = 0; i < portfolio->n; i++){
        first += portfolio->mu[i]*portfolio->y_step[i];
        for (int j = 0; j < portfolio->n; j++){
            second += portfolio->covariance[i*portfolio->n + j]*(portfolio->x[i]*portfolio->y_step[j] + portfolio->x[j]*portfolio->y_step[i]);
            third += portfolio->y_step[j]*portfolio->y_step[i]*portfolio->covariance[i*portfolio->n + j];
            
        }
    }
    //printf("first is %f\n", first);
    portfolio->step_size = (first - portfolio->lambda*second)/(2*third*portfolio->lambda);
    
    if (portfolio->step_size < 0 ) portfolio->step_size = 0;
    if (portfolio->step_size > 1 ) portfolio->step_size = 1;
    /**
    if (portfolio->step_size < 0 || portfolio->step_size > 1) {
        G_prime_0 = second - first;
        G_prime_1 = second + third - first; 
    }
    **/
    printf("step size is %f\n", portfolio->step_size); 
    
    // get out of improve loop if step size is very small 
    if (fabs(portfolio->step_size) < 0.00005) retcode = 400;
    
    BACK:
    return retcode;
}

int move(portfolio *portfolio){
    int retcode = 0, j = 0;
    for (int i =0; i < portfolio->n; i++){
        portfolio->x[i] += portfolio->step_size*portfolio->y_step[i];
        if ((portfolio->x[i] >= portfolio->lb[i] - 0.0000005) && (portfolio->x[i] <= portfolio->ub[i] + 0.0000005))
            j++;
    }
    if (j == portfolio->n) printf("improved solution is feasible\n");
    else printf("%d\n",j);

    BACK:
    return retcode;
}

int stepsize_1(portfolio *portfolio){
    int retcode = 0;
    double *delta_g, *new_grad, first, second, numerator_sum = 0, denominator_sum = 0;
    delta_g = (double *)calloc(portfolio->n, sizeof(double));

    
    for (int j = 0; j < portfolio->n; j++){
        first = 0;
        for (int i = 0; i < portfolio->n; i++)
            first += (portfolio->x[i]+portfolio->y_step[i])*portfolio->covariance[j*portfolio->n + i];
        
        second = -portfolio->mu[j];

        delta_g[j] = 2*portfolio->lambda*first + second - portfolio->gradient[j];
        
        denominator_sum += delta_g[j]*delta_g[j];
        numerator_sum += delta_g[j]*portfolio->y_step[j];
        
    }
    portfolio->step_size = numerator_sum/denominator_sum;
    printf("new lamda is %f\n", portfolio->step_size);
    free(delta_g);
    return retcode;

}
