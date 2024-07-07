#ifndef MARKOWITZ_H
#define MARKOWITZ_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 函数声明
double **calculate_returns(double **prices, int rows, int cols);
double *calculate_mean_returns(double **returns, int rows, int cols);
double portfolio_annulised_return(double *weights, double *mean_returns, int cols);
// double *calculate_volatility(double **returns, int rows, int cols);
double portfolio_annulised_std(double *weights, double **cov_matrix, int cols);
double **calculate_covariance_matrix(double **returns, int rows, int cols);
double *markowitz_optimize(double **cov_matrix, double *returns, int n);
double calculate_sharpe_ratio(double *weights, double *mean_returns, double **cov_matrix, int num_assets);

#endif // MARKOWITZ_H
