#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/********************************************
var_count constr_count
objective function
constrainnts

eg. 
3 2				#3 variables	#2 constraints
1 2 3			#max z=x+2y+3z
1 0 0 3			#x=3
1 2 3 10		#x+2y+3z=10
********************************************/

typedef struct
{
	double	*obj;			//objective function c 
	double 	**constr;		//constrraints equation A|B (AX=B) X>=0
	int 	var_count;		//number of variables
	int 	constr_count;	//number of constrraints
	double	max;			//max obj
	double	type;			//maximization or minimization
	double	*solution;		//X s.t. c'X is maximized
}myLP;


void	solve(myLP*);
void	rowMult(double*, int, double);
void	rowSwap(double*, double*, int);
void	rowMultAdd(double*, double*, int, double);
int		rowReduce(double**, int, int);
void	permWorker(int, int, int, int, int*, char*, char**);
char**	permGen(int, int);
int		nCR(int, int);
myLP*	read(void);
void	printMatrix(double **A, int row, int col);

int main()
{
	int i;
	myLP *LP;
	LP=read();
	solve(LP);
	printf("max = %.1lf\n", LP->max);
	for(i=0; i<LP->var_count; i++)
		printf("x%d = %.1lf\n", i, LP->solution[i]);
	return 0;
}

void solve(myLP *LP)
{
	double **temp, obj;
	int i, j, k, l, n, count;
	char **perm;
	
	LP->constr_count=rowReduce(LP->constr, LP->constr_count, LP->var_count+1);
	n=LP->var_count;
	temp=(double**)malloc(sizeof(double*)*n);
	for(i=0; i<n; i++)
		temp[i]=(double*)malloc(sizeof(double)*(n+1));
	
	perm=permGen(n, LP->constr_count); //n items in row, LP->constr_count 1's in one row
	count=nCr(n, LP->constr_count);
	LP->max=0;
	LP->solution=(double*)malloc(sizeof(double)*n);
	
	for(i=0; i<count; i++){
		//reset temp matrix
		for(j=0; j<LP->constr_count; j++)
			for(k=0; k<n+1; k++)
				temp[j][k]=LP->constr[j][k];
		for(j=LP->constr_count ; j<n; j++)
			for(k=0; k<n+1; k++)
				temp[j][k]=0;
				
		k=LP->constr_count;
		for(j=0; j<n; j++)
			if(perm[i][j]=='0')
				temp[k++][j]=1;
		k=rowReduce(temp, n, n+1);
		if(k!=n)
			continue;
		obj=0;
		for(j=0; j<n; j++)
			obj+=temp[j][n]*(LP->obj[j]);
		if(obj>LP->max){
			LP->max=obj;
			for(j=0; j<n; j++)
				LP->solution[j]=temp[j][n];
		}
	}
		
}

void rowMult(double *Row, int n, double a)
{
	int i;
	for(i=0; i<n; i++)	Row[i]*=a;
}

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

void rowMultAdd(double *A, double *B, int n, double a)
{
	int i;
	for(i=0; i<n; i++)	A[i]+=a*B[i];
}

int rowReduce(double **A, int row, int col)
{
	int i, j, k;
	double Epsilon=0.001;
	for(i=0, k=0; k<row && k<col; k++){
		j=i;
		while( j<row && fabs(A[j][k])<Epsilon )	
			j++;
		if(j==row)	
			continue;
		if(j!=i)	
			rowSwap(A[i], A[j], col);
		rowMult(A[i], col, 1/A[i][k]);
		for(j=0; j<row; j++){
			if(j==i) continue;
			if(fabs(A[j][k])<Epsilon) continue;
			rowMultAdd(A[j], A[i], col, -A[j][k]);
		}
		i++;
	}
	return i;
}

void permWorker(int n, int r, int curr_n, int curr_r, int *count, char *arr, char **all_perm)
{
	if(curr_n==n){	
		int i;
		all_perm[count[0]]=(char*)malloc(sizeof(char)*n);
		for(i=0; i<n; i++)
			all_perm[count[0]][i]=arr[i];
		count[0]+=1;
		return;
	}
	if(n-curr_n==r-curr_r && r-curr_r>0){
		arr[curr_n]='1';
		permWorker(n, r, curr_n+1, curr_r+1, count, arr, all_perm);
	}
	else if(r==curr_r){
		arr[curr_n]='0';
		permWorker(n, r, curr_n+1, curr_r, count, arr, all_perm);
	}
	else{
		arr[curr_n]='0';
		permWorker(n, r, curr_n+1, curr_r, count, arr, all_perm);
		arr[curr_n]='1';
		permWorker(n, r, curr_n+1, curr_r+1, count, arr, all_perm);
	}
}

char** permGen(int n, int r) 
{
	char *arr, **all_perm; 
	int *count;
	count=(int*)malloc(sizeof(int));
	count[0]=0;
	arr=(char*)malloc(sizeof(char)*n);
	all_perm=(char**)malloc(sizeof(char*)*nCr(n,r));
	permWorker(n, r, 0, 0, count, arr, all_perm);
	return all_perm;
}

int nCr(int n, int r)
{
	int C=1, i;
	if(r>n/2) r=n-r/2;
	for(i=1; i<=r; i++)
		C*=(n-i+1);
	for(i=1; i<=r; i++)
		C/=i;
	return C;
}
	
myLP* read(void)
{
	int i, j;
	myLP *LP=(myLP*)malloc(sizeof(myLP));
	
	scanf("%d %d", &LP->var_count, &LP->constr_count);
	
	LP->obj=(double*)malloc(sizeof(double)*(LP->var_count));
	LP->constr=(double**)malloc(sizeof(double*)*(LP->constr_count));
	
	for(i=0; i<(LP->var_count); i++)
		scanf("%lf", &(LP->obj[i]));
		
	for(i=0; i<(LP->constr_count); i++){
		LP->constr[i]=(double*)malloc(sizeof(double)*(LP->var_count+1));
		for(j=0; j<(LP->var_count+1); j++)
			scanf("%lf", &(LP->constr[i][j]));
	}
	return LP;
}

void printMatrix(double **A, int row, int col)
{
	int i, j;
	for(i=0; i<row; i++){
		for(j=0; j<col; j++)	printf("%.2lf\t", A[i][j]);
		printf("\n");
	}
}
	

