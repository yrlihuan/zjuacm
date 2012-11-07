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


typedef struct point_t
{
	double x;
	double y;
}point;

int is_crossed( point* a , int i , int j )
{

	double t1;
	double t2;

	t1 = ( a[j].y - a[i+1].y ) / ( a[i+1].y - a[i].y) - ( a[j].x - a[i+1].x )/( a[i+1].x - a[i].x );
	t2 = ( a[j+1].y - a[i+1].y ) / ( a[i+1].y - a[i].y) - ( a[j+1].x - a[i+1].x )/( a[i+1].x - a[i].x ); 
	if(  ( t1 == 0 && ( a[j].y - a[i].y > 0 ) != ( a[j].y - a[i+1].y > 0) ) 
	||   ( t2 == 0 && ( a[j+1].y - a[i].y > 0 ) != ( a[j+1].y - a[i+1].y > 0 ) )  )
		return 1;
	if ( ( t1 > 0 ) == ( t2 > 0 ) )
		return 0;
	
	t1 =  ( a[i].y - a[j+1].y ) / ( a[j+1].y - a[j].y) - ( a[i].x - a[j+1].x )/( a[j+1].x - a[j].x ) ;
	t2 =  ( a[i+1].y - a[j+1].y ) / ( a[j+1].y - a[j].y) - ( a[i+1].x - a[j+1].x )/( a[j+1].x - a[j].x ) ;

	if(  ( t1 == 0 && ( a[i].y - a[j].y > 0 ) != ( a[i].y - a[j+1].y > 0) ) 
	||   ( t2 == 0 && ( a[i+1].y - a[j].y > 0 ) != ( a[i+1].y - a[j+1].y > 0 ) )  )
		return 1;
	if ( ( t1 >0 ) == ( t2 > 0 ) )
		return 0;
	return 1;

}

double calculate_area( point* a , int num )
{
	double area;

	int i;

	area = 0;

	for( i = 0 ; i < num ; i++ )
	{
		area += 0.5 * ( a[i].y + a[i+1].y ) * ( a[i].x - a[i+1].x );
	}

	return area>0?area:-area;
}

int main()
{

#ifdef DEBUG
	fp = fopen("test.txt","r");
#endif

	int figure = 0;
	
	point* buffer;
	int num_of_points;
	
	int i , j;

	double area;
	
	buffer = (point*)malloc(sizeof(point) * 1003 );

	SCAN("%d",&num_of_points);
	
	while( num_of_points )
	{
		
		figure++;

		if( figure != 1 )
			printf("\n");

		for( i = 0 ; i < num_of_points ; i++ )
		{
			SCAN("%lf %lf" , &buffer[i].x , &buffer[i].y );
		}

		if( num_of_points < 3 )
		{
			printf("Figure %d: Impossible\n" , figure );
			goto next;
		}
		

		buffer[i].x = buffer[0].x;
		buffer[i].y = buffer[0].y;

		for( i = 0 ; i < num_of_points ; i++ )
		{
			for( j = i + 2; j < num_of_points ; j++ )
			{
				if( i == 0 && j == num_of_points - 1 )
					continue;
				if( is_crossed( buffer , i , j ) )
				{
					printf("Figure %d: Impossible\n" , figure );
					goto next;
				}
			}
		}

		area = calculate_area( buffer , num_of_points ) ;
		if( area  <  0.001 )
			printf("Figure %d: Impossible\n" , figure );
		else
			printf("Figure %d: %.2lf\n" , figure , area );

		
next:
		SCAN( "%d" , &num_of_points );
	}

	return 0;
}
