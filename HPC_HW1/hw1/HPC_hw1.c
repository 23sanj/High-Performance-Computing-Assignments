#include<time.h>
#include<stdio.h>
#include <stdlib.h>
int main()
{
/* assign pointer for matrices A,B and C */
float *matrix_A,*matrix_B,*matrix_C;
int i,j,k, n;

printf("input size of matrix\n");
scanf("%d",&n);

 int mem_space=n*n*sizeof(float);

/*dynamic memory allocation*/

matrix_A= (float*)malloc(mem_space);
matrix_B= (float*)malloc(mem_space);
matrix_C= (float*)malloc(mem_space);

/* Creation of random matrices*/
for(i=0;i<n;i++)
{
  for(j=0;j<n;j++)
  {
  matrix_A[i*n+j]= ((float)rand())/RAND_MAX;
  matrix_B[i*n+j]= ((float)rand())/RAND_MAX;
 // printf("A=%f",matrix_A[i*n+j]);
 // printf("B=%f",matrix_B[i*n+j]);
  }
}
 
 //time_t t;
   
 //  n = 5;
   
   /* Intializes random number generator */
  // srand((unsigned) time(&t));

   /* Print 5 random numbers from 0 to 50 */
/*   for( i = 0 ; i < n ; i++ ) 
   {
      printf("%f\n", ((float)rand())/RAND_MAX);
   }
*/

/* start timer*/
 clock_t start_t, end_t, total_t;
  

   start_t = clock();
   printf("Starting of the program, start_t = %ld\n", start_t);
    
   printf("Going to scan a big loop, start_t = %ld\n", start_t);
  // for(i=0; i< 10000000; i++)
  // {
  // }
// start=clock();//predefined  function in c


/* matrix multiplication*/
/* dgemm0:simple ijk version triple loop algorithm*/
/*for(i=0;i<n;i++)
{
  for(j=0;j<n;j++)
  {
    for(k=0;k<n;k++)
    {
     matrix_C[i*n+j]+=  matrix_A[i*n+k] * matrix_B[k*n+j];
    // printf("C=%f",matrix_C[i*n+j]);
    }
  }
}
*/
 /* dgemm1: simple ijk version triple loop algorithm with register reuse*/
/* dgemm0:simple ijk version triple loop algorithm*/
for(i=0;i<n;i++)
{
  for(j=0;j<n;j++)
  {
   double tempR= matrix_C[i*n+j];
    for(k=0;k<n;k++)
    {
     tempR+=  matrix_A[i*n+k] * matrix_B[k*n+j];
     
    }
     matrix_C[i*n+j]=tempR;
    // printf("C=%f",matrix_C[i*n+j]);
  }
}



/* end  timer*/

//after the user defined function does its work
//end=clock();
end_t = clock();
   printf("End of the big loop, end_t = %ld\n", end_t);

 /* calculate time taken for program execution*/
long double t=(end_t-start_t);
//print("time taken for program execution=%ld",t);
//end_t = clock();
//   printf("End of the big loop, end_t = %ld\n", end_t);
double time_taken = ((double)t)/CLOCKS_PER_SEC;
printf("execution time:%f",time_taken);
//total_t = (long double)(end_t - start_t) / CLOCKS_PER_SEC;
//printf("Total time taken by CPU: %f\n", (total_t*(10^20))  );

return 0;
}
