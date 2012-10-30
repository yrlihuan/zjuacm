#include <stdio.h>
#include <stdlib.h>

/*#define DPRINT*/

#ifdef DEBUG
FILE *fp;
#ifdef DPRINT
#define PRINT(S,arg...) printf(S,##arg)
#else /*!DPRINT*/
#define PRINT(S,arg...)
#endif
#define SCAN( S , arg... ) fscanf(fp , S , ##arg )
#define GETCHAR() getc( fp )
#else
#define PRINT(S,...)
#define SCAN( S , arg... ) scanf( S , ##arg )
#define GETCHAR() getchar()
#endif

#include <string.h>

/*operator priority: 	(	0
 * 			) 	1
 *			^ 	2
 *			*	3
 *			+	4
 * */

char pri[] = "()^*+";

char op_priority( char c )
{
	char i;
	for( i = 0 ; i < 5 ; i++ )
	{
		if( pri[i] == c )
			break;
	}

	return i;
	
}

void sub_equation( char* equation , int* start , int* end , int pos )
{
	
	int i;
	int left;
	int largest;

	int a , b;

	a = *start;
	b = *end;
	
	largest = 0;
	left = 0;
	
	for( i = *start ; i < *end ; i++ )
	{
		switch( equation[i] )
		{
			/* 0 ( , 1 ) , 2 ^ , 3 * , 4 + , 5 others*/
			case 0:
				left++;
				break;
			case 1:
				left--;
				break;
			case 2:
			case 3:
			case 4:
				if( left )
					break;
				if( largest < equation[i] )
					largest = equation[i];
				break;
			default:
				break;
		}
	}

	if( largest == 0 )
	{
		if( equation[*start] == 0 )
		{
			*start += 1;
			*end -= 1;
			return;
		}
		else
			return;
	}

	
	left = 0;
	
	for( ; a < b ; a++ )
	{
		if( equation[a] == 0 )
		{
			left++;
			continue;
		}

		if( equation[a] == 1 )
		{
			left--;
			continue;
		}

		if( left )
			continue;

		if( equation[a] == largest )
		{
			pos--;
			if( pos == 1 )
			{
				*start = a + 1;
			}
			else if( pos == 0 )
			{
				*end = a;
				return;
			}
		}
	}
	

	
	return;
}
int main()
{
#ifdef DEBUG
	fp = fopen("test.txt","r");
#endif
	
	char equation[1024];
	char equation_priority[1024];
	
	int start , end;

	int cases;
	int subs[128];
	int case_len;

	int i;
	char c;

	int is_first;

	is_first = 1;
	while( SCAN("%s" , equation ) )
	{
		if( equation[0] == '*' )
			break;

		if( !is_first )
			printf("\n");
		is_first = 0;

		printf("Expression %c:\n" , equation[0] );
		
		for( i = 0 ; i < strlen(equation) ; i++ )
		{
			equation_priority[i] = op_priority( equation[i] );
		}


		SCAN("%d" , &cases );

case_start:

		case_len = 0;
		do
		{
			SCAN( "%d" , subs+case_len++ );
			c = GETCHAR();
		}while( c != '\n' );
		

		start = 3;
		end = strlen(equation);

		for( i = 0 ; i < case_len ; i++ )
		{
			sub_equation( equation_priority , &start , &end , subs[i] );
		}


		for( i = case_len - 1; i >= 0 ; i-- )
		{
			printf("op(%d," , subs[i] );
		}
		printf( "%c" , equation[0] );
		for( i = 0 ; i < case_len ; i++ )
		{
			printf(")");
		}
		c = equation[end];
		equation[end] = '\0';
		printf( "=%s\n" , equation + start );
		equation[end] = c;

/*case_end*/
		if( --cases > 0 )
			goto case_start;

		
	}

	return 0;
}
