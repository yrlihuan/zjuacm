#include <stdio.h>
int main()
{
	int number;
	char p[51], ch;
	scanf("%d", &number);
	for(int i = 0; i < number; i++ )
	{	
		printf("String #%d\n", i+ 1);
		scanf("%s",p);
		ch = p[0];
		int x = 0;
		while( ch != '\0')
		{
			if(ch == 'Z')
				ch = 'A';
			else
				ch ++;
			printf("%c", ch);
			x++;
			ch = p[x];
			
		}
		printf("\n\n");
	}
}
