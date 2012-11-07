#include <stdio.h>
#include <stdlib.h>

/*#define DEBUG*/
#ifdef DEBUG
FILE *fp;
#define PRINT(S,arg...) printf(S,##arg)
#define SCAN( S , arg... ) fscanf(fp , S , ##arg )
#else
#define PRINT(S,...)
#define SCAN( S , arg... ) scanf( S , ##arg )
#endif


int main()
{
	double x[2001];
	double y[2001];
	double d1[2000];
	double d2[1999];

	int k;
	double sum;

	int i;
	
	y[0] = 	1.644934066848;
	y[500]= 1.227411277760;
	y[1000]=1.000000000000;
	y[2000]=0.750000000000;

	x[0] = 0.00000000000000000000000;
	for( i = 1 ; i < 2001 ; i++ )
	{
		x[i] = 0.0010000000000000000000 + x[i-1];
	}

	for( i = 0 ; i < 1999 ; i++ )
	{
		sum = 0.0000000000000000000000000;
		for( k = 1 ; k < 20000 ; k++ )
		{
			sum += 0.00000200000000 / ( k * (k+x[i]) * (k+x[i+1]) * (k+x[i+2]) );
		}

		d2[i] = sum;
	}

	/*calculate d1[0]*/

	sum = y[500] - y[0];

	for( i = 0 ; i < 499 ; i++ )
		sum -= (499-i) * d2[i];
	
	d1[0] = sum / 500;

	for( i = 1 ; i < 2000 ; i++ )
	{
		d1[i] = d1[i-1] + d2[i-1];
	}

	for( i = 1 ; i < 2001 ; i++ )
		y[i] = y[i-1] + d1[i-1];

	for( i = 0 ; i < 2001 ; i++)
		printf("%5.3f %16.12f\n" , x[i] , y[i]);


	return 0;
}
