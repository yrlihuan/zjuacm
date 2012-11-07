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

char reached[4][1024];

int stack[1024*4];
int pos;

/*
 *catogory = 0:a is empty
 *catogory = 1:a is full
 *catogory = 2:b is empty and a is neither empty or full
 *catogory = 3:b is full and a is neither empty or full
 * */

/*
 *stack:
 *0 fill a
 *1 fill b
 *2 pour b a
 *3 pour a b
 *4 empty a
 *5 empty b 
 * */


int search( int a , int b , int d , int an ,int bn )
{
	int catogory;
	int index;
	
	if( an == d || bn == d )
		return 1;

	if( an == 0 )
	{
		catogory = 0;
		index = bn;
	}
	else if ( an == a )
	{
		catogory = 1;
		index = bn;
	}
	else if ( bn == 0 )
	{
		catogory = 2;
		index = an ;
	}
	else if ( bn == b )
	{
		catogory = 3;
		index = an;
	}

	if( reached[catogory][index] )
		return 0;

	reached[catogory][index] = 1;

	/*fill a*/
	if( an != a )
	{
		stack[pos++] = 0;
		if( search( a , b , d , a , bn ) )
			return 1;
		pos--;
	}

	/*fill b*/
	if( bn != b )
	{
		stack[pos++] = 1;
		if( search( a , b , d , an , b ) )
			return 1;
		pos--;
	}

	/*pour b a*/
	if( an != a )
	{
		stack[pos++] = 2;
		if( bn >= a - an )
		{
			if( search( a , b ,d , a , bn - a + an ) )
				return 1;
		}
		else
		{
			if( search( a , b , d , an + bn  , 0 ) )
				return 1;
		}

		pos--;
	}

	/*pour a b*/
	if( bn != b )
	{
		stack[pos++] = 3;
		if( an >= b - bn )
		{
			if( search( a , b , d , an - b + bn , b ))
				return 1;
		}
		else
		{
			if( search( a , b ,d , 0 , an + bn ) )
				return 1;
		}

		pos --;
	}


	/*empty a*/
	if( an!= 0 )
	{
		stack[pos++] = 4;
		if( search( a , b ,d , 0 , bn ))
			return 1;
		pos--;
	}

	/*empty b*/
	if( bn!= 0 )
	{
		stack[pos++] = 5;
		if( search( a , b ,d , an , 0) )
			return 1;
		pos--;
	}

	return 0;
		
}

void print_steps( void )
{

	int i;

	for( i = 0 ; i < pos ; i++ )
	{
		switch( stack[i] )
		{
			case 0:
				printf("fill A\n");
				break;
			case 1:
				printf("fill B\n");
				break;
			case 2:
				printf("pour B A\n");
				break;
			case 3:
				printf("pour A B\n");
				break;
			case 4:
				printf("empty A\n");
				break;
			case 5:
				printf("empty B\n");
				break;
			default:
				break;
		}
	}
	printf("success\n");
}
int main()
{
	int a , b;
	int d;

#ifdef DEBUG
	fp = fopen("test.txt" , "r" );
#endif

	
	while( SCAN("%d %d %d",&a , &b , &d ) != EOF )
	{
		memset( (void*)reached , 0 , 4 * 1024 );
		pos = 0;

		if( !search( a , b , d , 0 , 0 ) )
			printf("Oh my god.NOT FOUND!\n");
		else
			print_steps();
	}


	return 0;
}
