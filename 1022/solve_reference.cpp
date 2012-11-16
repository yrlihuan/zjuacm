#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
 
char Command[ 2 ][ 26 ][ 65 ];
 
struct asmmble {
    int  Varible1;
    int  Varible2;
    char Operator;
}Asmmble[ 2 ][ 105 ];
 
int Read( char Command[][ 65 ] )
{
    int  List   = 0;
    char ch     = 0;
    while ( 1 ) {
        int  Count  = 0;
        while ( ( ch = getchar() ) != '\n' ) {
            while ( ch == ' ' ) ch = getchar();
            Command[ List ][ Count ++ ] = ch;
        }
        Command[ List ][ Count ] = '\0';
        if ( !strcmp( Command[ List ], "END" ) ) break;
        if ( strcmp( Command[ List ], "" ) ) ++ List;
    }
    return List;
}
 
struct word {
    int Start;
    int End;
}Word[ 3 ];
 
struct varible {
    char    Name[ 21 ];
    int     Length;
    double  Value;
    bool    IsConst;
}Varible[ 155 ];
 
int Search( int& Number, char* String, int number )
{
    for ( int i = 4 ; i < Number ; ++ i )
        if ( Varible[ i ].Length == number && !strcmp( Varible[ i ].Name, String ) )
            return i;
    strcpy( Varible[ Number ].Name, String );
    Varible[ Number ].Length  = number;
    Varible[ Number ].IsConst = false;
    Varible[ Number ].Value   = 0.0;
    return Number ++;
}
 
char Change( char ch )
{
    if ( ch >= 'a' && ch <= 'z' )
        return ch - 'a' + 'A';
    return ch;
}
 
int searchVarible( char* Command, word Word, int& Number )
{
    int  count = 0;
    char String[ 21 ];
    if ( Command[ Word.Start ] >= '0' && Command[ Word.Start ] <= '9' ) {
        count = Command[ Word.Start ++ ]-'0';
        while ( Word.Start <= Word.End ) {
            count *= 10;
            count += Command[ Word.Start ++ ]-'0';
        }
        strcpy( Varible[ Number ].Name, "0");
        Varible[ Number ].Length    = 1;
        Varible[ Number ].IsConst   = true;
        Varible[ Number ].Value     = count;
        return Number ++;
    }else {
        int number = 0;
        String[ number ++ ] = Change( Command[ Word.Start ++ ] );
        while ( Word.Start <= Word.End ) 
            String[ number ++ ] = Change( Command[ Word.Start ++ ] );
        String[ number ] = '\0';
        return Search( Number, String, number );
    }
}
 
void commbine( int m, int n, int v1, int v2 ,char o3 )
{
    Asmmble[ m ][ n ].Varible1 = v1;
    Asmmble[ m ][ n ].Varible2 = v2;
    Asmmble[ m ][ n ].Operator = o3;
}
 
void Operat( int n, int m, int XX, int YY, int ZZ, char O )
{
    int R1 = 2*m+0;
    int R2 = 2*m+1;
    commbine( m, 4*n+1, R1, YY, ':' );
    commbine( m, 4*n+2, R2, ZZ, ':' );
    commbine( m, 4*n+3, R1, R2,  O  );
    commbine( m, 4*n+4, XX, R1, ':' );
}
 
int FindChar( char* p, char O )
{
    int count = 0;
    while ( p[ count ] ) {
        if ( p[ count ] == O )
            return count;
        ++ count;
    }
    return -1;
}
 
int command( int machine, char Command[][ 65 ], int List, int Number )
{
    for ( int i = 0 ; i < List ; ++ i ) {
        int space1 = FindChar( Command[ i ], ':' );
        int space2 = max( FindChar( Command[ i ], '+' ), FindChar( Command[ i ], '-' ) );
        int space3 = strlen( Command[ i ] );
        Word[ 0 ].Start = 0;        Word[ 0 ].End = space1-1;
        Word[ 1 ].Start = space1+2; Word[ 1 ].End = space2-1;
        Word[ 2 ].Start = space2+1; Word[ 2 ].End = space3-1;
         
        int X = searchVarible( Command[ i ], Word[ 0 ], Number );
        int Y = searchVarible( Command[ i ], Word[ 1 ], Number );
        int Z = searchVarible( Command[ i ], Word[ 2 ], Number );
         
        Operat( i, machine, X, Y, Z, Command[ i ][ space2 ] );
    }
    return Number;
}
 
void calculate( double* Var, asmmble Asm )
{
    switch( Asm.Operator ) {
        case ':':   Var[ Asm.Varible1 ]  = Var[ Asm.Varible2 ];break;
        case '+':   Var[ Asm.Varible1 ] += Var[ Asm.Varible2 ];break;
        case '-':   Var[ Asm.Varible1 ] -= Var[ Asm.Varible2 ];break;
    }
}
 
