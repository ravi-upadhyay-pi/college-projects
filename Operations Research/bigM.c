
#include <stdio.h>
#include <math.h>
#define LENGTH 10
#define M 1000

int temp(float TEMP[LENGTH], int sum)
    {
    int i,c=0;
    for(i=0;i<sum;i++)
    {
        if( TEMP[i] < 0 && (int) TEMP[i] != -0)
            c = 1;
    }
    return c;
    }

int main()
{
    int i,j, bvar,var,sum,temp_1=0,temp_2=0, track[LENGTH];
    float A[LENGTH][LENGTH], B[LENGTH],Z[LENGTH],X[LENGTH],Y[LENGTH], min=9999, mulfac,divfac;
    printf("what is the number of Equations: ");
    scanf("%d",&bvar);

    printf(" number of variables in the equation?: ");
    scanf("%d",&var);
    
    sum = 2*bvar + var;
    
    for(i=0;i<=bvar;i++)
    {
        for(j=0;j<=sum;j++)
            A[i][j] = 0;
    }

    for(j=0;j<=sum;j++)
            Z[j] = 0;
    
    for(j=0;j<=bvar;j++)
            B[j] = 0;
    
    for(j=0;j<bvar;j++)
        track[j] = bvar + var + j + 1;

    printf("now sequentially give the coefficients of variables. \n");
   
    for(j=0;j<bvar;j++)
    {
        printf("\nenter for equation # %d : ",j+1);
        for(i=0;i<var;i++)
            scanf("%f",&A[j][i]);
    }
    
    for(i=0;i<bvar;i++)
    {
        A[i][var+i] = -1;
        A[i][var+bvar+i] = 1;
    }
    
    printf("Now enter the right hand side matrix : ");
    for(i=0;i<bvar;i++)
    {
        scanf("%f",&B[i]);
        A[i][sum] = B[i];
    }
    
    printf("\nwhat is  the coefficients of minimization objective function : ");
     for(i=0;i<var;i++)
    {
        scanf("%f",&Z[i]);
        A[bvar][i] = Z[i];
        Y[i] = Z[i];
    }
    
    for(i=0;i<bvar;i++)
        Z[bvar+ var+i] = M;

    for(i=0;i<bvar;i++)
    {
        for(j=0;j<=sum;j++)
            Z[j] = Z[j] - M*A[i][j];    
    }
    
    Z[sum] = 0;
    for(j=0;j<=sum;j++)
        A[bvar][j] = Z[j];


    for(i=0;i<=bvar;i++)
    {
        for(j=0;j<=sum;j++)
            printf("%0.2f\t",A[i][j]);
        printf("\n");
    }

    do{
        min = 9999;
        for(i=0;i<sum;i++)
        {
            if(Z[i] < min && Z[i] < 0)
            {
                min = Z[i];
                temp_1 = i;
            }
        }

        printf("entering variable : x[%d] \n",temp_1+1);
        
        min = 9999;
        for(i=0;i<bvar;i++)
        {    
            X[i] = A[i][sum] / A[i][temp_1];
            if(X[i] <= min && X[i] >= 0)
                {
                    min = X[i];
                    temp_2 = i;
                }
        }
        printf("leaving variable = x[%d]\n",track[temp_2]); 
        track[temp_2] = temp_1 + 1;
        
        divfac = A[temp_2][temp_1];
        for(i=0;i<=sum;i++)
            A[temp_2][i] = A[temp_2][i] / divfac;
    
        for(i=0;i<=bvar;i++)
        {
            if(i != temp_2)
            {    
                mulfac = A[i][temp_1];
                for(j=0;j<=sum;j++)
                    A[i][j] = A[i][j] - mulfac*A[temp_2][j];
            }
        }
        
        for(i=0;i<sum;i++)
            Z[i] = A[bvar][i];

        Z[sum] = 0;
        A[bvar][sum] = Z[sum];
    }while(temp(Z,sum) != 0);

    for(i=0;i<=bvar;i++)
    {
        for(j=0;j<=sum;j++)
            printf("%0.2f\t",A[i][j]);
        printf("\n");
    }

    for(i=0;i<bvar;i++)
    {
        for(j=0;j<bvar;j++)
        {
        if(track[j]-1 == i)
                Z[sum] =  Y[i] * A[j][sum] + Z[sum];
        }
    }

    printf("\nMin value of Z is : %f \n",Z[sum]);

    return 1;     
}

    

