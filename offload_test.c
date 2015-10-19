#include <omp.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  
  double *a, *b, *c, *d;
  int i,j,k, ok, n=20;
  
  // allocated memory on the heap aligned to 64 byte boundary
  /* ok = posix_memalign((void**)&a, 64, n*n*sizeof(double)); */
  /* ok = posix_memalign((void**)&b, 64, n*n*sizeof(double)); */
  /* ok = posix_memalign((void**)&c, 64, n*n*sizeof(double)); */

  // initialize matrices
  a = malloc(n*n*sizeof(double));
  /* b = a + n*n; */
  b = malloc(n*n*sizeof(double));
  c = malloc(n*n*sizeof(double));
  /* d = c + n*n; */

  for(i = 0; i < n*n; i++)
    {
      a[i] = 3;
      b[i] = 0;
    }
  for(i = 0; i < n; i++)
    {
      b[i*n+i] = 1;
    }

  //offload code
/* #pragma offload target(mic) in(a,b:length(n*n)) out(c:length(n*n)) */
  {
    //parallelize via OpenMP on MIC
/* #pragma omp parallel for */
    for( i = 0; i < n; i++ ) {
      for( k = 0; k < n; k++ ) {
	/* #pragma vector aligned */
	/* #pragma ivdep */
	for( j = 0; j < n; j++ ) {
	  //c[i][j] = c[i][j] + a[i][k]*b[k][j];
	  /* c[i*n+j] = c[i*n+j] + a[i*n+k]*b[k*n+j]; */
	  c[i*n+j] = a[i*n+k]*b[k*n+j];
	  /* d[i*n+j] = a[i*n+k]*b[k*n+j]; */
	}
      }
    }
  }
  for(i = 0; i < n; i++)
    {
      for(j = 0; j < n; j++)
  	{
  	  printf("%f, ", c[i*n+j]);
  	}
      printf("\n");
    }
}
