#include <stdio.h>
#include <math.h>
#include <stdio.h>

#define MAX 100
#define epsilon 0.001

int var_count, constr_count, pivot_row, pivot_col;
double constr[MAX][MAX], objective[MAX], simplex[MAX][MAX];

int doubleEqual(double a, double b)
{
	double max=a;
	if(max<b) max=b;
	if(fabs(a-b)<epsilon*fabs(max))
		return 1;
	return 0;
}

void printMatrix(int row, int col)
{
	int i, j;
	for(i=0; i<row; i++){
		for(j=0; j<col; j++)	printf("%.2lf\t", simplex[i][j]);
		printf("\n");
	}
	printf("\n");
}

void readData(void)
{
	int i, j;
	scanf("%d %d", &var_count, &constr_count);
	for(i=0; i<var_count; i++)
		scanf("%lf", &objective[i]);
	for(i=0; i<constr_count; i++)
		for(j=0; j<var_count+1; j++)
			scanf("%lf", &constr[i][j]);
}

void createTable(void)
{
	int i, j;
	for(i=0; i<=var_count; i++)
		simplex[0][i]=i;
	for(i=1; i<=constr_count; i++)
		simplex[i][0]=var_count+i;
	
	for(i=1; i<=constr_count; i++)
		for(j=1; j<=var_count+1; j++)
			simplex[i][j]=constr[i-1][j-1];
	
	for(i=1; i<=var_count; i++)
		simplex[constr_count+1][i]=-1*objective[i-1];
	simplex[constr_count+1][var_count+1]=0;
}


/*****************************
1 if pivot found
2 if no pivot exists
3 if unbounded
*****************************/
int findPivot(void)
{
	int i, m, n;
	double min_value=0.0, min_ratio;
	pivot_row=0;
	pivot_col=0;
	m=constr_count;
	n=var_count;
	printMatrix(constr_count+1, var_count+1);
	for(i=1; i<=n; i++){
		if(simplex[m+1][i]<min_value){
			pivot_col=i;
			min_value=simplex[m+1][i];
		}
	}
	if(pivot_col==0)
		return 2;
	for(i=0; i<=m; i++){
		if(simplex[i][pivot_col]>0 && simplex[i][pivot_col]/simplex[i][n+1]<min_ratio){
			pivot_row=i;
			min_ratio=simplex[i][pivot_col]/simplex[i][n+1];
		}
	}
	if(pivot_row==0)
		return 3;
}


void newTable(void)
{
	int i, j, m, n;
	double p, q, r, s;
	m=constr_count;
	n=var_count;
	for(i=1; i<m+1; i++){
		if(i==pivot_row) continue;
		simplex[i][pivot_col]*=(-1/simplex[pivot_row][pivot_col]);
	}

	for(i=1; i<n+1; i++){
		if(i==pivot_col) continue;
		simplex[pivot_row][i]*=(-1/simplex[pivot_row][pivot_col]);
	}
	simplex[pivot_row][pivot_col]=1/simplex[pivot_row][pivot_col];
	p=simplex[pivot_row][pivot_col];
	for(i=1; i<=m+1; i++){
		if(i==pivot_row) continue;
		for(j=1; j<=n+1; j++){
			if(j==pivot_col) continue;
			q=simplex[i][j];
			r=simplex[i][pivot_col];
			s=simplex[pivot_row][j];
			simplex[i][j]=((p*q)-(r*s))/p;
		}
	}
}

//1 if solution exists. 2 if unbounded
int Simplex(void)
{
	int i;
	while(findPivot()==1)
		newTable();
	if(i==3) return 2;
	if(i==2) return 1;
}

void showResult(void)
{
	int i, m, n;
	m=constr_count;
	n=var_count;
	double values[MAX];
	for(i=1; i<=var_count; i++)
		values[(int)simplex[0][i]]=0;
	for(i=1; i<=constr_count; i++)
		values[(int)simplex[i][0]]=simplex[i][var_count+1];
	printf("max Z: = %.2lf\nThe Solution is\n", simplex[m+1][n+1]);	
	for(i=1; i<var_count; i++)
		printf("x%d = %.2lf\n", i, values[i]);
}

int main()
{
	readData();
	createTable();
	if(Simplex()==1)
		showResult();
	else
		printf("Unbounded\n");
	return 0;
}