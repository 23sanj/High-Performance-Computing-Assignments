/* Sieve of eratosthenes */
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



#define MIN(a,b)  ((a)<(b)?(a):(b))

#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)   (BLOCK_LOW(((id)+1),p,n)-1)
#define BLOCK_SIZE(id,p,n)   ((BLOCK_LOW(((id)+1),p,n))-(BLOCK_LOW(id,p,n)))
#define BLOCK_OWNER(index,p,n)   (((p)*(index)+1)-1)/(n))

int main (int argc, char *argv[])
{
   int count;		/* local prime count	*/
   double elapsed_time; /* execution time	*/
   int first, first1;		/* index of the first sieve	*/
   int global_count;	/* global count of prime numbers */
   int high_value; 	/* highest value assigned to this process */
   int i;		/* loop counter		*/
   int id;		/* this process id	*/
   int index;		/* index of the current sieve	*/
   int low_value, low_value1;	/* lowest value assigned to this process */
   char *marked, *marked1;		/* array elements to be  marked	*/
   int  n;		/* value of the largest number	*/
   int p; 		/* number of processes		*/
   int  proc0_size;	/* number of elements assigned to process zero */
			/* this is to find if process zero has all primes */
   int  prime, prime1;		/* current prime or sieve	*/
   int  size;		/* elements in marked array 	*/
   char *primeArray *newprime;
   int PrimeArray_size;
 
   MPI_Init (&argc, &argv);
	/* start timer */
   MPI_Barrier(MPI_COMM_WORLD);
   elapsed_time = -MPI_Wtime();
   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   MPI_Comm_size (MPI_COMM_WORLD, &p);

if (argc != 2) 
{
	if (!id) 
		printf ("Command line: %s <m>\n", argv[0]);
      
	MPI_Finalize(); 
	exit (1);
}

   n = atoi(argv[1]);
	//find how many elements are assigned to this process 
   low_value = 2*(BLOCK_LOW(id,p,(n/2) - 1)) + 3;
   high_value = 2*(BLOCK_HIGH(id,p,(n/2) - 1)) + 3;
   size = ((high_value-low_value)/ (2)) + 1;
   proc0_size = ((((n)/(2))-1)/p);
   PrimeArray_size=(int )(( sqrt((double) n))/2);
   int start = 3;
/*modification*/
for (i=0; i< PrimeArray_size ; i++)
{
primeArray[i]= start;
start+=2;
}

low_value1= primeArray[0];

/*modification end*/

   if ((2 * proc0_size +3) < (int) sqrt((double) n)) 
   {
      if (!id) 
		printf ("Too many processes\n");
      
	MPI_Finalize();
      	exit (1);
   }

   marked = (char*) malloc (size* sizeof(int));
   if (marked == NULL) 
   {
      printf ("Cannot allocate enough memory\n");
      MPI_Finalize();
      exit (1);
   }

for (i = 0; i < size; i++) 
	{marked[i] = 0;}

for (i = 0; i < PrimeArray_size ; i++) 
        {marked1[i]=0;}

   index = 0;
   prime = 3;
   prime1 = 3;
 /*  do {
      if (prime1 * prime1 > low_value1)
         {first1= (prime1 * prime1 - low_value1)/2;}
            
      else { if ((low_value1 % prime1)==0) first1 = 0;
             else {if ((low_value1 /prime1) % 2 ==0) first1= (prime1-low_value1 % prime1)/2;
                   else first1= prime1-(low_value1 % prime1)/2; } 
           }
           
      
       for (i = first1; i < PrimeArray_size; i += prime1)      
        {marked1[i] = 1;}
 
       while (marked1[++index]);
       { prime1 = 2 * index + 3;}
       }
  
      newprime[i]= prime1;i++;

    } while (prime1 * prime1 <= n); */

do { i=0;
/*all processes compute their own primes*/
  do {
      if (prime1 * prime1 > low_value1)
         {first1= (prime1 * prime1 - low_value1)/2;}
            
      else { if ((low_value1 % prime1)==0) first1 = 0;
             else {if ((low_value1 /prime1) % 2 ==0) first1= (prime1-low_value1 % prime1)/2;
                   else first1= prime1-(low_value1 % prime1)/2; } 
           }
           
      
       for (i = first1; i < PrimeArray_size; i += prime1)      
        {marked1[i] = 1;}
 
       while (marked1[++index]);
       { prime1 = 2 * index + 3;}
       }
  
      newprime[i]= prime1;i++;

    } while (prime1 * prime1 <= n);

/*end of the primes computation*/
      if (prime * prime > low_value)
         {first= (prime * prime - low_value)/2;}
            
      else { if ((low_value % prime)==0) first = 0;
             else {if ((low_value /prime) % 2 ==0) first= (prime-low_value%prime)/2;
                   else first= prime-(low_value % prime)/2; } 
           }
           
      
       for (i = first; i < size; i += prime)      
        {marked[i] = 1;}
        
       while (marked[++index]);
         {prime = 2 * index + 3;}
       
      while (marked1[i] = )
        
         }}
    
          while (marked[++index]);
         { prime= 2 * index + 3;
         
         }}
     } while (prime * prime <= n);

   count = 0;
   for (i = 0; i < size; i++)
      if (!marked[i]) {count++;}

   MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);
   elapsed_time += MPI_Wtime();
   if (!id) {
      printf ("% d primes are less than or equal to % d \n", global_count + 1, n);
      printf ("Total elapsed time: %10.6f\n", elapsed_time);
      }
   MPI_Finalize ();
   return 0;
}


