///contains all of the needed function declarations
#include <stdio.h>
#include "neuralnet.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

void deepcopy(int rows, int cols, int m[][cols], int newm[][cols]){
    for(int row=0;row<rows;row++){
        for(int col=0;col<cols;col++){
            newm[row][col] = m[row][col];
        }
    }
}

void vector_matrix(int size, double matrix[][size], double vector[], double newvec[]){
    for(int row=0;row<NUM_DATA_SETS;row++){
        for(int col=0;col<NUM_DATA_SETS;col++){
            newvec[row]+=matrix[row][col]*vector[row];
        }
    }
}

void matrix_mult(int size1, int matrix1[][size1], int size2, double matrix2[][size2], double result[][size2]){
    for(int row=0;row<NUM_DATA_SETS;row++){
        for(int col=0;col<NUM_DATA_SETS;col++){
            printf("Row:%d, Col:%d\n",row,col);
            for(int inner = 0;inner<LEN_DATA;inner++){
                printf("(%d * %f)+",matrix1[row][inner],matrix2[inner][col]);
                result[row][col] += matrix1[row][inner] * matrix2[inner][col];
            }
            printf("\nResult: %f\n", result[row][col]);
        }
    }
}

void nonlinearity(int size, double table[][size]){
    for(int row=0;row<NUM_DATA_SETS;row++){
        for(int col=0;col<size;col++){
            table[row][col] = 1/(1+exp(-table[row][col]));
        }
    }
}

void nonlinearityVector(double vector[]){
    for(int i=0;i<NUM_DATA_SETS;i++){
        vector[i]=1/(1+exp(-vector[i]));
    }
}

void nonlinearityprime(int size, double table[][size]){
    for(int row=0;row<NUM_DATA_SETS;row++){
        for(int col=0;col<size;col++){
            table[row][col] = table[row][col]*(1-table[row][col]);
        }
    }
}

