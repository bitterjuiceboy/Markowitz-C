#include "markowitz.h"
#include "optimal.h"
#include <stdlib.h>
#include <math.h>

#define LEARNING_RATE 0.01
#define MAX_ITERATIONS 10000
#define TOLERANCE 1e-6
#define BATCH_SIZE 10

void stochastic_gradient_descent(double *weights, double *mean_returns, double **cov_matrix, int num_assets, double (*objective_function)(double *, double *, double **, int)) {
    double *gradient = (double *)calloc(num_assets, sizeof(double));
    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double current_value = objective_function(weights, mean_returns, cov_matrix, num_assets);
        
        // Calculate the gradient using a random subset (mini-batch)
        for (int batch = 0; batch < BATCH_SIZE; batch++) {
            int random_asset = rand() % num_assets;
            double original_weight = weights[random_asset];
            
            // Perturb the weight slightly
            weights[random_asset] += TOLERANCE;
            double new_value = objective_function(weights, mean_returns, cov_matrix, num_assets);
            
            // Approximate the gradient
            gradient[random_asset] = (new_value - current_value) / TOLERANCE;
            
            // Restore the original weight
            weights[random_asset] = original_weight;
        }

        // Update weights using the gradient
        double sum = 0.0;
        for (int i = 0; i < num_assets; i++) {
            weights[i] += LEARNING_RATE * gradient[i];
            if (weights[i] < 0) weights[i] = 0; // Ensure weights are non-negative
            sum += weights[i];
        }

        // Normalize the weights
        for (int i = 0; i < num_assets; i++) {
            weights[i] /= sum;
        }

        // Check for convergence
        double new_value = objective_function(weights, mean_returns, cov_matrix, num_assets);
        if (fabs(new_value - current_value) < TOLERANCE) {
            break;
        }
    }

    free(gradient);
}

double **numerical_optimization(int num_portfolios, double *mean_returns, double **cov_matrix, int num_assets, double (*objective_function)(double *, double *, double **, int)) {
    double **result = (double **)malloc(num_portfolios * sizeof(double *));
    for (int i = 0; i < num_portfolios; i++) {
        result[i] = (double *)calloc(num_assets + NUM_COLS, sizeof(double));
    }
    // hard code. 07.2024, 4.36%. 
    // https://www.macrotrends.net/2016/10-year-treasury-bond-rate-yield-chart
    double risk_free_rate = 0.0436;

    srand((unsigned int)time(NULL));

    // Generate initial random weights and optimize them
    for (int i = 0; i < num_portfolios; i++) {
        double *weights = (double *)calloc(num_assets, sizeof(double));
        double sum = 0.0;
        for (int j = 0; j < num_assets; j++) {
            weights[j] = (double) rand() / RAND_MAX;
            sum += weights[j];
        }
        for (int j = 0; j < num_assets; j++) {
            weights[j] /= sum;
            result[i][j] = weights[j];
        }

        // Optimize the weights using stochastic gradient descent
        stochastic_gradient_descent(weights, mean_returns, cov_matrix, num_assets, objective_function);

        for (int j = 0; j < num_assets; j++) {
            result[i][j] = weights[j];
        }

         // return
        result[i][num_assets] = portfolio_annulised_return(weights, mean_returns, num_assets);
        // std
        result[i][num_assets + 1] = portfolio_annulised_std(weights, cov_matrix, num_assets);
        // sharp ratio
        result[i][num_assets + 2] = (result[i][num_assets] - risk_free_rate) / result[i][num_assets + 1];

        free(weights);
    }

    return result;
}

