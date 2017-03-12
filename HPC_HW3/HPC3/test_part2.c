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
   long long count;		/* local prime count	*/
   double elapsed_time; /* execution time	*/
   long long first;		/* index of the first sieve	*/
   long long global_count;	/* global count of prime numbers */
   long long high_value; 	/* highest value assigned to this process */
   long long i;		/* loop counter		*/
   int id;		/* this process id	*/
   long long index;		/* index of the current sieve	*/
   long long low_value;	/* lowest value assigned to this process */
   char *marked, *marked1;		/* array elements to be  marked	*/
   long long  n;		/* value of the largest number	*/
   int p; 		/* number of processes		*/
   long long  proc0_size;	/* number of elements assigned to process zero */
			/* this is to find if process zero has all primes */
   long long  prime;		/* current prime or sieve	*/
   long long size;		/* elements in marked array 	*/
   //char *primeArray;

 
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

   n = atoll(argv[1]);
	//find how many elements are assigned to this process 
   low_value = 2*(BLOCK_LOW(id,p,(n/2) - 1)) + 3;
   high_value = 2*(BLOCK_HIGH(id,p,(n/2) - 1)) + 3;
   size = ((high_value-low_value)/ (2)) + 1;
   proc0_size = ((((n)/(2))-1)/p);
  

   if ((2 * proc0_size +3) < (long long ) sqrt((double) n)) 
   {
      if (!id) 
		printf ("Too many processes\n");
      
	MPI_Finalize();
      	exit (1);
   }

   marked = (char*) malloc (size* sizeof(long long));
   marked1 = (char*) malloc (size* sizeof(long long));
   if (marked == NULL) 
   {
      printf ("Cannot allocate enough memory\n");
      MPI_Finalize();
      exit (1);
   }

for (i = 0; i < size; i++) 
	{marked[i] = 0; marked1[i]=0;}

   index = 0;
   prime = 3;

      do {
      first =(prime*prime -3)/2; // execute process 0 first
      for(i= first;i<size;i+=prime) marked1[i]=1;//all other processes compute primes as process0 does.
      
       if (prime * prime > low_value)
         {first= (prime * prime - low_value)/2;}
            
      else { if ((low_value % prime)==0) first = 0;
             else {if ((low_value /prime) %2 ==0) first= (prime-low_value%prime)/2;
                   else first= prime-(low_value%prime)/2; } 
           }
           
      
       for (i = first; i < size; i += prime)   marked[i] = 1; 

         while (marked1[++index]);// it stores the same array of primes as process0 did in part1. this step is equivalent of Bcast.
         {prime = 2 * index + 3;}
         
   
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
  free (marked);
  free(marked1);
   MPI_Finalize ();
   return 0;
}


