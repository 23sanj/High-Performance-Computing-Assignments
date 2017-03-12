
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
   double elapsed_time; /* execution time	*/
   long long first;		/* index of the first sieve	*/
   long long global_count;	/* global count of prime numbers */
   long long high_value; 	/* highest value assigned to this process */
   long long i, i1;		/* loop counter		*/
   int id;		/* this process id	*/
   long long index= 0, primeindex = 1;		/* index of the current sieve	*/
   long long low_value;	/* lowest value assigned to this process */
   char *marked, *marked1;		/* array elements to be  marked	*/
   int *prime1;
   long long  n;		/* value of the largest number	*/
   int p; 		/* number of processes		*/
   long long  proc0_size;	/* number of elements assigned to process zero */
			/* this is to find if process zero has all primes */
   long long  prime, Block_Size= 640000;		/* current prime or sieve	*/
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

   n = atoll(argv[1]);
   
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
   int squareRoot= (int) sqrt(n) ;
   int size1= (squareRoot)/2;
   marked = (char*) malloc (size* sizeof(long long));
   marked1 = (char*) malloc (size* sizeof(long long));
   prime1 = (int*) malloc (size* sizeof(int));
   if (marked == NULL | marked1== NULL ) 
   {
      printf ("Cannot allocate enough memory\n");
      MPI_Finalize();
      exit (1);
   }

   for (i = 0; i < size; i++)  	{marked[i] = 0; marked1[i]=0;}

   prime = 3;
   do{
       first = (prime*prime- 3)/2;
       for(i=first; i< size1; i+=prime) marked1[i]=1;
       while (marked1[++index]);
       prime = 2 * index + 3;
       prime1[primeindex++]= prime;// the prime array stores all the prime numbers calculated by processor 0
       } while(prime <squareRoot);
    prime1[0]=3;

    for (i=0; i< (int)(size)/Block_Size; i++) // for each block we mark the composite numbers
    { prime=3; index=0; 
       do {  if (prime * prime > low_value) {first= (prime * prime - low_value)/2;}
             else { if ((low_value % prime)==0) first = 0;
             else {if ((low_value /prime) %2 ==0) first= (prime-low_value%prime)/2;
             else first= prime-(low_value%prime)/2; } 
           }  
                      
     for (i1 = 0; i1 < Block_Size - first; i1 += prime)   
     marked[Block_Size * i +first + i1 ] = 1; 
     index++; prime=prime1[index];
     }while (prime * prime < low_value + 2 * Block_Size);// since we have eliminated the even numbers, so we increment the block size twice
           low_value += 2 * Block_Size;}
          
       i= (size)/ Block_Size;
       index = 0; prime = 3;
     
        do { 
             if (prime * prime > low_value) first= (prime * prime - low_value)/2;
             else { if ((low_value % prime)==0) first = 0;
             else {if ((low_value /prime) %2 ==0) first= (prime-low_value%prime)/2;
             else first= prime-(low_value%prime)/2; } }  
           
        for (i1 = 0; i1 < Block_Size - first; i1 += prime) 
       { if (Block_Size * i + first+ i1< size)  marked[Block_Size * i +first + i1 ] = 1; }
              index++; prime=prime1[index];
        }while(prime*prime < high_value);
            
   count = 0;
   for (i = 0; i < size; i++)
      if (!marked[i]) {count++;}

   MPI_Reduce (&count, &global_count, 1, MPI_LONG_LONG, MPI_SUM,0, MPI_COMM_WORLD);
   elapsed_time += MPI_Wtime();
   if (!id) {
      printf ("% lld primes are less than or equal to % lld \n", global_count + 1, n);// add 1 to the global index to account for prime 2
      printf ("Total elapsed time: %10.6f\n", elapsed_time);
      }
  free (marked);
  free(marked1);
  free(prime1);
   MPI_Finalize ();
   return 0;
}

	
