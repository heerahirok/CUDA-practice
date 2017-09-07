// unique_RC_resp.c : This is RC circuit's impulse response
// We assume RC = 1

//#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define N 2000
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
	float temp;
	float x ;
	float f;

	int i, j, k, n, zc, index, index2;
	int mask;
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
		//c0 = random_0_300();
		//temp = j * x;
		//f = FREQ * (1 + temp);
		t1 = PI2 * FREQ * ((float)j);
		t1 = t1 / ((float)N);
		*(Xp + j) = (5.0 * cos(t1)) + 6.0;
	}
	
	//Initialize the signal vector in A[][]
	for (i = 0; i < N; i++)
	{
		//temp = (i - 1) * x;
		//f = FREQ * (1 + temp);
		*(Bp + i) = 0;
		for (n = 0; n < N; n++)
		{
			index = (i*(N + 1)) + n;
			//*(Ap + index) = 1.0; //(float)(n)
			if (n == i)
			{
				*(Ap + index) = 1.0;
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
					t2 = exp((0-1) * temp);
					*(Ap + index) = t2;
				}				
			}
			else //n > i
			{
				//*(Ap + index) = 0;
				/*temp = n - i;
				if (temp>4)
				{*/
					*(Ap + index) = 0;
				/*}
				else
				{
					t2 = exp((0-1) * temp);
					*(Ap + index) = t2;
				}*/
			}
			*(Bp + i) = *(Bp + i) + ( (*(Ap + index)) * (*(Xp + n)) );
		}
		index = ((N + 1) * i) + N;
		*(Ap + index) = *(Bp + i); //Augmented Matrix
	}

	printf("\nStarting Calculation\n");
	printf("\nStart Counting time from now on\n");
	
	// Program execution timing
	clock_t begin = clock();


	for (j = 0; j < N; j++)
	{
		index = ((N + 1) * j) + j;
		c1 = *(Ap + index);
		if (c1 == 0)
		{
			printf("\nDiagonal Element 0 at %d\n",j);
			continue;
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
	float precision = 0.01;
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
	

	printf("\n Press any digit and Enter to end\n");
	scanf("%d", &none);
	return(0);

}
