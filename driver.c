#include <stdio.h>
#include <unistd.h>
#include "tree.h"


int add(avl_tree * root);
int delete(avl_tree * root);
void print_menu();
void exists();
void inorder_print(avl_tree * root);
void preorder_print(avl_tree * root);
void postorder_print(avl_tree * root);


int main()
{
	printf("\n\n****** DRIVER FOR TREE DATASTRUCTURE *******\n\n");
	avl_tree * p_new_tree;
	char input;
	p_new_tree = create_tree();

	if(!p_new_tree)
	{
		printf("\nError when creating shit\n");
		return -1;
	}

	print_menu();

	while(input = getchar())
	{
		switch(input)
		{
			case '1':
				add(p_new_tree);
			break;
			case '2':
				delete(p_new_tree);
			break;
			case '3':
				exists(p_new_tree);
			break;
			case '4':
				inorder_print(p_new_tree);
			break;
			case '5':
				preorder_print(p_new_tree);
			break;
			case '6':
				postorder_print(p_new_tree);
			break;
			case '7':
				pretty_print_tree(p_new_tree);
			break;
			case '8':
				array_print(p_new_tree);
			break;
			case '9':
				print_menu();
			break;
			case '0':
				printf("Exiting...\n\n");
				return 0;
			break;
			case '\n':
			case ' ':
			case '\t':
			break;
			default:
				printf("Unknown option...\n");
				print_menu();
			break;
		}
		
	}
}

int add(avl_tree * root)
{
	int value;
	printf("\n*** ADD ELEMENT ***\n");
	printf("Value: ");
	scanf("%d", &value);
	add_element(root, value);
	printf("\nELEMENT ADDED!\n");
}

int delete(avl_tree * root)
{
	int value;
	printf("\n*** REMOVE ELEMENT ***\n");
	printf("Value: ");
	scanf("%d", &value);
	if(remove_element(root, value))
	{
		printf("\nELEMENT REMOVED!\n");
	}
	else
	{
		printf("\nNO ELEMENT WAS REMOVED...\n");
	}
	
}

void exists(avl_tree * root)
{
	int value;
	printf("\n*** SEARCH ***\n");
	printf("Value: ");
	scanf("%d", &value);
	if(value_exists(root, value))
	{
		printf("\nVALUE EXISTED!\n");
	}
	else
	{
		printf("\nVALUE NOT IN TREE\n");
	}
	
}

void inorder_print(avl_tree * root)
{
	printf("\n*** INORDER ***\n");
	tree_node ** array = inorder_node_list(root);
	int i;
	for(i = 0; i < tree_size(root); i++)
	{
		printf(" %d ", array[i]->value);
	}
	printf("\n***********\n");
}

void preorder_print(avl_tree * root)
{
	printf("\n*** PREORDER ***\n");
	tree_node ** array = preorder_node_list(root);
	int i;
	for(i = 0; i < tree_size(root); i++)
	{
		printf(" %d ", array[i]->value);
	}
	printf("\n***********\n");
}

void postorder_print(avl_tree * root)
{
	printf("\n*** POSTORDER ***\n");
	tree_node ** array = postorder_node_list(root);
	int i;
	for(i = 0; i < tree_size(root); i++)
	{
		printf(" %d ", array[i]->value);
	}
	printf("\n***********\n");
}

void print_menu()
{
	printf("\n***** MENU ***************************\n");
	printf("* 1. add element                     *\n");
	printf("* 2. remove element                  *\n");
	printf("* 3. element exists                  *\n");
	printf("* 4. Display inorder                 *\n");
	printf("* 5. Display preorder                *\n");
	printf("* 6. Display postorder               *\n");
	printf("* 7. Display tree                    *\n");
	printf("* 8. Display tree array              *\n");
	printf("* 9. Display menu                    *\n");
	printf("* 0. Quit                            *\n");
	printf("**************************************\n");
}
