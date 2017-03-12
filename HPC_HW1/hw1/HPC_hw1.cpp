%..................HPC ASSIGNMENT-1, PROBLEM 1......................%
// dgemm0
#include <iostream>
using namespace std
{
	/* data */
};

void MatrixMul(int n) 
{
// generate random matrices
    int A[3][2] = {{1, 4}, {2, 5}, {3, 6}};
    int B[2][3] = {{7, 8, 9}, {10, 11, 12}};
    int C[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
// start timer
  for (i=0; i<n; i++) 
  {
    for (j=0; j<n; j++) 
    { 
        for (k=0; k<n; k++) 
         { A[i*n+j] += B[i*n+k] * C[k*n+j];}

    } 

  }
 //end timer

            cout << product[row][col] << "  ";
        }
        cout << "\n"<< timer;
    }
}

void main() 
{   
	cout<<"enter the matrix size";
	cin>>int n ;
    Matrix mul(n);
    getchar();
}
