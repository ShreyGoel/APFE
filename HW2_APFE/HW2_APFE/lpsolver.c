/** this function writes the linear program model to an lp file and calls gurobi to solve this lp **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gurobi_c++.h"
#include "gurobi_c.h"
#include "firstorder.h"

int lpsolver(portfolio *portfolio) {
    FILE *out;
    GRBenv     *env = NULL;
    GRBmodel *model = NULL;
    const char *lpfilename;
    int numvars, numcons;
    int optimstatus;
    double objval;
    char *varname;
    double x;
    int retcode = 0, error = 0;
    lpfilename = "linear_prog.lp";
    out = fopen(lpfilename, "w");
    if (out == NULL){
        printf("can't open %s\n", lpfilename);
        retcode = 500; 
        goto BACK;
    }
    printf("printing LP to file %s\n", lpfilename);

    fprintf(out, "Minimize ");
    for (int j = 0; j < portfolio->n; j++){
        fprintf(out, "+ "); 
        fprintf(out, "%f x%d ", portfolio->gradient[j], j);
    }
    fprintf(out, "\n");
    fprintf(out, "Subject to\n");
    fprintf(out, "c0: ");
    
    for (int k = 1; k < portfolio->n; k++){
        
        fprintf(out, "+ ");
        fprintf(out, "x%d ", k);
    }
    fprintf(out, " = 0\n");
    

    fprintf(out, "Bounds\n");
    for (int j = 0; j < portfolio->n; j++){
        fprintf(out, "%f <= x%d <= %f\n", (portfolio->lb[j]-portfolio->x[j]), j, (portfolio->ub[j]-portfolio->x[j]));
    }
    fprintf(out, "End\n");

    fclose(out);

    error = GRBloadenv(&env, "lpenv.log");
    if (error) goto BACK;

    // Open log file for writing
    /*
    log_file = fopen(logfilename, "w");
    if (!log_file){
        printf("Cannot open log file %s for writing\n", logfilename);
        exit(1);
    } 
    */

    // Read model from the specified file
    error = GRBreadmodel(env, lpfilename, &model);
    if (error) goto BACK;

    //fprintf(log_file, "Solving %s\n", lpfilename);

    // Solve the model
    error = GRBoptimize(model);
    if (error) goto BACK;

    // Compute the number of variables and constraints in the model
    error = GRBgetintattr(model, "NumVars", &numvars);
    if (error) goto BACK;
    error = GRBgetintattr(model, "NumConstrs", &numcons);
    if (error) goto BACK;

    //fprintf(log_file, "variables = %d\n", numvars);
    //fprintf(log_file, "constraints = %d\n", numcons);

    // Capture solution information
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto BACK;
  
    printf("\nOptimization complete\n");
    if (optimstatus == GRB_OPTIMAL) {
      error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
      if (error) goto BACK;
      printf("Optimal objective: %.4e\n\n", objval);
      //fprintf(log_file, "Optimal objective = %.4e\n", objval);
    } else if (optimstatus == GRB_INF_OR_UNBD) {
      printf("Model is infeasible or unbounded\n\n");
      //fprintf(log_file, "->LP infeasible or unbounded\n");
      goto BACK;
    } else if (optimstatus == GRB_INFEASIBLE) {
      printf("Model is infeasible\n\n");
      //fprintf(log_file, "->LP infeasible\n");
      goto BACK;
    } else if (optimstatus == GRB_UNBOUNDED) {
      printf("Model is unbounded\n\n");
      //fprintf(log_file, "->LP unbounded\n");
      goto BACK;
    } else {
      printf("Optimization was stopped with status = %d\n\n", optimstatus);
      goto BACK;
    }

      for (int j = 0; j < numvars; j++)
      {
        error = GRBgetstrattrelement(model, "Varname", j, &varname);
        if (error) goto BACK;
        error = GRBgetdblattrelement(model, "X", j, &x);
        if (error) goto BACK;
        printf("%s = %f\n", varname, x);
        //fprintf(log_file, "%s = %f\n", varname, x);
        portfolio->y_step[j] = x;
      }
    

    

    //fprintf(log_file, "bye.\n");
    
    BACK:

    //fclose(log_file);

    if (error) {
        printf("ERROR %s \n", GRBgeterrormsg(env));
        exit(1);
    }

    // Free models
    GRBfreemodel(model);

    // Free environment
    GRBfreeenv(env);
    
    return retcode;
}