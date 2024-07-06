#include <stdio.h>
#include <assert.h>

void test_monte_carlo() {
    int num_portfolios = 10;
    int num_assets = 5;
    double mean_returns[] = {0.1, 0.2, 0.15, 0.12, 0.18};
    double **cov_matrix = (double **)malloc(num_assets * sizeof(double *));
    for (int i = 0; i < num_assets; i++) {
        cov_matrix[i] = (double *)malloc(num_assets * sizeof(double));
        for (int j = 0; j < num_assets; j++) {
            cov_matrix[i][j] = 0.0;
        }
    }

    double **result = monte_carlo(num_portfolios, mean_returns, cov_matrix, num_assets);

    // Check if the result is not NULL
    assert(result != NULL);

    // Check if the result has the correct dimensions
    for (int i = 0; i < num_portfolios; i++) {
        assert(result[i] != NULL);
    }

    // Check if the weights are normalized
    for (int i = 0; i < num_portfolios; i++) {
        double sum = 0.0;
        for (int j = 0; j < num_assets; j++) {
            sum += result[i][j];
        }
        assert(sum == 1.0);
    }

    // Check if the portfolio return, std, and sharp ratio are calculated correctly
    for (int i = 0; i < num_portfolios; i++) {
        double portfolio_return = result[i][num_assets];
        double portfolio_std = result[i][num_assets + 1];
        double risk_free_rate = 0.0436;
        double sharp_ratio = (portfolio_return - risk_free_rate) / portfolio_std;
        assert(result[i][num_assets + 2] == sharp_ratio);
    }

    // Free memory
    for (int i = 0; i < num_assets; i++) {
        free(cov_matrix[i]);
    }
    free(cov_matrix);
    for (int i = 0; i < num_portfolios; i++) {
        free(result[i]);
    }
    free(result);
}

int main() {
    test_monte_carlo();
    printf("All tests passed!\n");
    return 0;
}