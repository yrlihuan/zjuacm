#include <stdio.h>
#include <stdlib.h>

main()
{
	int* self_num;
	
	int	i;
	int di;

	int	q;

	int n;

	self_num = (int*)malloc( sizeof( int ) * 1000100 ) ;
	
	for( i = 0 ; i < 1000100 ; i++ )
	{
		self_num[i] = 1;
	}

	for( i = 1 ; i < 1000000 ; i ++ )
	{
		di = i;
		
		n = i;

		while( n )
		{
			q = n % 10;
			n /= 10;

			di += q;

		}

		self_num[di] = 0;


	}

	for( i = 1 ; i <= 1000000 ; i++ )
	{
		if( self_num[i] )
			printf("%d\n",i);
		
	}
	

}
