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

    // 使用指针来储存有效前沿的大小
    int ef_size_value = 0;
    int *ef_size = &ef_size_value;

    CSVData *csv_data = read_csv(filepath);
    int num_assets = csv_data->cols;

    // 计算收益率
    double **returns = calculate_returns(csv_data->data, csv_data->rows, csv_data->cols);

    // 计算平均收益率
    double *mean_returns = calculate_mean_returns(returns, csv_data->rows, csv_data->cols);

    // 计算协方差矩阵
    double **cov_matrix = calculate_covariance_matrix(returns, csv_data->rows, csv_data->cols);

    // 基于 Monte Carlo 的Markowitz优化
    double **result = monte_carlo(num_simulations, mean_returns, cov_matrix, csv_data->cols);

    // 基于 SGD 的Markowitz优化
    double **result_max_sharp = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols, calculate_sharpe_ratio, 1);
    double **result_min_std = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols, calculate_portfolio_std, 0);
    double **result_max_return = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols, calculate_portfolio_return, 1);

    // 将result中的数据存储到Portfolio结构体中 (在这里仅仅使用获得的Monte Carlo数据进行演示输出)
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

    Portfolio *efficient_frontier = find_efficient_frontier(portfolios, num_simulations, num_assets, ef_size);
    
    // show result
    FILE *file = fopen("/Users/juice/Desktop/coding/markowitz-c-master/data/output.csv", "w");// your file path

    if (!file) {
        perror("Unable to open file");
        return 0;
    }
    
    // print title
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
    
    fprintf(file, "Efficient frontier:\n");
    for (int i = 0; i < ef_size_value; i++) {
        for (int j = 0; j < num_assets; j++) {
            fprintf(file, "%f,", efficient_frontier[i].weights[j]);
        }
        fprintf(file, "%f,%f,%f", efficient_frontier[i].return_value, efficient_frontier[i].std, efficient_frontier[i].sharpe_ratio);
        fprintf(file, "\n");
    }
    
    fclose(file);
    
    double target_std = 0.20;
    double target_return = 0.50;

    find_max_return_for_std(result, num_simulations, csv_data->cols, target_std);
    find_min_std_for_return(result, num_simulations, csv_data->cols, target_return);

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
        free(result_max_sharp[i]);
        free(result_min_std[i]);
        free(result_max_return[i]);
    }
    free(result);
    
    // **portfolios
    for (int i = 0; i < num_simulations; i++) {
        free(portfolios[i].weights);
    }
    free(portfolios);

    // **efficient_frontier
    for (int i = 0; i < num_simulations; i++) {
        free(efficient_frontier[i].weights);
    }
    free(efficient_frontier);

    return 0;
}
