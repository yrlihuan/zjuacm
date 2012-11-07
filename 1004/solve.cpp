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

#define MAX 128

char input[MAX];
char output[MAX];
char stack[MAX];
char out_string[MAX*4];
	

	int out_string_pos;
	int in_pos;
	int out_pos;
	int pos;
void process( void )
{
	char tmp;
	if( output[out_pos] == '\0' )
	{
		out_string[out_string_pos] = '\0';
		printf("%s\n", out_string );
		return;
	}
	if( input[in_pos] == '\0' && stack[pos-1] != output[out_pos] )
	{
		return;
	}

	if( input[in_pos] != '\0' )
	{
		tmp = stack[pos];
		stack[pos++] = input[in_pos++];
		out_string[out_string_pos++] = 'i';
		out_string[out_string_pos++] = ' ';
		
		process( );
		
		in_pos--;
		stack[--pos] = tmp;
		out_string_pos--;
		out_string_pos--;
	}

	if( pos > 0 && stack[pos-1] == output[out_pos] )
	{
		out_string[out_string_pos++] = 'o';
		out_string[out_string_pos++] = ' ';
		out_pos++;
		tmp = stack[--pos];
		process();
		stack[pos++] = tmp;
		out_pos--;
		out_string_pos -= 2;

	}
		
}
int main()
{
#ifdef DEBUG
	fp = fopen("./test.txt","r");
#endif

	while( SCAN("%s\n%s",input , output ) != EOF )
	{
	
		pos = 0;
		in_pos = 0;
		out_pos = 0;
		out_string_pos = 0;


		PRINT("%s\n%s\n" , input , output);
		printf("[\n");
		process();
		printf("]\n");
		
	}
	return 0;
}
