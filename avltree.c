#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NULL_REF NULL



typedef struct n
{
	int value;
	struct n * left_child;
	struct n * right_child;
} tree_node;

typedef struct r
{
	int size;
	tree_node * first_node;
} tree;

/**** function prototypes begin ****/

	int log_2(int x);

	tree_node * add_node(tree_node * node, int value);

	int node_height(tree_node * node);

	void balance_node(tree_node ** node_pointer);

	void rotate(tree_node ** node_pointer);

	int left_leaning(tree_node * node);

	int right_leaning(tree_node * node);

	void rotate_left_left(tree_node ** node_pointer);
	void rotate_left_right(tree_node ** node_pointer);
	void rotate_right_right(tree_node ** node_pointer);
	void rotate_right_left(tree_node ** node_pointer);

	tree_node ** tree_2_array(tree * root);

	unsigned int max_integer_string_size(int a, int b);
	unsigned int integer_string_size(int value);

	tree_node * remove_node(tree_node * node, int value);

	int value_exists(tree * root, int value);

	int node_no_children(tree_node * node);
	int node_has_only_left_child(tree_node * node);
	int node_has_only_right_child(tree_node * node);
	int node_has_2_children(tree_node * node);

	int inorder(tree_node * node, tree_node ** array, int pos);

/**** function prototypes end ****/


/**** BASIC OPERATIONS BEGIN ****/

	tree * create_tree()
	{
		tree * new_tree = (tree *) malloc(sizeof(tree));	//Lets belive that the OS is zeroing for us!
		
		new_tree->size = 0;
		new_tree->first_node = NULL_REF;
		
		if(new_tree != NULL)
		{
			return new_tree;
		}
		else
		{
			return 0;
		}
	}

	int add_element(tree * root, int value)
	{
		tree_node * new_node = (tree_node *)malloc(sizeof(tree_node));
		new_node->value = value;
		
		if(node_exists(new_node))
		{
			root->size++;
			if(root->first_node == NULL_REF)
			{
				root->first_node = new_node;
			}
			else
			{
				root->first_node = add_node(root->first_node, value);
			}
			
			return 1;
		}
		else
		{
			return 0;
		}
	}

	tree_node * add_node(tree_node * node, int value)
	{
		if(!node_exists(node))
		{
			tree_node * new_node = (tree_node *)malloc(sizeof(tree_node));
			
			if(!new_node)
			{
				exit(-1);
			}
			
			new_node->value = value;
			return new_node;
		}
		
		if(node->value <= value)
		{
			node->right_child = add_node(node->right_child, value);
		}
		else
		{
			node->left_child = add_node(node->left_child, value);
		}
		
		balance_node(&node);
		
		return node; 
	}

	int remove_element(tree * root, int value)
	{
		if(value_exists(root, value))
		{
			root->first_node = remove_node(root->first_node, value);
			root->size--;
			return 1;
		}
		else
		{
			return 0;
		}
	}

	tree_node * remove_node(tree_node * node, int value)
	{
		if(!node_exists(node))
		{
			return NULL_REF;
		}
		else if(node->value == value)
		{

			if(node_has_no_children(node))
			{
				free(node);
				return NULL_REF;
			}

			if(node_has_only_left_child(node))
			{
				tree_node * left_tree = node->left_child;
				free(node);
				return left_tree;
			}

			if(node_has_only_right_child(node))
			{
				tree_node * right_tree = node->right_child;
				free(node);
				return right_tree;
			}

	
			if(node_has_2_children(node))
			{
				tree_node * upper_node = node;
				tree_node * upper_left_child = node->left_child;
				tree_node * upper_right_child = node->right_child;
				
				if(right_leaning(node))
				{
					node = upper_right_child;
					
					upper_node->left_child = node->left_child;
					upper_node->right_child = node->right_child;

					node->left_child = upper_left_child;
					
					node->right_child = remove_node(upper_node, value);
				}
				else
				{
					node = upper_left_child;

					upper_node->left_child = node->left_child;
					upper_node->right_child = node->right_child;
					
					node->right_child = upper_right_child;
					
					node->left_child = remove_node(upper_node, value);
				}
			}
			
		}
		else if(value < node->value)
		{
			node->left_child = remove_node(node->left_child, value);
		}
		else
		{
			node->right_child = remove_node(node->right_child, value);
		}
		
		if(unbalanced(node))
		{
			balance_node(&node);
		}
		
		return node;
		
	}

	tree_node ** inorder_node_list(tree * root)
	{
		if(!tree_empty(root))
		{
			tree_node ** array = malloc(sizeof(tree_node)*tree_size(root));

			inorder(root->first_node, array, 0);
			return array;
		}
		else
		{
			return NULL_REF;
		}
	}

	int inorder(tree_node * node, tree_node ** array, int pos)
	{
		if(node_exists(node))
		{
			array[pos] = node;
			pos++;
			pos = inorder(node->left_child, array, pos);
			pos = inorder(node->right_child, array, pos);
			return pos;
		}
		else
		{
			return pos;
		}	
	}

	

