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


int search_for_match( long higher , long lower , int start )
{

	if( higher == 1 && lower == 1)
		return 1;

	if( start > higher && start > lower )
		return 0;
	if( start == 101 )
		return 0;



	if( higher!=1 && (higher%start)==0 )
	{
		if( search_for_match(higher/start , lower , start + 1) )
			return 1;
	}
	if( lower != 1 && ( lower%start ) == 0 )
	{
		if( search_for_match(higher , lower/start , start + 1) )
			return 1;
	}

	return search_for_match( higher , lower , start+1);
}

int main()
{
	long higher;
	long lower;
	long tmp;
#ifdef DEBUG
	fp= fopen("./test.txt" , "r");
#endif

	/*2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97*/
	
	while( SCAN("%ld %ld",&higher , &lower ) != EOF )
	{
		if( higher < lower )
		{
			tmp = lower;
			lower = higher;
			higher = tmp;
		}


		if( !search_for_match(lower , 1 , 2 ) )
		{
			printf("%ld\n",higher);
			continue;
		}

		if( !search_for_match( higher , 1 , 2 ))
		{
			printf("%ld\n",lower);
			continue;
		}


		if( search_for_match(higher , lower , 2 ) )
		{
			printf("%ld\n",higher);
			continue;
		}
		else
		{
			printf("%ld\n",lower);
			continue;

		}
	
		
	}
	
	return 0;
}
