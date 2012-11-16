#include <stdio.h>
#include <stdlib.h>

/*#define DEBUG*/
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

typedef struct list_t
{
	int l , r;
	struct list_t* next;
}list;

list signal_table[15][10];

typedef struct tree_node_t
{
	int type;
	int accept;
	struct tree_node_t *parent;
	struct tree_node_t *next;
}tree_node;

tree_node tree_node_cache[2098];
int cache_len;

void format_cache()
{
	cache_len = 0;
	memset( tree_node_cache , 0 , sizeof(tree_node) * 2098 );
}

tree_node* get_node()
{
	return tree_node_cache + cache_len++;
}


int main()
{
#ifdef DEBUG
	fp = fopen("test.txt","r");
#endif
	
	int n , m , k;
	
	int i , j;

	int t1 , t2;
	list *tmp_list;

	int l;

	tree_node* levels[11];
	tree_node* tmp_node;
	tree_node* node_prev_level;
	tree_node* tmp_parent;

	char tmp_char;
	char c1 , c2;

	int tmp_type;

	int ac1 , ac2;

	int valid;

	int cases;

	cases = 0;

	while( 1 )
	{
		
		SCAN("%d %d %d" , &n , &m , &k );
		if( n == 0 )
			break;

		cases++;
		if( cases != 1 )
			printf("\n");
		printf("NTA%d:\n" , cases );

		for( i = 0 ; i < n ; i++ )
			for( j = 0 ; j < k ; j++ )
			{
				SCAN("%d %d" , &t1 , &t2 );
				signal_table[i][j].l = t1;
				signal_table[i][j].r = t2;
				tmp_list = &signal_table[i][j];

				while( GETCHAR() != '\n' )
				{
					SCAN("%d %d" , &t1 , &t2 );
					tmp_list->next = (list*)malloc( sizeof(list) );
					tmp_list = tmp_list->next;
					tmp_list->l = t1;
					tmp_list->r = t2;
				}

				tmp_list->next = NULL;
			}

		/*for( i = 0 ; i < n ;i++ )
			for( j = 0 ; j < k; j++ )
			{
				tmp_list = &signal_table[i][j];

				while( tmp_list )
				{
					printf("%d %d " , tmp_list->l , tmp_list->r );
					tmp_list = tmp_list->next;
				}

				printf("\n");
			}
		*/


		while( 1 )
		{
			SCAN("%d\n" , &l );
			if( l == -1 )
				break;
			
			format_cache();
			valid = 0;

			/*PRINT("REACH 1:l=%d\n" , l);*/

			levels[0] = get_node();
			tmp_node = levels[0];
			tmp_char = GETCHAR();
			GETCHAR();
			tmp_node->type = tmp_char - 'a';
			tmp_node->next = get_node();


			for( i = 1 ; i <= l ; i++ )
			{
				node_prev_level = levels[i-1];

				levels[i] = get_node();

				tmp_node = levels[i];
				do
				{
					/*while( node_prev_level->type == '*'-'a' )
						node_prev_level = node_prev_level->next;*/

					SCAN("%c %c" , &c1 , &c2 );
					/*PRINT("REACH 2: c1=%c c2=%c\n" , c1 , c2 );*/

					tmp_node->next = get_node();
					tmp_node->parent = node_prev_level;
					tmp_node->type = c1 - 'a';
					if( c1 == '*' )
						tmp_node->accept = 0xffff<<(n-m) & 0xffff>>(16-n);
					tmp_node = tmp_node->next;

					tmp_node->next = get_node();
					tmp_node->parent = node_prev_level;
					tmp_node->type = c2 - 'a';
					if( c2 == '*' )
						tmp_node->accept = 0xffff<<(n-m) & 0xffff>>(16-n);
					tmp_node = tmp_node->next;

					node_prev_level = node_prev_level->next;
					if( node_prev_level == NULL )
					{
						malloc(1000000);
						return 0;
					}

					tmp_char = GETCHAR();	
				}while(tmp_char == ' ');

			}

			/*
			for( i = 1 ; i <= l ; i++ )
			{
				tmp_node = levels[i];
				while( tmp_node->next )
				{
					printf("%c(%c) " , tmp_node->type+'a' , tmp_node->parent->type+'a' );
					tmp_node = tmp_node->next;
				}
				printf("\n");
			}
			*/

			tmp_node = levels[l];

			while( tmp_node->next )
			{
				tmp_type = tmp_node->type;
				if( tmp_type != '*' - 'a' )
				{
					tmp_node->accept = 0;
					for( i = 0 ; i < n ; i++ )
					{
						tmp_list = &signal_table[i][tmp_type];

						while( tmp_list )
						{
							if( tmp_list->l >= n-m && tmp_list->r >= n-m )
							{
								tmp_node->accept |= 1<<i;
								break;
							}

							tmp_list = tmp_list->next;
						}
					}

					if( tmp_node-> accept == 0 )
					{
						valid = 0;
						goto print;
					}
				}

				tmp_node = tmp_node->next;
			}

			for( j = l ; j > 0 ; j-- )
			{
				tmp_node = levels[j];

				while( tmp_node-> next )
				{

					tmp_parent = tmp_node->parent;
					tmp_type = tmp_parent->type;
					
					ac1 = tmp_node->accept;
					tmp_node = tmp_node->next;
					ac2 = tmp_node->accept;
					tmp_node = tmp_node->next;

					if( tmp_type == '*' - 'a' )
						continue;
					
					tmp_parent->accept = 0;
					PRINT("REACH 3: node=%c ac1=0x%x ac2=0x%x\n" , tmp_type+'a' , ac1 , ac2 );

					for( i = 0 ; i < n ; i++ )
					{
						tmp_list = &signal_table[i][tmp_type];

						while( tmp_list )
						{
							PRINT("REACH 3.5: l=%d r=%d\n" , tmp_list->l , tmp_list->r );
							if( ( ( ac1 & (1<<tmp_list->l) ) ) && 
							     (  ac2 & (1<<tmp_list->r) ) )
							{
								tmp_parent->accept |= 1<<i;
								PRINT("REACH 4: node=%c signal=%d\n" , tmp_type+'a' , i );
								break;
							}

							tmp_list = tmp_list->next;
						}

					}

					if( tmp_parent->accept == 0 )
					{
						valid = 0;
						goto print;
					}
				}
				PRINT("REACH 5: j=%d\n" , j);
			}


			if( levels[0]->accept & 1 )
				valid = 1;
			else
				valid = 0;

print:
			if( valid )
				printf("Valid\n");
			else
				printf("Invalid\n");
		}
			

		
	}

	return 0;
