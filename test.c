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

    // 计算收益率
    double **returns = calculate_returns(csv_data->data, csv_data->rows, csv_data->cols);

    // 计算平均收益率
    double *mean_returns = calculate_mean_returns(returns, csv_data->rows, csv_data->cols);

    // 计算协方差矩阵
    double **cov_matrix = calculate_covariance_matrix(returns, csv_data->rows, csv_data->cols);

    double **result_max_return = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols,calculate_portfolio_return,1);
    double **result_min_std = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols,calculate_portfolio_std,0);
    double **result_max_sharp = numerical_optimization(num_simulations, mean_returns, cov_matrix, csv_data->cols,calculate_sharpe_ratio,1);

    double target_std = 0.20;
    double target_return = 0.50;

    /// 打印最大回报结果
    printf("最大回报结果:\n");
    for (int i = 0; i < csv_data->cols; i++) {
        printf("%s, ", csv_data->colnames[i+1]);
    }
    printf("return, volatility, sharp ratio, \n");
    for (int i = 0; i < num_simulations; i++) {
        for (int j = 0; j < csv_data->cols + NUM_COLS; j++) {
            printf("%f, ", result_max_return[i][j]);
        }
        printf("\n");
    }

    find_max_return_for_std(result_max_return, num_simulations, csv_data->cols, target_std);
    find_min_std_for_return(result_max_return, num_simulations, csv_data->cols, target_return);

    double **efficient_frontier_for_max_return = find_efficient_frontier(result_max_return, num_simulations, csv_data->cols);

    // 打印最小标准差结果
    printf("最小标准差结果:\n");
    for (int i = 0; i < csv_data->cols; i++) {
        printf("%s, ", csv_data->colnames[i+1]);
    }
    printf("return, volatility, sharp ratio, \n");
    for (int i = 0; i < num_simulations; i++) {
        for (int j = 0; j < csv_data->cols + NUM_COLS; j++) {
            printf("%f, ", result_min_std[i][j]);
        }
        printf("\n");
    }

    find_max_return_for_std(result_min_std, num_simulations, csv_data->cols, target_std);
    find_min_std_for_return(result_min_std, num_simulations, csv_data->cols, target_return);

    double **efficient_frontier_for_min_std = find_efficient_frontier(result_min_std, num_simulations, csv_data->cols);

    // 打印最大夏普比率结果
    printf("最大夏普比率结果:\n");
    for (int i = 0; i < csv_data->cols; i++) {
        printf("%s, ", csv_data->colnames[i+1]);
    }
    printf("return, volatility, sharp ratio, \n");
    for (int i = 0; i < num_simulations; i++) {
        for (int j = 0; j < csv_data->cols + NUM_COLS; j++) {
            printf("%f, ", result_max_sharp[i][j]);
        }
        printf("\n");
    }

    find_max_return_for_std(result_max_sharp, num_simulations, csv_data->cols, target_std);
    find_min_std_for_return(result_max_sharp, num_simulations, csv_data->cols, target_return);

    double **efficient_frontier_for_max_sharp = find_efficient_frontier(result_max_sharp, num_simulations, csv_data->cols);

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
        free(result_min_std[i]);
        free(result_max_return[i]);
        free(result_max_sharp[i]);
    }
    free(result_min_std);
    free(result_max_return);
    free(result_max_sharp);

    return 0;
}