/**** BASIC OPERATIONS END ***/


/**** META DATA FUNCTIONS BEGIN ****/

	int tree_size(tree * root)
	{
		return root->size;
	}

	int tree_empty(tree * root)
	{
		if(tree_size(root) > 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	int complete_tree_size(tree * root)
	{
		if(tree_empty(root))
		{
			return 0;
		}
		else
		{
			return (int)pow(2, tree_height(root)+1)-1;
		}
	}

	int tree_height(tree * root)
	{
		if(tree_empty(root))
		{
			return 0;
		}
		else
		{
			return (int)log_2(tree_size(root));
		}
	}

	int tree_is_complete(tree * root)	//Definition: a empty tree is not full.
	{
		if(tree_empty(root))
		{
			return 0;
		}
		else
		{
			return tree_size(root) == complete_tree_size(root);
		}
	}

	int node_exists(tree_node * node)
	{
		return node != NULL_REF;
	}

	tree_node * node_from_value(tree_node * node, int value)
	{
		if(!node_exists(node))
		{
			return NULL_REF;
		}
		else
		{
			if(node->value == value)
			{
				return node;
			}
			else if(node->value < value)
			{
				return node_from_value(node->right_child, value);
			}
			else
			{
				return node_from_value(node->left_child, value);
			}
		}
	}

	int value_exists(tree * root, int value)
	{
		return node_from_value(root->first_node, value) != NULL_REF;
	}

	int largest_value_in_tree(tree * root)
	{
		if(!tree_empty(root))
		{
			tree_node * node = root->first_node;
			while(node_exists(node->right_child))
			{
				node = node->right_child;
			}
			return node->value;
		}
		else
		{
			return 0;
		}
	}

	int smallest_value_in_tree(tree * root)
	{
		if(!tree_empty(root))
		{
			tree_node * node = root->first_node;
			while(node_exists(node->left_child))
			{
				node = node->left_child;
			}
			return node->value;
		}
		else
		{
			return 0;
		}
	}


/**** META DATA FUNCTIONS END ****/



/**** BALANCING FUNCTIONS BEGIN ****/

	void balance_node(tree_node ** node_pointer)
	{
		tree_node * node = *node_pointer;
		
		if(unbalanced(node))
		{
			rotate(node_pointer);
		}
	}

	void rotate(tree_node ** node_pointer)
	{
		tree_node * node = *node_pointer;

		if(left_leaning(node))
		{
			if(right_leaning(node->left_child))
			{
				rotate_left_right(node_pointer);
			}
			rotate_left_left(node_pointer);
		}
		else if(right_leaning(node))
		{
			if(left_leaning(node->right_child))
			{
				rotate_right_left(node_pointer);
			}
			rotate_right_right(node_pointer);
		}
	}

	void rotate_left_left(tree_node ** node_pointer)
	{
		tree_node * root = *node_pointer;
		tree_node * pivot = root->left_child;
		
		*node_pointer = pivot;
		root->left_child = pivot->right_child;
		pivot->right_child = root;	
	}

	void rotate_left_right(tree_node ** node_pointer)
	{
		tree_node * node = *node_pointer;
		tree_node * root = node->left_child;
		tree_node * pivot = root->right_child;

		node->left_child = pivot;
		root->right_child = pivot->left_child;
		pivot->left_child = root;
	}

	void rotate_right_right(tree_node ** node_pointer)
	{
		tree_node * root = *node_pointer;
		tree_node * pivot = root->right_child;
		
		*node_pointer = pivot;
		root->right_child = pivot->left_child;
		pivot->left_child = root;
	}
	
	void rotate_right_left(tree_node ** node_pointer)
	{
		tree_node * node = *node_pointer;
		tree_node * root = node->right_child;
		tree_node * pivot = root->left_child;

		node->right_child = pivot;
		root->left_child = pivot->right_child;
		pivot->right_child = root;
	}

	int left_leaning(tree_node * node)
	{
		return balance_factor(node) < 0;
	}

	int right_leaning(tree_node * node)
	{
		return balance_factor(node) > 0;
	}

	int unbalanced(tree_node * node)
	{
		return balance_factor(node) > 1 || balance_factor(node) < -1;
	}

	int balance_factor(tree_node * node)
	{
		return node_height(node->right_child) - node_height(node->left_child);
	}

/**** BALANCING FUNCTIONS END ****/




/**** PRINT FUNCTIONS BEGIN ****/

	void print_node_value(tree_node * node)
	{
		printf("%d", node->value);
	}

	void array_print(tree * root)
	{
		tree_node ** tree_array = tree_2_array(root);

		if(tree_array == NULL)
		{
			printf("\nArray empty!\n");
		}
		else
		{
			int i;
			for(i = 0; i < complete_tree_size(root); i++)
			{
				if(node_exists(tree_array[i]))
				{
					print_node_value(tree_array[i]);
				}
				else
				{
					printf("-");
				}
				printf(" ");
			}
			printf("\n");
		}
	}

	void pretty_print_tree(tree * root)	//TODO: make code pretty as well :D
	{
		

		if(tree_empty(root))
		{
			printf("\nTree empty!\n");
		}
		else
		{
			int row = 0;
			int nodes_in_row, start_node;
			int j, k;
			unsigned int value_string_size = max_integer_string_size(largest_value_in_tree(root), smallest_value_in_tree(root));
			tree_node ** tree_array = tree_2_array(root);
			
			int base_width = value_string_size * (2 * (int)pow(2, tree_height(root)) + 1);
			int node_space;
			int start_space = (int)pow(2, tree_height(root));
			
			printf("\n****** TREE *******\n\n");

			
			for(row = 0; row < (tree_height(root) + 1); row++)	//Print row is in for loop
			{
				nodes_in_row = (int)(pow(2, row));
				start_node = (int)(pow(2, row) - 1);
				node_space = 2 * start_space - 1;

		
				for(k = 0; k < (start_space * value_string_size); k++)
				{
					printf(" ");
				}

				for(j = 0; j < nodes_in_row; j++)
				{
			
					if(node_exists(tree_array[start_node + j]))
					{					
						for(k = 0; k < (value_string_size - integer_string_size(tree_array[start_node + j]->value)); k++)
						{
							printf(" "); 
						}
						print_node_value(tree_array[start_node + j]);
					}
					else
					{
						for(k = 0; k < value_string_size; k++)	//Null-node is as spacious as node...
						{
							printf(" ");
						}
					}

					for(k = 0; k < ( node_space * value_string_size ); k++)
					{
						printf(" ");
					}
				}
				
				start_space = start_space / 2;
				for(k = 0; k < (tree_height(root) - row); k++)
				{
					printf("\n");
				}
			}
			
			printf("\n\n*******************\n");
			
		}
	}

/**** PRINT FUNCTIONS END ****/




/**** HELPER FUNCTIONS BEGIN ****/

	int log_2(int x)
	{
		return (int)floor(log(x)/log(2));
	}

	int log_10(int x)
	{
		return (int)floor(log(x)/log(10));
	}

	void init_zero_tree_array(tree * root, tree_node ** array)
	{
		int i;
		for(i = 0; i< complete_tree_size(root); i++)
		{	
			array[i] = NULL_REF;
		}
	}

	void fill_array(tree_node * node, tree_node ** array, int pos)
	{
		if(node_exists(node))
		{
			array[pos] = node;
			fill_array(node->left_child, array, 2*pos+1);
			fill_array(node->right_child, array, 2*pos+2);
		}
	}

	tree_node ** tree_2_array(tree * root)
	{
		int array_size = complete_tree_size(root);
		
		if(array_size)
		{
			tree_node ** array = (tree_node **)malloc(array_size * sizeof(tree_node));
			init_zero_tree_array(root, array);
			fill_array(root->first_node, array, 0);
			return array;
		}
		else
		{
			return NULL_REF;
		}
	}
	
	int node_height(tree_node * node)
	{
		if(node == NULL_REF)
		{
			return 0;
		}
		else
		{
			if(node_height(node->left_child) >= node_height(node->right_child))
			{
				return 1 + node_height(node->left_child);
			}
			else
			{
				return 1 + node_height(node->right_child); 
			}
		}
	}

	unsigned int integer_string_size(int value)
	{
		if(value == 0)
		{
			return 1;
		}
		else
		{
			if(value < 0)
			{
				value *= -1;
			}
			return (unsigned int)floor(log_10(value)) + 1;
		}
	}

	unsigned int max_integer_string_size(int a, int b)
	{
		if(integer_string_size(a) > integer_string_size(b))
		{
			return integer_string_size(a);
		}
		else
		{
			return integer_string_size(b);
		}
	}
	
	int node_has_no_children(tree_node * node)
	{
		return !node_exists(node->left_child) && !node_exists(node->right_child);
	}

	int node_has_only_left_child(tree_node * node)
	{
		return node_exists(node->left_child) && !node_exists(node->right_child);
	}
	
	int node_has_only_right_child(tree_node * node)
	{
		return !node_exists(node->left_child) && node_exists(node->right_child);
	}

	int node_has_2_children(tree_node * node)
	{
		return node_exists(node->left_child) && node_exists(node->right_child);
	}

/**** HELPER FUNCTIONS END ****/



