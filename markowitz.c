#include "markowitz.h"


// 计算收益率
double **calculate_returns(double **prices, int rows, int cols) {
    double **returns = (double **)malloc((rows - 1) * sizeof(double *));
    for (int i = 0; i < rows - 1; i++) {
        returns[i] = (double *)malloc(cols * sizeof(double));
    }
    for (int j = 0; j < cols; j++) {
        for (int i = 1; i < rows; i++) {
            returns[i-1][j] = (prices[i][j] - prices[i - 1][j]) / prices[i - 1][j];
        }
    }
    return returns;
}

// 计算平均收益率
double *calculate_mean_returns(double **returns, int rows, int cols) {
    double *mean_returns = (double *)malloc(cols * sizeof(double));
    // double **returns = calculate_returns(prices, rows, cols);
    for (int j = 0; j < cols; j++) {
        double sum = 0.0;
        for (int i = 0; i < rows - 1; i++) {
            sum += returns[i][j];
        }
        mean_returns[j] = sum / (rows - 1);
    }
    return mean_returns;
}

// // 计算波动率
// double *calculate_volatility(double **returns, int rows, int cols) {
//     double *volatility = (double *)malloc(cols * sizeof(double));
//     for (int j = 0; j < cols; j++) {
//         double sum = 0.0;
//         for (int i = 0; i < rows - 1; i++) {
//             sum += returns[i][j] * returns[i][j];
//         }
//         volatility[j] = sqrt(sum / rows);
//     }
//     return volatility;
// }

// 计算协方差矩阵
double **calculate_covariance_matrix(double **returns, int rows, int cols) {
    double **cov_matrix = (double **)malloc(cols * sizeof(double *));
    for (int i = 0; i < cols; i++) {
        cov_matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    double *mean_returns = calculate_mean_returns(returns, rows, cols);

    for (int i = 0; i < cols; i++) {
        for (int j = i; j < cols; j++) {  // 根据对称性，仅计算右上三角矩阵。
            double sum = 0.0;
            for (int k = 0; k < rows - 1; k++) {  // returns->rows = prices->rows - 1
                sum += (returns[k][i] - mean_returns[i]) * (returns[k][j] - mean_returns[j]);
            }
            cov_matrix[i][j] = sum / (rows - 1);
            if (i != j) {
                cov_matrix[j][i] = cov_matrix[i][j];  // 左下三角直接对称复制。
            }
        }
    }

    return cov_matrix;
}

// 计算组合年化收益
double portfolio_annulised_return(double *weights, double *mean_returns, int cols) {
    double ann_ret = 0;
    for (int i = 0; i < cols; i++) {
        ann_ret += weights[i] * mean_returns[i];
    }
    return ann_ret*252;    // 一年差不多252个交易日
}

// 函数定义：二维数组点乘
double **dot_product(double **matrix1, double **matrix2, int rows1, int cols1, int cols2) {
    double **result = (double **)malloc(rows1 * sizeof(double *));  // m1.shape = a * b , m2.shape = b * c, result.shape = a * c
    for (int i = 0; i < rows1; i++) {
        result[i] = (double *)malloc(cols2 * sizeof(double));
    }
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

// 计算组合年化波动率
double portfolio_annulised_std(double *weights, double **cov_matrix, int cols) {
    double ann_std = 0;
    double *covdtw = (double *)malloc(cols * sizeof(double));
    // w.T.dot(cov.dot(w))
    for (int i = 0; i < cols; i++) {
        covdtw[i] = 0.0;
        for (int j = 0; j < cols; j++) {
            covdtw[i] += cov_matrix[i][j] * weights[j];
        }
        ann_std += weights[i] * covdtw[i];
    }
    ann_std *= 252;  // 一年差不多252个交易日. 
    return sqrt(ann_std);  // var to std.
}

// 简单的Markowitz优化 (此处为简单示例，不考虑约束条件等复杂情况)
double *markowitz_optimize(double **cov_matrix, double *returns, int n) {
    double *weights = (double *)malloc(n * sizeof(double));
    // double sum_returns = 0.0;
    double sum_inverse_cov = 0.0;

    for (int i = 0; i < n; i++) {
        // sum_returns += returns[i];
        sum_inverse_cov += 1.0 / cov_matrix[i][i];
    }

    for (int i = 0; i < n; i++) {
        weights[i] = (returns[i] / cov_matrix[i][i]) / sum_inverse_cov;
    }

    return weights;
}
