#include "csv_reader.h"
#include "markowitz.h"
#include "optimal.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <csv_file_path> \n", argv[0]);
        return 1;
    }

    const char *filepath = argv[1];
    int num_simulations = atoi(argv[2]);

    CSVData *csv_data = read_csv(filepath);

    int num_assets = csv_data->cols;

    // 计算收益率
    double **returns = calculate_returns(csv_data->data, csv_data->rows, csv_data->cols);

    // 计算平均收益率
    double *mean_returns = calculate_mean_returns(returns, csv_data->rows, csv_data->cols);

    // 计算协方差矩阵
    double **cov_matrix = calculate_covariance_matrix(returns, csv_data->rows, csv_data->cols);

    double **result = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols,calculate_portfolio_return,1);
    double **result_min_std = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols,calculate_portfolio_std,0);
    double **result_max_sharp = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols,calculate_sharpe_ratio,1);
    
    // 定义一个结构体来存储投资组合的性能指标
    Portfolio *portfolios = (Portfolio *)malloc(num_simulations * sizeof(Portfolio));
    for (int i = 0; i < num_simulations; i++) {
        portfolios[i].return_value = result[i][num_assets];
        portfolios[i].std = result[i][num_assets + 1];
        portfolios[i].sharpe_ratio = result[i][num_assets + 2];
        portfolios[i].weights = (double*)malloc((num_assets + NUM_COLS) * sizeof(double));
        for(int j = 0; j < num_assets + NUM_COLS; j++) {
            portfolios[i].weights[j] = result[i][j];
        }
    }

    FILE *file = fopen("/Users/juice/Desktop/coding/markowitz-c-master/data/output.csv", "w");
    if (!file) {
        perror("Unable to open file");
        return 0;
    }

    // 写入标题行
    for (int i = 0; i < csv_data->cols; i++) {
        fprintf(file,"%s, ", csv_data->colnames[i+1]);  
    }
    fprintf(file, "Return,Std,Sharpe Ratio\n");

    // 遍历portfolios数组并写入数据
    for (int i = 0; i < num_simulations; i++) {
        for (int j = 0; j < num_assets; j++) {
            fprintf(file, "%f,", portfolios[i].weights[j]);
        }
        fprintf(file, "%f,%f,%f", portfolios[i].return_value, portfolios[i].std, portfolios[i].sharpe_ratio);
        fprintf(file, "\n");
    }
    
    fclose(file);


    // 释放内存
    // *mean_returns
    free(mean_returns);
    // **cov_matrix
    for (int i = 0; i < csv_data->cols; i++) {
        free(cov_matrix[i]);
    }
    free(cov_matrix);
    // **returns
    for (int i = 0; i < csv_data->rows - 1; i++) {
        free(returns[i]);
    }
    free(returns);
    // CSVData
    free_csv_data(csv_data);

    // **result
    for (int i = 0; i < num_simulations; i++) {   
        free(result[i]);
    }

    free(result);
    return 0;
}

Portfolio *find_efficient_frontier(Portfolio *portfolios, int num_portfolios, int num_assets) {
    
    // 按风险（标准差）升序排序
    qsort(portfolios, num_portfolios, sizeof(Portfolio), compare_by_sharpe_std);
    Portfolio *efficient_frontier = (Portfolio *)malloc(num_portfolios * sizeof(Portfolio));
    int ef_index = 0;
    double max_return = -INFINITY;

    for (int i = 0; i < num_portfolios; i++) {
        double portfolio_return = portfolios[i].return_value;
        if (portfolio_return > max_return) {
            efficient_frontier[ef_index].return_value = portfolio_return;
            efficient_frontier[ef_index].std = portfolios[i].std;
            efficient_frontier[ef_index].sharpe_ratio = portfolios[i].sharpe_ratio;
            efficient_frontier[ef_index].weights = (double *)malloc((num_assets + NUM_COLS) * sizeof(double));
            for (int j = 0; j < num_assets + NUM_COLS; j++) {
                efficient_frontier[ef_index].weights[j] = portfolios[i].weights[j];
            }
            ef_index++;
            max_return = portfolio_return;
        }
    }

    // 根据ef_index更新efficient_frontier的大小
    efficient_frontier = (Portfolio *)realloc(efficient_frontier, ef_index * sizeof(Portfolio));

    return efficient_frontier;
}
// 根据ef_index更新efficient_frontier的大小
    efficient_frontier = (Portfolio *)realloc(efficient_frontier, ef_index * sizeof(Portfolio));