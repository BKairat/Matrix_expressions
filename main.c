
//
//  main.c
//  test
//
//  Created by Bekbolinov Kairat on 3/4/22.
//
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
 
typedef struct matrix{
    int row;
    int col;
    int *data;
} matrix;
 
void print_matrix(const matrix *const mat);
static matrix *allocate_matrix(void);
void free_matrix(matrix **mat);
bool input_matrix_dim(int *row, int *col);
bool read_matrix(int row, int col, int matrix[row][col]);
matrix* input_matrix(void);
matrix* mult(const matrix *const mat1, const matrix *const mat2);
matrix* sum(const matrix *const mat1, const matrix *const mat2, int factor);
char get_op(void){
    char op[2] = {'\0'};
    if (scanf("%1s", op) != 1 || !(op[0] == '-'||op[0] == '+' ||op[0] == '*')){
    }
    return op[0];
}
 
//==============================================================
//==============================================================
//==============================================================
 
 
int main() {
    matrix *m1 = input_matrix();
    if (m1 == NULL){
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
    char op = get_op();
    if (!(op == '-' || op == '+' || op == '*')){
        free_matrix(&m1);
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
    matrix *m2 = input_matrix();
    if (m2 == NULL){
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
    matrix *result = NULL;
 
    if (op == '-'){
        result = sum(m1,m2,-1);
    } else if (op == '+'){
        result = sum(m1,m2,1);
    }else if (op == '*'){
        result = mult(m1,m2);
    }
 
    if (result == NULL){
        free_matrix(&m2);
        free_matrix(&m1);
        return 100;
    }
    printf("\nsolution:\n\n");
    print_matrix(result);
    free_matrix(&m2);
    free_matrix(&m1);
    free_matrix(&result);
    return 0;
}
 
 
//==============================================================
//==============================================================
//==============================================================
 
 
void print_matrix(const matrix *const mat){
    printf("%d %d\n", mat->row, mat->col);
    int i = 0;
    for (int r = 0; r < mat ->row; ++r){
        for (int c = 0; c < mat->col; ++c){
            printf("%d", mat->data[i]);
            if ((c+1) < mat->col) printf(" ");
            ++i;
        }
        printf("\n");
    }
}
 
static matrix *allocate_matrix(void){
    matrix* mat = malloc(sizeof(matrix));
    if (mat == NULL){
        fprintf(stderr, "Memory allocation error!\n");
        exit(-1);
    }
    mat->row = mat->col = 0;
    mat->data = NULL;
    return mat;
}
 
void free_matrix(matrix **mat){
    if ((*mat)->data) free ((*mat)->data);
    free(*mat);
    *mat = NULL;
}
 
bool input_matrix_dim(int *row, int *col){
    return scanf("%d %d", row, col) == 2;
}
 
bool read_matrix(int row, int col, int matrix[row][col]){
    for (int r = 0; r < row; ++r)
        for (int c = 0; c < col; ++c)
            if (scanf("%d", &(matrix[r][c])) != 1)
                return false;
    return true;
}
 
matrix* input_matrix(void){
    matrix* mat = allocate_matrix();
     
    if (!input_matrix_dim(&(mat->row),&(mat->col))){
        free_matrix(&mat);
        return NULL;
    }
    mat->data = malloc(sizeof(int)*mat->row*mat->col);
    if (!read_matrix(mat->row, mat->col, (int(*)[mat->col])mat->data)){
        free_matrix(&mat);
        mat = NULL;
    }
    return mat;
}
 
matrix* sum(const matrix *const mat1, const matrix *const mat2, int factor){
    if((mat1->col != mat2->col) || (mat1->row != mat2->row)){
        fprintf(stderr, "Error: Chybny vstup!\n");
        return NULL;
    }
    matrix* mat = allocate_matrix();
    mat->row = mat1->row;
    mat->col = mat1->col;
    mat->data = malloc(sizeof(int)*mat->row*mat->col);
    for (int i = 0; i < mat->row*mat->col; ++i){
        mat->data[i] = mat1->data[i]+((factor)*mat2->data[i]);
    }
    return mat;
}
 
matrix* mult(const matrix *const mat1, const matrix *const mat2){
 
    if(mat1->col != mat2->row){
        fprintf(stderr, "Error: Chybny vstup!\n");
        return NULL;
    }
    matrix* mat = allocate_matrix();
    mat->row = mat1->row;
    mat->col = mat2->col;
    mat->data = malloc(sizeof(int)*mat->row*mat->col);
    int r = 0;
    int c = 0;
    int s = 0;
    int top = 0;
    for (int i = 0; i < mat->row*mat->col; ++i){
        for (int j = 0; j < mat1->col; ++j){
            r += j;
            c += j*(mat2->col);
            s += mat1->data[r]*mat2->data[c];
            r -= j;
            c -= j*(mat2->col);
        }
        mat->data[top] = s;
        s = 0;
        ++c;
        if (c == mat2->col){
            c = 0;
            r += mat1->col;
        }
        ++top;
    }
    return mat;
}
