// white_noise.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define N 3000
#define FREQ 200
#define PI2 6.2832


int random_0_300()
{
	int a;
	a = rand() % 300;
	return (a);
}


int main()
{
	double temp = (N / (2 * FREQ));
	if (temp == 1)
	{
		printf("\n The parameter value N=%d and FREQ=%d is not suitable for this calculation", N, FREQ);
		return 1;
	}
	double x = (temp - 1) / (N - 1);
	double f;

	int i, j, k, n, zc, index, index2;
	int mask;
	int none;
	//double X[N+1];
	double *Xp;
	//double B[N+1];
	double *Bp;
	//double A[N + 2][N + 2];
	double *Ap;
	double c0, c1, c2;
	double t, t1, t2;
	FILE *fp;

	// Program execution timing
	//clock_t begin = clock();

	srand(time(NULL));

	Xp = (double *)malloc((N + 1) * sizeof(double));

	if (Xp == NULL) {
		printf("Error - unable to allocate required memory for X[N+1]\n");
		return 2;
	}

	Bp = (double *)malloc((N + 1) * sizeof(double));

	if (Bp == NULL) {
		printf("Error - unable to allocate required memory for B[N+1]\n");
		return 3;
	}

	Ap = (double *)malloc((N + 2) * (N + 2) * sizeof(double));

	if (Ap == NULL) {
		printf("Error - unable to allocate required memory for A[N+2][N+2]\n");
		return 4;
	}

	printf("\n Genarating Random Sequence for solving AX = B, where we will determine X\n");
	printf("\n Size of the Co-efficient Matrix is %d x %d\n",N,N);

	//Initialize channel signal - X vector 
	for (j = 0; j < N; j++)
	{
		//c0 = random_0_300();
		temp = j * x;
		f = FREQ * (1 + temp);
		t1 = 5 * PI2 * f * ((double)j);
		t1 = t1 / ((double)N);
		*(Xp + j + 1) = cos(t1);
	}
	
	//Initialize the signal vector in A[][]
	for (i = 1; i <= N; i++)
	{
		//temp = (i - 1) * x;
		//f = FREQ * (1 + temp);
		*(Bp + i) = 0;
		for (n = 1; n <= N; n++)
		{
			index = (i*(N + 1)) + n;
			//*(Ap + index) = 1.0; //(double)(n)
			if (n == i)
			{
				*(Ap + index) = 1.0;
				t = *(Xp + n);
				t = fabs(t);
				if (t < 0.001)
				{
					t1 = PI2 * f * (double)(n+0.5);
					t1 = t1 / ((double)N);
					*(Xp + j + 1) = cos(t1);
				}
			}
			else if (n < i)
			{
				temp = i - n;
				if(temp>4)
				{ 
					*(Ap + index) = 0; 
				}
				else 
				{
					t2 = 1 / (4 * temp);
					*(Ap + index) = t2;
				}
				
			}
			else //n > i
			{
				//*(Ap + index) = 0;
				temp = n - i;
				if (temp>4)
				{
					*(Ap + index) = 0;
				}
				else
				{
					t2 = 1 / (4 * temp);
					*(Ap + index) = t2;
				}
			}
			*(Bp + i) = *(Bp + i) + ( (*(Ap + index)) * (*(Xp + n)) );
		}
		index = ((N + 1) * i) + N + 1;
		*(Ap + index) = *(Bp + i); //Augmented Matrix
	}

	printf("\nStarting Calculation\n");
	printf("\nStart Counting time from now on\n");
	
	// Program execution timing
	clock_t begin = clock();


	for (j = 1; j <= N; j++)
	{
		index = ((N + 1) * j) + j;
		c1 = *(Ap + index);
		if (c1 == 0)
		{
			continue;
		}
		for (k = 1; k <= N + 1; k++)
		{
			index = ((N + 1) * j) + k;
			*(Ap + index) = *(Ap + index) / c1; //Normalize the diagonal element to 1
		}
		for (i = 1; i <= N; i++)
		{
			if (i != j)
			{
				index = ((N + 1) * i) + j;
				c2 = *(Ap + index);
				for (k = 1; k <= N + 1; k++)
				{
					//printf("\n j=%d i=%d k=%d", j, i, k);
					index = ((N + 1) * i) + k;
					index2 = ((N + 1) * j) + k;
					*(Ap + index) = *(Ap + index) - ( c2 * (*(Ap + index2)) );
					//t = A[i][k];
					//printf("\n A[%d][%d] = %f", i, k, t);
				}
			}
		}
	}

	//Program execution timing
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\n Calculation finished \n");
	printf("\n Time spend for program execution - %f\n", time_spent);


/*	printf("\n");
	for (k = 1; k <= N; k++)
	{
		for (n = 1; n <= N+1; n++)
		{
			index = ((N + 1) * k) + n;
			t = *(Ap + index);
			printf("%f\n", t);
		}
		t = *(Xp + k);
		//printf("%f\t", t);
		//t = *(Bp + k);
		printf("%f", t);
		printf("-----------------------\n");
	}
	*/

	// Testing the genrated value and calculated value
	zc = 0;
	double precision = 0.001;
	for (k = 1; k <= N; k++)
	{
		index = ((N+1) * k) + N + 1;
		t = *(Ap + index);
		c2 = *(Xp + k);
		//printf("A[%d][%d] = %f \t X[%d] = %f \n",k,N+1,t,k,c2);
		//t = fabs(t);
		//c2 = fabs(c2);
		c1 = fabs(t - c2);
		if (c1 > precision)
		{
			zc++;
		}
	}

	if (zc > 0)
	{
		printf("\nTest Failed : Out of %d element %d calculated as wrong element\n",N,zc);
	}
	

	printf("\n Press any digit and Enter to end\n");
	scanf("%d", &none);
	return(0);

}