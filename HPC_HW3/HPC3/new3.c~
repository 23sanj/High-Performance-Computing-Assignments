#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) ( BLOCK_LOW((id)+1,p,n)-1 )
#define BLOCK_SIZE(id,p,n) (BLOCK_LOW( (id)+1, p, n) - BLOCK_LOW( (id), p, n  ) )

int main (int argc, char* argv[])
{
	int id, p;
	long long int  n, size, prime, i, frst, index, step, l;
	long long int low_value, high_value;
	
	double elapsed_time;
	MPI_Init (&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);

	elapsed_time = -MPI_Wtime();	
	MPI_Comm_rank (MPI_COMM_WORLD,&id);
	MPI_Comm_size (MPI_COMM_WORLD,&p);
	n = atoll(argv[1]);//need to modify to n-1/2
	n = 10000000000;
	if (n%2==0){ n = n-1;}
	long long int prsize = (n+1)/2;
	
        low_value = 2*BLOCK_LOW(id, p, prsize) +1;
	high_value= 2*BLOCK_HIGH(id, p, prsize)+1;
        size = BLOCK_SIZE(id, p, prsize);
	
	char* marked = (char*) malloc (size);
	char* marked0 =(char*) malloc (size);
	 int * prm = ( int *) malloc(size*sizeof( int));
	if (marked == NULL | marked0 == NULL | prm == NULL){
	  if (id==0) printf("memory not enough");
	   MPI_Finalize();
	   exit(1);
		}
	
	for (i=0; i<size; i++)	{ marked[i] = 0;marked0[i] = 0;	}
	
	index =1;int prmindex=1;	
	int sqrtn=(int)sqrt(n);	int high = sqrtn/2;
	prime = 3;//compute marked0: prime array from 2 to n/2p
	do {
		frst = (prime*prime-1)/2;
		for (i=frst;i<high; i+=prime) marked0[i]=1;
		while (marked0[++index]);
		prime = 2*index + 1;
		prm[prmindex++] = prime;
		}while(prime <= sqrtn);
	prm[0]=3;
	l =0;
	step=400000;
	for (i=0;i<(size-1)/step;i++) //there is no condition check
	{prime=3; index=0;
		do{if (prime*prime>low_value) frst = (prime*prime-low_value)/2;
		   else {if(low_value%prime==0) frst=0;
		   else{if((low_value/prime)%2==0) frst = (prime-low_value%prime)/2;
		   else frst = prime - (low_value%prime)/2;}}
			
		    for (l = 0; l<step-frst; l+= prime) marked[step*i+frst+l]=1;
		    index++;prime = prm[index];
	            }while(prime*prime<=low_value+2*step);
			low_value+=2*step;}
	
	i = (size-1)/step;
	prime = 3; index = 0;

	do{if (prime*prime>low_value) frst = (prime*prime-low_value)/2;
	   else {if(low_value%prime==0) frst=0;
	   else{if((low_value/prime)%2==0) frst = (prime-low_value%prime)/2;
	   else frst = prime - (low_value%prime)/2;}}
	   for (l = 0; l<step-frst; l+= prime){if(step*i+frst+l<size) marked[step*i+frst+l]=1;}
		index++;prime = prm[index];
		} while(prime*prime<=high_value);
	
	int count = 0;
	int global_count;
	for (i=0; i<size; i++)
		{if(marked[i]==0) count++;}
	MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	elapsed_time += MPI_Wtime();
	if (id==0){printf("the number of primes is %d, with the range of %lld\n", global_count,n+1 );
		printf("Total elapsed time is %10.6f\n", elapsed_time);
		}
	MPI_Finalize();
	return 0;
}


	
