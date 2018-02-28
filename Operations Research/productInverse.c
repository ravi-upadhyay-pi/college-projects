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


int main()
{	
	int n, i, j, k;
	double **M, **M_temp, ***E, **temp;
	scanf("%d", &n); 
	M=(double**)malloc(sizeof(double*)*n);
	temp=(double**)malloc(sizeof(double*)*n);
	E=(double***)malloc(sizeof(double**)*n);
	M_temp=(double**)malloc(sizeof(double*)*n);

	//temp is identity 
	//M is read
	//M_temp is identity
	for(i=0; i<n; i++){
		temp[i]=(double*)malloc(sizeof(double)*n);
		M[i]=(double*)malloc(sizeof(double)*n);
		M_temp[i]=(double*)malloc(sizeof(double)*n);
		for(j=0; j<n; j++){
			temp[i][j]=0;
			M_temp[i][j]=0;
			scanf("%lf", &M[i][j]);
		}
		temp[i][i]=1;
		M_temp[i][i]=1;
	}
	
	
		
	//M temp is prepared
	//1st col is copied
	for(i=0; i<n; i++){
		M_temp[i][0]=M[i][0];
	}
	
	
	//loop for finding E is started
	for(i=0; i<n; i++){
		printf("Matrix %d\n", n-i-1);
		printMatrix(M_temp, n, n);
		printMatrix(temp, n, n);
		E[n-i-1]=multMat(M_temp, matInverse(temp, n), n, n, n);	//En-1 ^-1
		printMatrix(E[n-i-1], n, n);
		E[n-i-1]=matInverse(E[n-i-1], n);
		printMatrix(E[n-i-1], n, n);
		for(j=0; j<n; j++){
			for(k=0; k<n; k++){
				temp[j][k]=M_temp[j][k];
			}
		}
		//updating M_temp
		for(j=0; j<n; j++){
			M_temp[j][i+1]=M[j][i+1];		
		}
	}
	
	//temp=Identity;
	for(i=0; i<n; i++){
		for(j=0; j<n; j++)
			temp[i][j]=0;
			temp[i][i]=1;
	}
	for(i=0; i<n; i++){
		temp=multMat(E[i], temp, n, n, n);
	}
	printMatrix(temp, n, n);
	
	return 0;
}