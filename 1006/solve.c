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

#include <string.h>

char code_to_text( int code )
{
	if( code == 0 )
		return '_';
	if( code == 27 )
		return '.';

	return code + 96;
}

int text_to_code( char text )
{
	if( text == '_' )
		return 0;
	if( text == '.' )
		return 27;

	return text - 96;
}

int main()
{
	
	char ciphertext[128];
	char plaintext[128];

	int k;
	int n;

	int i;
	
#ifdef DEBUG
	fp = fopen("test.txt" , "r");
#endif
	
	/*PRINT("_:%d,a:%d,z:%d,.:%d\n" , '_' , 'a' , 'z' , '.');*/
	SCAN("%d %s",&k , ciphertext ); 	
	while( k != 0 )
	{
		n = strlen(ciphertext);

		for( i = 0 ; i < n ; i++ )
		{
			plaintext[ i * k % n ] = code_to_text( ( text_to_code( ciphertext[i]) + i ) % 28 );
		}
		
		plaintext[n] = '\0';

		printf("%s\n", plaintext);
		SCAN("%d %s",&k , ciphertext ); 	
	}

	return 0;

	
}
