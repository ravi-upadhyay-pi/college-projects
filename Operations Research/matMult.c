#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Epsilon 0.0001


void printMatrix(double **A, int row, int col);
void rowMult(double *Row, int n, double a);
void rowSwap(double *A, double *B, int n);
void rowMultAdd(double *A, double *B, int n, double a);
int  rowReduce(double **A, int row, int col);
double** inverse(double **A, int n);


/*******************
Row = a*Row
Row as n elements
*******************/
void rowMult(double *Row, int n, double a)
{
	int i;
	for(i=0; i<n; i++)	Row[i]*=a;
}

/*******************
A and B are swapped
They have n elements
*******************/
void rowSwap(double *A, double *B, int n)
{
	double temp;
	int i;
	for(i=0; i<n; i++){
		temp=A[i];
		A[i]=B[i];
		B[i]=temp;
	}
}

/*******************
A=A+a*B
They have n elements
*******************/
void rowMultAdd(double *A, double *B, int n, double a)
{
	int i;
	for(i=0; i<n; i++)	A[i]+=a*B[i];
}

double** multMat(double **A, double **B, int p, int q, int r)
{
	int i, j, k;
	double temp, **mult;
	mult=(double**)malloc(sizeof(double*)*p);
	for(i=0; i<p; i++){
		mult[i]=(double*)malloc(sizeof(double)*r);
		for(j=0; j<r; j++){
			temp=0;
			for(k=0; k<q; k++){
				temp+=A[i][k]*B[k][j];
			}
			mult[i][j]=temp;
		}
	}
	return mult;
}

/*******************************************
Changes A to row reduced echleon form.
Returns number of non-zero rows.
*******************************************/
int rowReduce(double **A, int row, int col)
{
	int i, j, k;
	for(i=0, k=0; k<row && k<col; k++){
		j=i;
		while( j<row && fabs(A[j][k])<Epsilon )	j++;
		if(j==row)	continue;
		if(j!=i)	rowSwap(A[i], A[j], col);
		rowMult(A[i], col, 1/A[i][k]);
		for(j=0; j<row; j++){
			if(j==i) continue;
			rowMultAdd(A[j], A[i], col, -A[j][k]);
		}
		i++;
	}
	return i;
}

/********************************************
Returns Inverse of Matrix A in a new Matrix
If Inverse D.N.E. then it returns NULL
*********************************************/
double** matInverse(double **A, int n)
{
	double **Inv, **retMat;
	int i, j;
	Inv=(double**)malloc(sizeof(double*)*n);
	for(i=0; i<n; i++){
		Inv[i]=(double*)malloc(sizeof(double)*2*n);
		for(j=0; j<n; j++)	Inv[i][j]=A[i][j];
		for(j=n; j<2*n; j++)	Inv[i][j]=0.0;
		Inv[i][n+i]=1.0;
	}
	rowReduce(Inv, n, 2*n);
	if(fabs(Inv[n-1][n-1]-1)<Epsilon){
		retMat=(double**)malloc(sizeof(double)*n);
		for(i=0; i<n; i++){
			retMat[i]=(double*)malloc(sizeof(double)*n);
			for(j=0; j<n; j++)	retMat[i][j]=Inv[i][j+n];
			free(Inv[i]);
		}
		free(Inv);
		return retMat;
	}
	for(i=0; i<n; i++)	free(Inv[i]);
	free(Inv);
	return NULL;
}

void printMatrix(double **A, int row, int col)
{
	int i, j;
	for(i=0; i<row; i++){
		for(j=0; j<col; j++)	printf("%.2lf\t", A[i][j]);
		printf("\n");
	}
	printf("\n");
}


int main()
{
	double **A, **B;
	int row, col, row2, col2, i, j;
	scanf("%d %d", &row, &col);
	A=(double**)malloc(sizeof(double*)*row);
	for(i=0; i<row; i++){
		A[i]=(double*)malloc(sizeof(double)*col);
		for(j=0; j<col; j++){
			scanf("%lf", &A[i][j]);
		}
	}
	scanf("%d %d", &row2, &col2);
	B=(double**)malloc(sizeof(double*)*row2);
	for(i=0; i<row2; i++){
		B[i]=(double*)malloc(sizeof(double)*col2);
		for(j=0; j<col; j++){
			scanf("%lf", &B[i][j]);
		}
	}
	
	printMatrix(multMat(A, B, row, col, col2), row, col2);
	return 0;
}