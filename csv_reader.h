#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 数据结构声明
typedef struct {
    char **index;
    char **colnames;
    double **data;
    int rows;
    int cols;
} CSVData;

// 函数声明
void remove_newline(char *str);
CSVData* read_csv(const char *filename);
void free_csv_data(CSVData *csv_data);

#endif // CSV_READER_H
