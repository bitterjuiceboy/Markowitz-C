#ifndef OPTIMAL_H
#define OPTIMAL_H

#include <time.h>


#define NUM_COLS 3  // return, std, sharp ratio.

// 定义一个结构体来存储投资组合的性能指标
typedef struct {
    double return_value;
    double std;
    double sharpe_ratio;
    double *weights;
} Portfolio;

// 函数声明
double **monte_carlo(int num_portfolios, double *mean_returns, double **cov_matrix, int num_assets);
void stochastic_gradient_descent(double *weights, double *mean_returns, double **cov_matrix, int num_assets, double (*objective_function)(double *, double *, double **, int),int max_or_min);
double **numerical_optimization(int num_portfolios, double *mean_returns, double **cov_matrix, int num_assets, double (*objective_function)(double *, double *, double **, int),int max_or_min);
double calculate_portfolio_return(double *weights, double *mean_returns, double **cov_matrix, int num_assets);
double calculate_portfolio_std(double *weights, double *mean_returns, double **cov_matrix, int num_assets);
double calculate_sharpe_ratio(double *weights, double *mean_returns, double **cov_matrix, int num_assets);
void find_max_return_for_std(double **result, int num_portfolios, int num_assets, double target_std);
void find_min_std_for_return(double **result, int num_portfolios, int num_assets, double target_return);
int compare_by_sharpe_std(const void *a, const void *b);
Portfolio *find_efficient_frontier(Portfolio *portfolios, int num_portfolios, int num_assets, int *ef_size);

#endif // OPTIMAL_H
