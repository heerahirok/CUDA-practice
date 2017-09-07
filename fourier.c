// white_noise.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define N 50
#define FREQ 100
#define PI2 6.2832


int random_0_300()
{
	int a;
	a = rand() % 300;
	return (a);
}


int main()
{
	float temp = (N / (2 * FREQ));
	float x = 8 * PI2 / (N);

	float f;

	int i, j, k, n, zc, index, index2;
	int amp;
	int none;
	//float X[N+1];
	float *Xp;
	//float B[N+1];
	float *Bp;
	//float A[N + 2][N + 2];
	float *Ap;
	float c0, c1, c2;
	float t, t1, t2;
	FILE *fp;

	// Program execution timing
	//clock_t begin = clock();

	srand(time(NULL));

	Xp = (float *)malloc((N + 1) * sizeof(float));

	if (Xp == NULL) {
		printf("Error - unable to allocate required memory for X[N+1]\n");
		return 2;
	}

	Bp = (float *)malloc((N + 1) * sizeof(float));

	if (Bp == NULL) {
		printf("Error - unable to allocate required memory for B[N+1]\n");
		return 3;
	}

	Ap = (float *)malloc((N + 2) * (N + 2) * sizeof(float));

	if (Ap == NULL) {
		printf("Error - unable to allocate required memory for A[N+2][N+2]\n");
		return 4;
	}

	printf("\n Genarating Random Sequence for solving AX = B, where we will determine X\n");
	printf("\n Size of the Co-efficient Matrix is %d x %d\n",N,N);

	//Initialize channel signal - X vector 
	for (j = 0; j < N; j++)
	{
		*(Xp + j) = 1.0;
	}
	
	//Initialize the signal vector in A[][]
	for (i = 0; i < N; i++)
	{
		c0 = i * x;
		//f = FREQ * (1 + temp);
		*(Bp + i) = 0;
		for (n = 0; n < N; n++)
		{
			index = (i*(N + 1)) + n;
			//*(Ap + index) = 1.0; //(float)(n)
			if (n == 0)
			{
				*(Ap + index) = 0.5;
			}
			else if (i == 0)
			{
				*(Ap + index) = 0; // n != 0
			}

			else 
			{
				amp = (2 * n) - 1;
				t = 3.1416 * (double) amp;
				t1 = 2.0 / t;
				t2 = (double) amp * c0;
				*(Ap + index) = t1 * sin(t2);
				zc = *(Ap + index);
				if(fabs(zc) <= 0.01)
				{*(Ap + index) = 0.017;}				
			}
			*(Bp + i) = *(Bp + i) + ( (*(Ap + index)) * (*(Xp + n)) );
		}
		index = ((N + 1) * i) + N;
		*(Ap + index) = *(Bp + i); //Augmented Matrix
	}

/*	printf("\nStarting Calculation\n");
	printf("\nStart Counting time from now on\n");
	
	// Program execution timing
	clock_t begin = clock();


	for (j = 0; j < N; j++)
	{
		index = ((N + 1) * j) + j;
		c1 = *(Ap + index);
		t = fabs(c1);
		if (t <= 0.001)
		{
			printf("\nDiagonal Element 0 at %d\n",j);
			//continue;
			c1 = 0.017;
		}
		for (k = 0; k < N + 1; k++)
		{
			index = ((N + 1) * j) + k;
			*(Ap + index) = *(Ap + index) / c1; //Normalize the diagonal element to 1
		}
		for (i = 0; i < N; i++)
		{
			if (i != j)
			{
				index = ((N + 1) * i) + j;
				c2 = *(Ap + index);
				for (k = 0; k < N + 1; k++)
				{
					index = ((N + 1) * i) + k;
					index2 = ((N + 1) * j) + k;
					*(Ap + index) = *(Ap + index) - ( c2 * (*(Ap + index2)) );
				}
			}
		}
	}

	//Program execution timing
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\n Calculation finished \n");
	printf("\n Time spend for program execution - %f\n", time_spent);
*/

	printf("\n");
	printf("Values of F(x)\n");
	for (k = 0; k < N; k++)
	{
		//for (n = 0; n < N+1; n++)
		//{
			index = ((N + 1) * k) + N; // + n
			t = *(Ap + index);
			printf("%d\t%f\t", k, t);
		//}
		//t = *(Xp + k);
		//printf("%f\t", t);
		//t = *(Bp + k);
		//printf("%f", t);
		printf("-\n");
	}

/*	// Testing the genrated value and calculated value
	zc = 0;
	float precision = 0.1;
	for (k = 0; k < N; k++)
	{
		index = ((N+1) * k) + N;
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
	
*/
	printf("\n Press any digit and Enter to end\n");
	scanf("%d", &none);

	return(0);

}
