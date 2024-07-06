double **find_efficient_frontier(double **result, int num_portfolios, int num_assets) {
    Portfolio *portfolios = (Portfolio *)malloc(num_portfolios * sizeof(Portfolio));
    for (int i = 0; i < num_portfolios; i++) {
        portfolios[i].return_value = result[i][num_assets];
        portfolios[i].std = result[i][num_assets + 1];
        portfolios[i].sharpe_ratio = result[i][num_assets + 2];
        for(int j = 0; j < num_assets + NUM_COLS; j++) {
            portfolios[i].weights[j] = result[i][j];
        }
    }
    // 按风险（标准差）升序排序
    qsort(portfolios, num_portfolios, sizeof(double *), compare_by_sharpe_std);

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
printf("Efficient Frontier:%f\n", efficient_frontier[0][0]);