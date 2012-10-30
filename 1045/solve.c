#include <stdio.h>
#include <stdlib.h>

main()
{
	float length;
	float l;

	int i;

	scanf( "%f", &length );

	while( length > 0.001 )
	{
		i = 2;
		l = 0;

		while( l + 1.0/i <= length )
		{
			l += 1.0/i;
			i++;
		}

		printf("%d card(s)\n",i-1);

		scanf( "%f", &length );
	}
}
