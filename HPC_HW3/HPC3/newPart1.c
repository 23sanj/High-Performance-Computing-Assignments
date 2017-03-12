#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)   (BLOCK_LOW(((id)+1),p,n)-1)
#define BLOCK_SIZE(id,p,n)   ((BLOCK_LOW(((id)+1),p,n))-(BLOCK_LOW(id,p,n)))
#define BLOCK_OWNER(index,p,n)   (((p)*(index)+1)-1)/(n))

int main (int argc, char *argv[])
{		  
   long long int first, low_value, high_value, size,n, global_count, count, index, prime, proc0_size, temp;			   	
   long long int i;		
   int id, p;		   		  									
   char *marked;			
   double elapsed_time; 
   
   MPI_Init (&argc, &argv);
   MPI_Barrier(MPI_COMM_WORLD);
   elapsed_time = -MPI_Wtime();
   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   MPI_Comm_size (MPI_COMM_WORLD, &p);

if (argc != 2) 
{
	if (!id) printf ("Command line: %s <m>\n", argv[0]);     
	MPI_Finalize(); 
	exit (1);
}
	n = atoll(argv[1]);
   low_value = 2 + BLOCK_LOW(id,p,n-1);
   if(low_value % 2 == 0) low_value++; // if low value is even, move to the next odd number
   high_value = 2 + BLOCK_HIGH(id,p,n-1);
   if(high_value % 2 == 0) high_value--; // if high value is even, move to the previous odd number, 
   size = (high_value-low_value)/2+1; // we can't use the old macro as the low and high values have changed to only have an odd value
   proc0_size = (n-1)/p;
   if ((2 + proc0_size) < (long long int) sqrt((double) n)) 
   {
      if (!id) printf ("Too many processes\n");
	MPI_Finalize();
    exit (1);
   }

   marked = (char *) malloc (size * sizeof(long long int));
   if (marked == NULL) 
   {
      printf ("Cannot allocate enough memory\n");
      MPI_Finalize();
      exit (1);
   }

	for (i = 0; i < size; i++) marked[i] = 0;

   if (!id) index = 0;
   prime = 3;
   do {
     if (prime*prime > low_value) 
	{
	first = (prime* prime-3)/2 - (low_value-3)/2;
	}
   else {
	if (!(low_value % prime)) first = 0;
	if (prime > low_value%(2*prime)) first = (prime-(low_value%prime))/2;
	if (prime < low_value%(2*prime)) first =prime-(low_value%prime)/2;
      for (i = first; i < size; i += prime) marked[i] = 1;
      if (!id) {
         while (marked[++index]);
         prime = 2*index + 3; // e.g. 5 is at 1 so 3(1)+2 = 5, replace prime = index + 2;
      }
      MPI_Bcast (&prime,  1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD); // changed data type to MPI_LONG_LONG_INT
   } while (prime * prime <= n);

	count = 0;
   for (i = 0; i < size; i++)
      if (!marked[i]) count++;

   MPI_Reduce (&count, &global_count, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   elapsed_time += MPI_Wtime();
   free(marked);
   if (!id) {
      printf ("%lld primes are less than or equal to %lld\n", global_count+1, n); // add 1 to the global count to account for prime 2
      printf ("Total elapsed time: %10.6f\n", elapsed_time);
   }
   MPI_Finalize ();
   return 0;
}

