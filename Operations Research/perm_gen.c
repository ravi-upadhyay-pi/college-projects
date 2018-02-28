#include <stdio.h>
#include <stdlib.h>


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
		perm_worker(n, r, curr_n+1, curr_r+1, count, arr, all_perm);
	}
	else if(r==curr_r){
		arr[curr_n]='0';
		perm_worker(n, r, curr_n+1, curr_r, count, arr, all_perm);
	}
	else{
		arr[curr_n]='0';
		perm_worker(n, r, curr_n+1, curr_r, count, arr, all_perm);
		arr[curr_n]='1';
		perm_worker(n, r, curr_n+1, curr_r+1, count, arr, all_perm);
	}
}

char** permGenerator(int n, int r) 
{
	char *arr, **all_perm; 
	int *count;
	count=(int*)malloc(sizeof(int));
	count[0]=0;
	arr=(char*)malloc(sizeof(char)*n);
	all_perm=(char**)malloc(sizeof(char*)*nCr(n,r));
	perm_worker(n, r, 0, 0, count, arr, all_perm);
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
	

	
	int main()
{
	int i, j;
	char **ch;
	ch=perm_Generator(5,2);
	for(i=0; i<10; i++){
		for(j=0; j<5; j++){
			printf("%c\t", ch[i][j]);
		}
		printf("\n");
	}
	return 0;
}