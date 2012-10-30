#include <stdio.h>
#include <stdlib.h>

int root_num( int );

/*
main()
{
	int num;

	scanf( "%d", &num );

	while( num )
	{
		printf("%d\n", root_num( num ) ) ; 
		scanf( "%d", &num );
	}
}
*/

main()
{
	int num;
	char c;
	
	num = 0;
	while( ( c = getchar() ) != '\n' )
		{
			num += c - '0';
		}

	while( num )
	{
		printf("%d\n", root_num( num ) ) ; 

		num = 0;
		while( ( c = getchar() ) != '\n' )
		{
			num += c - '0';
		}

		
	}
	


}
int	root_num( int n )
{
	int q;
	int sum;
	
	if( n < 10 )
		return n;
	else
	{
		sum = 0;
		while( n )
		{
			q = n%10;
			sum += q;
			n/=10;
		}

		return root_num( sum );
	}
}
