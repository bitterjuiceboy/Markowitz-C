#include "csv_reader.h"

void remove_newline(char *str) {
    char *pos;
    if ((pos = strchr(str, '\n')) != NULL) {
        *pos = '\0';
    }
}

// 读取CSV文件
CSVData* read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int row = 0;
    CSVData *csv_data = malloc(sizeof(CSVData));
    csv_data->index = NULL;
    csv_data->colnames = NULL;
    csv_data->data = NULL;
    csv_data->rows = 0;
    csv_data->cols = 0;

    // 读取第一行（列名）
    if (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ",");
        int col = 0;

        while (token) {
            remove_newline(token);  // 最后一个colname会把\n也读进去，这里检测并删除\n.
            csv_data->colnames = realloc(csv_data->colnames, (col + 1) * sizeof(char *));
            csv_data->colnames[col] = strdup(token);
            token = strtok(NULL, ",");
            col++;
        }

        csv_data->cols = col - 1; // 第一个是索引列，不计算在内
    }

    // 读取数据行
    while (fgets(line, sizeof(line), file)) {
        // 使用realloc调整行大小
        csv_data->data = realloc(csv_data->data, (row + 1) * sizeof(double *));
        csv_data->data[row] = NULL;
        csv_data->index = realloc(csv_data->index, (row + 1) * sizeof(char *));

        // 解析索引列（日期）
        char *token = strtok(line, ",");
        csv_data->index[row] = strdup(token);

        // 解析数据列
        int col = 0;
        token = strtok(NULL, ",");
        while (token) {
            csv_data->data[row] = realloc(csv_data->data[row], (col + 1) * sizeof(double));
            csv_data->data[row][col] = atof(token);
            token = strtok(NULL, ",");
            col++;
        }

        row++;
    }

    fclose(file);

    csv_data->rows = row; // 更新行数
    return csv_data;
}

void free_csv_data(CSVData *csv_data) {
    if (csv_data) {
        for (int i = 0; i < csv_data->rows; i++) {
            free(csv_data->index[i]);
            free(csv_data->data[i]);
        }
        for (int i = 0; i < csv_data->cols + 1; i++) {
            free(csv_data->colnames[i]);
        }
        free(csv_data->index);
        free(csv_data->data);
        free(csv_data->colnames);
        free(csv_data);
    }
}
