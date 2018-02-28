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

void revisedSimplex(double* obj, double** table, int var, int constr);

int main()
{
	int var, constr, i, j;
	double *obj, **table;
	scanf("%d %d", var, constr);
	
	obj=(double*)malloc(sizeof(double)*(var+constr+1));
	for(i=0; i<var; i++)
		scanf("%lf", &obj[i]);
	
	for(i=var; i<var+constr+1; i++)
		obj[i]=0;
	
	table=(double**)malloc(sizeof(double*)*constr);
	for(i=0; i<constr; i++){
		table[i]=(double*)malloc(sizeof(double)*(var+constr+1));
		for(j=0; j<var; j++)
			scanf("%lf", &table[i][j]);
		for(j=var; j<var+constr; j++)
			table[i][j]=0;
		scanf("%lf", &table[i][var+constr]);
	}
	for(i=0; i<constr; i++)
		table[i][i+var]=1;
		
	revisedSimplex(obj, table, var, constr);
	
	return 0;
}

void revisedSimplex(double* obj, double** table, int var, int constr)
{
	int i, j, *basic, *nonbasic;
	double **B, **BinverseA, *CB, *newObjective, **temp;
	
	basic=(int*)malloc(sizeof(int)*constr);					//basic variables
	B=(double**)malloc(sizeof(double*)*constr);				//A matrix corresponding basic variables 
	CB=(double*)malloc(sizeof(double)*constr);				//objective corresponding basic variables
	newObjective=(double*)malloc(sizeof(double)*(var+1));	//new objective function w.r.t non-basic variables
	nonbasic=(int*)malloc(sizeof(int)*var);			//non-basic variables
	
	
	//constructing basic, objective, basic matrix
	for(i=0; i<constr; i++){
		basic[i]=var+i;
		CB[i]=obj[basic[i]];
		B[i]=(double*)malloc(sizeof(double)*constr);
		for(j=0; j<constr; j++){
			B[i][j]=A[i][basic[j]];
		}
	}
	
	//constructing non-basic variables
	for(i=0; i<var; i++)
		nonbasic[i]=i;
		
	BinverseA=multMat(matInverse(B, constr), A, constr, constr, var+constr+1);
	while(1){
		int leaving=-1, entering=-1;
		//find entering variable
		//find newObjectivefunction
		temp=mulMat(CB, BinverseA, 1, constr, var+constr+1);
		temp[var+constr]*=-1;
		for(i=0; i<var+constr+1; i++)
			temp[i]=obj[i]-temp[i];
		for(i=0; i<var; i++){
			if(temp[nonbasic[i]]>0){
				if(entering==-1)
					entering=nonbasic[i];
				else
					if(temp[nonbasic[i]>temp[entering])
						entering=nonbasic[i];
			}
		}
		if(entering==-1){
			printf("optimum found\n");
			return;
		}
		
		//find leaving variable
		for(i=0; i<constr; i++){
			if(BinverseA[i][entering]>0){
				if(leaving==-1) leaving=basic[i];
				else
					if( (BinverseA[i][var+constr])/(BinverseA[i][entering]) < (BinverseA[basic[leaving]][var+constr])/(BinverseA[basic[leaving][entering]))
						leaving=i;
			}
		}
		if(leaving==-1){
			printf("Unbounded\n");
			return;
		}
		
		for(i=0; i<var; i++){
			if(nonbasic[i]==entering){
				nonbasic[i]=leaving;
				i=var;
			}
		}
		
		for(i=0; i<constr; i++){
			if(basic[i]==leaving){
				basic[i]=entering;
				i=constr;
			}
		}
		
		for(i=0; i<constr; i++){
			for(j=0; j<constr; j++){
				B[i][j]=A[i][basic[i]];
			}
		}
		BinverseA=multMat(matInverse(B, constr), A, constr, constr, var+constr+1);
		for(i=0; i<constr; i++)
			CB[i]=obj[basic[i]];
		
}
