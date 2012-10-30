#include <stdio.h>
#include <math.h>
int ispri(int n)
{
	int i;
	double m = n + 0.0;
	if ( n == 2)
		return n;
	for( i = 2; i < sqrt(m) + 1; i++ )
		if( n % i == 0 )
			return 0;
	return n;
}
	
main()
{
	int number, di, pri[1001], i, j;

	int k;
	i = 1;
	for( j = 1; j < 1001; j++ )
		pri[j] = 1001;
	for(j = 1; j < 1001; j++)
		if( ispri(j) > 0 )
			pri[i++] = ispri(j);
	while( scanf("%d %d", &number, &di) != EOF )
	{
		
		printf("%d %d:", number, di );
		for( k = 1;pri[k] <= number ; k++)
			;
		if( (k - 1)  % 2 == 0 )
		{	
			if( (k-1)/2-di+1 <= 0 )
				for(j = 1; j < k ; j ++)
					printf( " %d", pri[j] );
			else
			    for( j = (k - 1) / 2 - di + 1; j < ( k - 1 ) / 2 + di + 1; j ++ )
					printf( " %d", pri[j] );
			printf("\n\n");
		}
		else
		{	
			if( k / 2 - di + 1 <= 0 )
				for(j = 1; j < k ; j++ )
					printf(" %d", pri[j] );
			else
				for( j =  k / 2 - di + 1 ; j < k / 2 + di; j ++ )
					printf(" %d", pri[j] );
			printf("\n\n");
		}

	}	 
}

