
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b)  ((a)<(b)?(a):(b))

#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)   (BLOCK_LOW(((id)+1),p,n)-1)
#define BLOCK_SIZE(id,p,n)   ((BLOCK_LOW(((id)+1),p,n))-(BLOCK_LOW(id,p,n)))

int main (int argc, char *argv[])
{
   long long count;		/* local prime count	*/
   double elapsed_time;         /* execution time	*/
   long long first,first1;     /* index of the first sieve	*/
   long long global_count;	/* global count of prime numbers */
   long long high_value; 	/* highest value assigned to this process */
   long long i;		/* loop counter		*/
   int id;		         /* this process id	*/
   long long index= 0;		/* index of the current sieve	*/
   long long low_value;	         /* lowest value assigned to this process */
   char *marked, *marked1;       /* array elements to be  marked	*/

   long long  n;		/* value of the largest number	*/
   int p; 		/* number of processes		*/
   long long  proc0_size;	/* number of elements assigned to process zero */
			/* this is to find if process zero has all primes */
   long long  prime;		/* current prime or sieve	*/
   long long size;		/* elements in marked array 	*/
   
  
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

   n = atoll(argv[1]);// converts long long integer values
   /*redefine the low_value, highvalue and block size by eliminating the odd numbers*/
   low_value = 2*(BLOCK_LOW(id,p,(n/2) - 1)) + 3;
   high_value = 2*(BLOCK_HIGH(id,p,(n/2) - 1)) + 3;
   proc0_size = ((((n)/(2))-1)/p);
    size= BLOCK_SIZE(id,p,(n/2));

   if ((2 * proc0_size +3) < (long long ) sqrt((double) n)) 
   {
      if (!id) 
		printf ("Too many processes\n");
      
	MPI_Finalize();
      	exit (1);
   }
   int squareRoot= (int) sqrt(n) ; // finds squareroot of n
    /*allocate memory to the arrays used to mark the composite numbers*/
   marked = (char*) malloc (size* sizeof(long long));
   marked1 = (char*) malloc (size* sizeof(long long));
   
   if (marked == NULL | marked1== NULL ) 
   {
      printf ("Cannot allocate enough memory\n");
      MPI_Finalize();
      exit (1);
   }
    /*initialise the arrays used to mark the composite numbers*/
   for (i = 0; i < size; i++)  	{marked[i] = 0; marked1[i]=0;}

   prime = 3;// start from 3 and then add 2 as a prime number later  in the global count
  
       do {  
    if (prime * prime > (low_value)) {first= (prime * prime - (low_value))/2;}
             else { if (((low_value) % prime)==0) first = 0; // first index is a composite number
             else {if (((low_value) /prime) %2 ==0) first= (prime-(low_value)%prime)/2;// lowvalue is divisible by twice the prime then follow this equation
             else first= prime-((low_value)%prime)/2;  }
           }  
        for (i= first; i< size; i+=prime) marked[i]=1;              
          first1=(prime*prime-3)/2; //mark composite numbers 
         /
        for (i= first1; i< squareRoot; i+=prime) marked1[i]=1;// this step is identical to broadcast.
        while(marked1[++index]); prime=2*index+3; // if index =0, then prime=3
        
        }while (prime * prime < n);
      
   count = 0;
   for (i = 0; i < size; i++)
      if (!marked[i]) {count++;} // count the total number of primes left unmarked by each process

   MPI_Reduce (&count, &global_count, 1, MPI_LONG_LONG, MPI_SUM,0, MPI_COMM_WORLD);// process0 collects the final count of the primes in the global count.
   elapsed_time += MPI_Wtime();// finish timer
   if (!id) {
      printf ("% lld primes are less than or equal to % lld \n", global_count + 1, n);// add 1 to the global index to account for prime 2
      printf ("Total elapsed time: %10.6f\n", elapsed_time);
      }
  free (marked);
  free(marked1);

   MPI_Finalize ();
   return 0;
}

	