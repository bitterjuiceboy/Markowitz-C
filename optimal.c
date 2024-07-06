#include "markowitz.h"
#include "optimal.h"
#include <stdlib.h>


double **monte_carlo(int num_portfolios, double *mean_returns, double **cov_matrix, int num_assets) {
    double **result = (double **)malloc(num_portfolios * sizeof(double *));
    for (int i = 0; i < num_portfolios; i++) {
        result[i] = (double *)calloc(num_assets + NUM_COLS, sizeof(double));
    }
    // hard code. 07.2024, 4.36%. 
    // https://www.macrotrends.net/2016/10-year-treasury-bond-rate-yield-chart
    double risk_free_rate = 0.0436;

    srand((unsigned int)time(NULL));

    double *weights = (double *)calloc(num_assets, sizeof(double));

    // simulation
    for (int i = 0; i < num_portfolios; i++) {
        // 生成随机weights并归一化。
        double sum = 0.0;
        for (int j = 0; j < num_assets; j++) {
            weights[j] = (double) rand() / RAND_MAX;
            sum += weights[j];
        }
        for (int j = 0; j < num_assets; j++) {
            weights[j] /= sum;
            result[i][j] = weights[j];
        }
        
        // return
        result[i][num_assets] = portfolio_annulised_return(weights, mean_returns, num_assets);
        // std
        result[i][num_assets + 1] = portfolio_annulised_std(weights, cov_matrix, num_assets);
        // sharp ratio
        result[i][num_assets + 2] = (result[i][num_assets] - risk_free_rate) / result[i][num_assets + 1];

    }

    free(weights);

    return result;
}

void find_max_return_for_std(double **result, int num_portfolios, int num_assets, double target_std) {
    double max_return = 0.0;
    for (int i = 0; i < num_portfolios; i++) {
        if (result[i][num_assets + 1] <= target_std && result[i][num_assets] > max_return) {
            max_return = result[i][num_assets];
        }
    }
    printf("Max return for std %f is %f\n", target_std, max_return);
}

void find_min_std_for_return(double **result, int num_portfolios, int num_assets, double target_return) {
    double min_std = 100.0;
    for (int i = 0; i < num_portfolios; i++) {
        if (result[i][num_assets] >= target_return && result[i][num_assets + 1] < min_std) {
            min_std = result[i][num_assets + 1];
        }
    }
    printf("Min std for return %f is %f\n", target_return, min_std);
}

// 定义一个函数来比较两个投资组合的std，用于排序
int compare_by_sharpe_std(const void *a, const void *b) {
    Portfolio *portfolioA = (Portfolio *)a;
    Portfolio *portfolioB = (Portfolio *)b;
    return (portfolioA->std > portfolioB->std) - (portfolioA->std < portfolioB->std);
}

double **find_efficient_frontier(double **result, int num_portfolios, int num_assets) {
    Portfolio *portfolios = (Portfolio *)malloc(num_portfolios * sizeof(Portfolio));
    ;
    for (int i = 0; i < num_portfolios; i++) {
        portfolios[i].return_value = result[i][num_assets];
        portfolios[i].std = result[i][num_assets + 1];
        portfolios[i].sharpe_ratio = result[i][num_assets + 2];
        portfolios[i].weights = (double*)malloc((num_assets + NUM_COLS) * sizeof(double));
        for(int j = 0; j < num_assets + NUM_COLS; j++) {
            portfolios[i].weights[j] = result[i][j];
        }
    }
    
    // 按风险（标准差）升序排序
    qsort(portfolios, num_portfolios, sizeof(Portfolio), compare_by_sharpe_std);
    double **efficient_frontier = (double **)malloc(num_portfolios * sizeof(double *));
    for (int i = 0; i < num_portfolios; i++) {
        efficient_frontier[i] = (double *)calloc(num_assets + NUM_COLS, sizeof(double));
    }
    int ef_index = 0;
    double max_return = -INFINITY;

    for (int i = 0; i < num_portfolios; i++) {
        double portfolio_return = portfolios[i].return_value;
        if (portfolio_return > max_return) {
            for(int j = 0; j < num_assets + NUM_COLS; j++) {
                efficient_frontier[ef_index][j] = portfolios[i].weights[j];
            }
            ef_index++;
            max_return = portfolio_return;
        }
    }

    for (int i = 0; i < ef_index; i++) {
        for (int j = 0; j < num_assets + NUM_COLS; j++) {
            printf("%f, ", efficient_frontier[i][j]);
        }
        printf("\n");
    }
    return efficient_frontier;
}