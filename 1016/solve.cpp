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
	char p[64];
	int i;
	int nr;
	int pairs;

	int pl;
	int pr;

	int tmp;

	int pos;
	
#ifdef DEBUG
	fp = fopen("test.txt","r");
#endif

	SCAN("%d",&nr);
	while(nr--)
	{
		SCAN( "%d", &pairs );
		pl = 0;
		pos = 0;
		for( i = 0 ; i < pairs ; i++ )
		{
			SCAN("%d" , &tmp );
			while( tmp != pl )
			{
				p[pos++] = '(';
				pl++;
			}
			p[pos++] = ')';
		}

		p[pos] = '\0';
		
		pos = 0;
		for( i = 0 ; i < 2 * pairs ; i++ )
		{
			if( p[pos] == ')' )
			{
				pl = pos;
				pr = 1;

				while( p[--pl] != '(' )
				{
					if( p[pl] != ')' )
						pr++;
				}

				p[pl] = 'i';
				printf( "%d",pr );

				if( p[pos+1] != '\0' )
					printf(" ");

			}

			pos++;
		}

		printf("\n");
		
	}
	return 0;

}