int cmpstr( char* a, char* b, int n )
{
    for ( int i = 0 ; i < n ; ++ i )
        if ( a[ i ] != b[ i ] )
            return a[ i ] - b[ i ];
    return 0;
}
 
int cmp( const void* a, const void* b )
{
    struct varible* p = (struct varible*)a;
    struct varible* q = (struct varible*)b;
    if ( p->IsConst == q->IsConst ) {
        int value = cmpstr( p->Name, q->Name, min( p->Length, q->Length ) );
        if ( !value )
            return p->Length - q->Length;
        else return value;
    }else return q->IsConst - p->IsConst;
}
 
double M1[ 55 ],M2[ 55 ];
double P[ 105 ][ 105 ],P1,P2;
double T[ 101 ][ 101 ][ 35 ];
 
void dp( int number1, int number2, int count )
{
    P[ 0 ][ 0 ] = 1.0;
     
    for ( int k = 0 ; k < count ; ++ k )
        if ( Varible[ k ].IsConst )
            T[ 0 ][ 0 ][ k ] = Varible[ k ].Value+0.0;
        else
            T[ 0 ][ 0 ][ k ] = 0.0;
             
    for ( int i = 1 ; i <= number1 ; ++ i ) {
        P[ i ][ 0 ] = 0.5*P[ i-1 ][ 0 ];
        for ( int k = 0 ; k < count ; ++ k ) 
            T[ i ][ 0 ][ k ] = T[ i-1 ][ 0 ][ k ];
        calculate( T[ i ][ 0 ], Asmmble[ 0 ][ i ] );
    }
     
    for ( int j = 1 ; j <= number2 ; ++ j ) {
        P[ 0 ][ j ] = 0.5*P[ 0 ][ j-1 ];
        for ( int k = 0 ; k < count ; ++ k ) 
            T[ 0 ][ j ][ k ] = T[ 0 ][ j-1 ][ k ];
        calculate( T[ 0 ][ j ], Asmmble[ 1 ][ j ] );
    }
     
    for ( int i = 1 ; i <= number1 ; ++ i )
    for ( int j = 1 ; j <= number2 ; ++ j ) {
        for ( int k = 0 ; k < count ; ++ k ) {
            M1[ k ] = T[ i-1 ][ j ][ k ];
            M2[ k ] = T[ i ][ j-1 ][ k ];
        }
        calculate( M1, Asmmble[ 0 ][ i ] );
        calculate( M2, Asmmble[ 1 ][ j ] );
         
        if ( i < number1 && j < number2 ) {
            P1 = P[ i-1 ][ j ]/2;P2 = P[ i ][ j-1 ]/2;
        }else if ( i < number1 ) {
            P1 = P[ i-1 ][ j ];  P2 = P[ i ][ j-1 ]/2;
        }else if ( j < number2 ) {
            P1 = P[ i-1 ][ j ]/2;P2 = P[ i ][ j-1 ];
        }else {
            P1 = P[ i-1 ][ j ]  ;P2 = P[ i ][ j-1 ];
        }
        P[ i ][ j ] = P1 + P2;
        //用 P[ i-1 ][ j ],P[ i ][ j-1 ] 代替 P1,P2的值了。。。 
        for ( int k = 0 ; k < count ; ++ k )
            T[ i ][ j ][ k ] = (P1*M1[ k ]+P2*M2[ k ])/(P[ i ][ j ]);
    }
     
    for ( int k = 0 ; k < count ; ++ k )
        Varible[ k ].Value = T[ number1 ][ number2 ][ k ];
     
    qsort( &Varible[ 4 ], count-4, sizeof( Varible[ 0 ] ), cmp );
     
    int start = 4;
    while ( Varible[ start ].IsConst ) ++ start;
    for ( int i = start ; i < count ; ++ i )
        printf("%.4lf\n",Varible[ i ].Value);
}
 
void Initial( void )
{
    for ( int k = 0 ; k < 4 ; ++ k ) {
        Varible[ k ].IsConst = false;
        Varible[ k ].Value   = 0.0;
    }
}
 
int main()
{
    int t,line1,line2,numb1,numb2;
    while ( scanf("%d",&t) != EOF )
    while ( t -- ) {
        Initial();
         
        line1 = Read( Command[ 0 ] );
        line2 = Read( Command[ 1 ] );
         
        numb1 = command( 0, Command[ 0 ], line1, 00004 );
        numb2 = command( 1, Command[ 1 ], line2, numb1 );
         
        dp( 4*line1, 4*line2, numb2 );
        if ( t ) printf("\n");
    }
    return 0;
}
