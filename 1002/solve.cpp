#include <stdio.h>
#include <stdlib.h>

int  matrix[4][4];
int  danger[4*4];
int  in_relation[16][6];
int  num_relation[16];
  
int  rows;
int  num;

int  max;

void  search( int n , int num_of_castles )
{
  int i;
  
  if( num - n + num_of_castles <= max )
    return;
  if( n + 1 == num )
  {
    num_of_castles += danger[n]?0:1;
    if( num_of_castles > max )
      max = num_of_castles;
    return;
  }

  if( !danger[n] )
  {

    for( i = 0 ; i < num_relation[n] ; i++ )
    {
      danger[in_relation[n][i]]++;
    }

    search( n + 1 , num_of_castles + 1 );

    for( i = 0 ; i < num_relation[n] ; i++ )
    {
      danger[in_relation[n][i]]--;
    }
  }

  search( n + 1 , num_of_castles );
  return ;
}


int main()
{
  
  int  i , j;
  int  n;
  int  p , q;
  
  
  while( scanf("%d\n" , &rows ) != EOF )
  {

    if( rows == 0 )
      break;
    
    /*read the matrix*/
    for( i = 0 ; i < rows ; i++ )
    {
      for( j = 0 ; j < rows ; j++ )
        scanf("%c", &matrix[i][j] );
      getchar();
    }

    /*initiate the relations*/
     num = 0;
    /*give each position an id*/
    for( i = 0 ; i < rows ; i++ )
      for( j = 0 ; j < rows ; j++ )
        if( matrix[i][j] != 'X' )
          matrix[i][j] = num++;
        

    /*initiate the danger and in_relation num_relation matrix*/
    for( i = 0 ; i < rows ; i++ )
      for( j = 0 ; j < rows ; j++ )
      {	
        n = matrix[i][j];
        if( n == 'X' )
          continue;
          
        num_relation[n] = 0;

        p = i + 1;
        while( p < rows && matrix[p][j] != 'X')
        {
          in_relation[n][num_relation[n]++] = matrix[p][j];
          p++;
        }

        q = j + 1;
        while( q < rows && matrix[i][q] != 'X')
        {
          in_relation[n][num_relation[n]++] = matrix[i][q];
          q++;
        }

        danger[n] = 0;
        
      }

    /*testing
    for( p = 0 ; p < num ; p ++ )
    {
      printf("%d,%d:" , p , num_relation[p] );
      for( q = 0 ; q < num_relation[p] ; q ++ )
        printf("%d " , in_relation[p][q] );
      printf("\n");
    }*/
    /*now searching*/
    
    max = 0;
    
    if( num )
      search( 0 , 0 );
    
    printf("%d\n",max);
    
  }
  return 0;
}

