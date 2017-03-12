/* Sieve of eratosthenes */
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



#define MIN(a,b)  ((a)<(b)?(a):(b))
/*
#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p)+1)
#define BLOCK_HIGH(id,p,n)   (BLOCK_LOW(((id)+1),p,n)-1)
#define BLOCK_SIZE(id,p,n)   ((BLOCK_LOW(((id)+1),p,n))-(BLOCK_LOW(id,p,n)))
#define BLOCK_OWNER(index,p,n)   (((p)*(index)+1)-1)/(n))
*/

#define BLOCK_LOW(id,p,n)    (((id)*(n)/(p)))
#define BLOCK_HIGH(id,p,n)   (BLOCK_LOW(((id)+1),(2*p),n)-1)
#define BLOCK_SIZE(id,p,n)   ((BLOCK_HIGH(id,p,n))-(BLOCK_LOW(id,p,n))+1)

int main (int argc, char *argv[])
{
   int count;		/* local prime count	*/
   double elapsed_time; /* execution time	*/
   int first;		/* index of the first sieve	*/
   int global_count;	/* global count of prime numbers */
   int high_value; 	/* highest value assigned to this process */
   int i;		/* loop counter		*/
   int id;		/* this process id	*/
   int index;		/* index of the current sieve	*/
   int low_value;	/* lowest value assigned to this process */
   char *marked;		/* array elements to be  marked	*/
   int  n;		/* value of the largest number	*/
   int p; 		/* number of processes		*/
   int  proc0_size;	/* number of elements assigned to process zero */
			/* this is to find if process zero has all primes */
   int  prime;		/* current prime or sieve	*/
   int  size;		/* elements in marked array 	*/
   
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
   low_value = 2*BLOCK_LOW(id,p,n-1);

   printf ("id %d \n\n" ,id);
   printf ("low_value %d \n" ,low_value);
   printf ("BLOCK_LOW %d \n" ,BLOCK_LOW(id,p,n-1));
/*Block high works fine*/
   high_value = 2*BLOCK_HIGH(id,p,n-1) +3;

   printf ("id %d \n\n" ,id);
   printf ("high_value %d \n" ,high_value);
   printf ("BLOCK_HIGH %d \n" ,BLOCK_HIGH(id,p,n-1));

   size = BLOCK_SIZE(id,p,n-1);

   printf ("id %d \n\n" ,id);
   printf ("size %d \n" ,size);
   printf ("BLOCK_SIZE %d \n" ,BLOCK_SIZE(id,p,n-1));
   
   //proc0_size = (n-1)/p;
   proc0_size = (BLOCK_SIZE(0,p,n));
   printf ("proc0_size %d \n" ,proc0_size);
   if ((2 + proc0_size) < (int ) sqrt((double) n)) 
   {
      if (!id) 
		printf ("Too many processes\n");
      
	MPI_Finalize();
      	exit (1);
   }

   marked = (char*) malloc (size* sizeof(char));
   if (marked == NULL) 
   {
      printf ("Cannot allocate enough memory\n");
      MPI_Finalize();
      exit (1);
   }

for (i = 0; i < size; i++) 
	marked[i] = 0;

   if (!id) index = 0;
   prime = 3;
   do {
      if (prime * prime > low_value)
         {first = prime * prime - low_value;
          printf ("id %d \n\n " ,id);
          printf ("first %d \n" ,first);
          printf ("prime %d \n" ,prime);
          printf ("low_value %d \n" ,low_value);
           }
      else {
         if (!(low_value % prime)) 
           {first = 0;
           printf ("id %d \n\n " ,id);
           printf ("first %d \n" ,first);
           printf ("prime %d \n" ,prime);
           printf ("low_value %d \n" ,low_value);
           }
         else {first = prime - (low_value % prime);
           printf ("id %d \n\n " ,id);}
           printf ("first %d \n" ,first);
           printf ("prime %d \n" ,prime);
           printf ("low_value %d \n" ,low_value);
           
      }
      for (i = first; i < size; i += prime) {marked[i] = 1;printf ("marked[i] %d \n" ,marked[i]);printf ("prime %d \n" ,prime);printf ("i %d \n" ,i);printf ("id %d \n " ,id);}


      if (!id) {
         while (marked[++index]);
         {prime = index + 2;
         printf ("id %d \n\n " ,id);}
         printf ("prime %d \n" ,prime);
         printf ("index %d \n" ,index);
         printf ("marked[index] %d \n" ,marked[index]);
         
      }
      MPI_Bcast (&prime,  1, MPI_INT, 0, MPI_COMM_WORLD);
   } while (prime * prime <= n);

count = 0;
   for (i = 0; i < size; i++)
      if (!marked[i]) {count++; printf ("prime %d \n" ,prime);
         printf ("id %d \n\n " ,id);         
         printf ("count %d \n" ,count);
         printf ("i %d \n" ,i);
         printf ("marked[i] %d \n" ,marked[i]);
         printf ("global_count %d \n" ,global_count);}
   MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);
   elapsed_time += MPI_Wtime();
   if (!id) {
      printf ("% d primes are less than or equal to % d \n", global_count, n);
      printf ("Total elapsed time: %10.6f\n", elapsed_time);
   }
   MPI_Finalize ();
   return 0;
}


