#include<stdio.h>
#include<stdlib.h>
#include<athread.h>

#define NN 64
#define NM 4

typedef struct
{
	double *A;
	double *B;
	int row;
	int col;

} Param;


extern SLAVE_FUN(func_reg)(Param *);



int main() {

	int i, j;

	const int row = NN;
	const int col = NM;

	double *A_temp = (double *)malloc((row * col) * sizeof(double));
	double **A = (double **)malloc(row * sizeof(double *));
	for (i = 0 ; i < row ; i ++) {
		A[i] = &A_temp[i * col];
	}

    double *B_temp = (double *)malloc((row * col) * sizeof(double));
	double **B = (double **)malloc(row * sizeof(double *));
	for (i = 0 ; i < row ; i ++) {
		B[i] = &B_temp[i * col];
	}

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			//A[i][j] = i + 0.1;
			A[i][j] = i/8;
			B[i][j] = 0.0;
		}
	}

	for (i = 0; i < row; i++) {
       if(i%8==0)
            printf("row: %d   ", i/8);
		for (j = 0; j < 1; j++) {
			printf("%f  ", A[i][j]);
		}
        if((i+1)%8==0)
            printf("\n");
	}

	Param host;

	host.A = &A[0][0];
	host.B = &B[0][0];
	host.row = row;
	host.col = col;

	athread_init();
	athread_spawn(func_reg, &host);
	athread_join();
	athread_halt();

	printf("*******************************************************************************\n");

	for (i = 0; i < row; i++) {
       if(i%8==0)
            printf("row: %d   ", i/8);
		for (j = 0; j < 1; j++) {
			printf("%f  ", A[i][j]);
		}
        if((i+1)%8==0)
            printf("\n");
	}

	return 0;


}



