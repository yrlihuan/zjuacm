#include <stdio.h>
#include <math.h>
int squ( double n)
{
	int m = sqrt(n);
	if ( n == m * m )
		return 1;
	else 
		return 0;
}
main()
{
	int number, temp, flag, xx, p[51], j, mm;
	scanf("%d", &number);

	for( int i = 0; i < number; i++ )
	{	
		flag = 0;
		for(int k = 0; k < 51; k++ )
			p[k] = 0;

		scanf("%d", &temp);
		xx = 1;
		while(flag == 0)	
		{
			for( j = 1; j <= temp && p[j] != 0&& (squ(p[j] + xx) == 0) ; j++ )
			;
			if( j > temp )
				{
					flag = 1;
			        printf( "%d\n", xx-1 );
				}
			else 
			{	
				p[j] = xx;
                xx =xx + 1;
			}
		}
	
	
	}
	
}
