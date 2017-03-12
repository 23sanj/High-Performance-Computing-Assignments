include<time.h>
#include<stdio.h>
#include <stdlib.h>

/* function returning the max between two numbers */
void matrix_generation(int n, double *matrix)
{
 int i,j;
 /* Creation of random matrices*/
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
     {
       matrix[i*n+j]= ((double)rand())/RAND_MAX;
 //      printf("matrix elements-%f\n, i=%d, j=%d",matrix[i*n+j],i,j);
     }
  }
 }


/* matrix multiplication*/
void dgemm0(int n,double *matrix_A,double *matrix_B,double *matrix_C)
{
/* dgemm0:simple ijk version triple loop algorithm*/
 int i,j,k;
 for(i=0;i<n;i++)
 {
  for(j=0;j<n;j++)
  {
    matrix_C[i*n+j] = 0.0;
    for(k=0;k<n;k++)
    {
     matrix_C[i*n+j]+=  matrix_A[i*n+k] * matrix_B[k*n+j];
//     printf("C1=%f\n, i=%d, j=%d",matrix_C[i*n+j],i,j);
    }
 //  printf("C1=%f\n, i=%d, j=%d",matrix_C[i*n+j],i,j);

  }
 }
}
void show(double * a,int size)
{
 int i,j;
 for(i=0;i<size;i++)
{
 for(j=0;j<size;j++)
  printf("%.2f ",a[i*size+j]);
 printf("\n");
}
}
void  dgemm1(int n,double *matrix_A,double *matrix_B,double *matrix_C)
{
 /* dgemm1: simple ijk version triple loop algorithm with register reuse*/
 int i,j,k;

 for(i=0;i<n;i++)
 {
  for(j=0;j<n;j++)
  {
   matrix_C[i*n+j] = 0.0;
   double tempR= matrix_C[i*n+j];
    for(k=0;k<n;k++)
    {
     tempR+=  matrix_A[i*n+k] * matrix_B[k*n+j];

    }
     matrix_C[i*n+j]=tempR;
   // printf("C2=%f\n, i=%d, j=%d",matrix_C[i*n+j],i,j);
  }
}

}

/*Comparing the output matrix by element by element basis*/
double compare(double *matrix_Cdgemm0, double *matrix_Cdgemm1, int n)

{ int i,j;
   int max_diff = matrix_Cdgemm0[0] - matrix_Cdgemm1[0];
 for ( i=0;i<n;i++)
 {
  for (j=0;j<n; j++)
  {
if((matrix_Cdgemm0[i*n+j] - matrix_Cdgemm1[i*n+j]) > max_diff);

         max_diff =matrix_Cdgemm1[i*n+j] - matrix_Cdgemm0[i*n+j];
    }
  }
  return max_diff;

  }

int main()
{
/* assign pointer for matrices A,B and C */
double *matrix_A,*matrix_B,*matrix_C1,*matrix_C2;
int i,j,k, n;

printf("input size of matrix\n");
scanf("%d",&n);

 int mem_space=n*n*sizeof(double);
matrix_A= (double*)malloc(mem_space);
matrix_B= (double*)malloc(mem_space);
matrix_C1= (double*)malloc(mem_space);
matrix_C2= (double*)malloc(mem_space);

 matrix_generation(n,matrix_A);
 matrix_generation(n,matrix_B);
 matrix_generation(n,matrix_C1);
 matrix_generation(n,matrix_C2);

//printf("A");
//show(matrix_A,n);

//printf("B");
//show(matrix_A,n);



double start_t = clock();
printf("start of the loop, start_t = %f\n", start_t);


dgemm0( n, matrix_A, matrix_B,matrix_C1);

//  dgemm1( n, matrix_A, matrix_B,matrix_C2);

/* end  timer*/

double  end_t = clock();
 printf("End of the loop, end_t = %f\n", end_t);
 /* calculate time taken for program execution*/
double t=(end_t-start_t);
 printf("t = %f\n", t);

double time_taken = (t/CLOCKS_PER_SEC);
printf("execution time:%f",time_taken);
dgemm0(n,matrix_A,matrix_B,matrix_C1);
//printf("C1");
//show(matrix_C1,n);

dgemm1(n, matrix_A, matrix_B,matrix_C2);
//printf("C2");
//show(matrix_C2,n);



int max_diff= compare(matrix_C1,matrix_C2,n);
//printf("maximum diferrence between the output matrix of both the algorithms:%d\n",max_diff);
if(max_diff==0)
{printf("verification successful for elements of matrices output by dgemm0 and dgemm1");}
return 0;
}

