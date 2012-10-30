#include <stdio.h>
int ant(int a)
{
	int temp1 = a, temp2 = 0;
	int i, len = 0;
	while(temp1 != 0 )
	{
		len++;
		temp1 = temp1 / 10 ;
	}
	for(i = 0; i < len; i++)
	{
		temp2 = temp2 * 10 + a % 10;
		a = a / 10;
	}
	return temp2;
}

main()
{
	int number, j , num1, num2;
	scanf("%d", &number);
	for( j = 0; j < number; j++ )
	{
		scanf( "%d %d", &num1, &num2 );
		printf("%d\n",ant( ant(num1) + ant(num2) ));	
	}

}
