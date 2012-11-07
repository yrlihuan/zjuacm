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

	int num;
	
	int i , j , k;
	int p;
	int len;

	int lines;

	int pos[3];
	char enigma[3][27];

	char code[1024];

	int c1 , c2 , c3;

#ifdef DEBUG
	fp = fopen("test.txt","r");
#endif

	num = 0;
	SCAN("%d" , &len );
	while( len )
	{
		if( num )
			printf("\n");
	
		printf("Enigma %d:\n" , ++num );

		for( i = 0 ; i < 3 ; i++ )
			SCAN("%s",enigma[i] );
		for( i = 0 ; i < 3 ; i++ )
		{
			for( j = 0 ; j < len ; j++ )
			{
				enigma[i][j] = ( enigma[i][j] - 'A' - j + len) % len ;
				PRINT("%d " , enigma[i][j]);
			}

			PRINT("\n");
		}
		SCAN("%d" , &lines );
		while( lines-- )
		{
			SCAN("%s" , code );
			p = 0;
			pos[0] = pos[1] = pos[2] = 0;
			
			while( 1 )
			{
				for( j = 0 ; j < len ; j++ )
				{
					for( k = 0 ; k < len ; k++ )
					{
						if( code[p] == '\0' )
							goto done;

						for( i = 0 ; i < len ; i++ )
						{
							c1 = i + enigma[0][(i+pos[0])%len];
							if( c1 >= len )
								c1 -= len;

							c2 = c1 + enigma[1][(c1+pos[1])%len];
							if( c2 >= len )
								c2 -= len;

							c3 = c2 + enigma[2][(c2+pos[2])%len];
							if( c3 >= len )
								c3 -= len;

							if( c3 + 'A' == code[p] )
							{
								code[p] = i + 'a';
								break;
							}
						}

						p++;
						if( pos[0] == 0 )
							pos[0] = len - 1;
						else
							pos[0]--;
					}
					if( pos[1] == 0 )
						pos[1] = len - 1;
					else
						pos[1]--;
				}
				if( pos[2] == 0 )
					pos[2] = len - 1;
				else
					pos[2]--;
			}
done:
			printf("%s\n" , code );
		
		
		}

		
		SCAN("%d" , &len );
	}
	
	return 0;
}
