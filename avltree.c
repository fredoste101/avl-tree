/*
	The idea is a binary search tree data structure with an easy to use API
	Will be an int since it is compare problems otherwise..!!

	ADD HEIGHT IN EACH NODE?
	IS LEAF?
	BULK OPERATIONS - That is: add tree to tree and so on... split and joint but that is future
	THERE ARE MORE WAYS TO DELETE THAN CURRENTLY IMPLEMENTED!
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NULL_REF NULL



typedef struct tree_node_struct
{
	int value;
	struct tree_node_struct * left_child;
	struct tree_node_struct * right_child;
} tree_node;

typedef struct avl_tree_struct
{
	int size;
	tree_node * first_node;
} avl_tree;

/**** PROTOTYPES BEGIN ****/

	/**** BASIC OPERATIONS BEGIN ****/
		static tree_node * add_node(tree_node * node, int value);
		static tree_node * remove_node(tree_node * node, int value);
		static int inorder(tree_node * node, tree_node ** array, int pos);
		static int preorder(tree_node * node, tree_node ** array, int pos);
		static int postorder(tree_node * node, tree_node ** array, int pos);
	/**** BASIC OPERATIONS END ****/

	/**** META DATA FUNCTIONS BEGIN ****/
		static tree_node * node_from_value(tree_node * node, int value);
		static int node_exists(tree_node * node);

	/**** META DATA FUNCTIONS END ****/

	/**** BALANCING FUNCTIONS BEGIN ****/
		static void balance_node(tree_node ** node_pointer);
		static void rotate(tree_node ** node_pointer);
		static void rotate_left_left(tree_node ** node_pointer);
		static void rotate_left_right(tree_node ** node_pointer);
		static void rotate_right_right(tree_node ** node_pointer);
		static void rotate_right_left(tree_node ** node_pointer);
		static int left_leaning(tree_node * node);
		static int right_leaning(tree_node * node);
		static int unbalanced(tree_node * node);
		static int balance_factor(tree_node * node);
	/**** BALANCING FUNCTIONS END ****/


	/**** PRINT FUNCTIONS BEGIN ****/
		static void print_n_chars(int n, char c);
		static void pretty_print_tree_row(int row, tree_node ** tree_array, int start_space, int value_string_size);
	/**** PRINT FUNCTIONS END ****/

	/**** HELPER FUNCTIONS BEGIN ****/
		static int log_2(int x);
		static int log_10(int x);
		static void init_zero_tree_array(avl_tree * root, tree_node ** array);
		static void fill_array(tree_node * node, tree_node ** array, int pos);
		static tree_node ** tree_2_array(avl_tree * root);
		static int node_height(tree_node * node);
		static unsigned int integer_string_size(int value);
		static unsigned int max_integer_string_size(int a, int b);
		static int node_has_no_children(tree_node * node);
		static int node_has_only_left_child(tree_node * node);
		static int node_has_only_right_child(tree_node * node);
		static int node_has_2_children(tree_node * node);
		static tree_node ** node_array(avl_tree * root);
	/**** HELPER FUNCTIONS END ****/

/**** PROTOTYPES END ****/


/**** BASIC OPERATIONS BEGIN ****/

	avl_tree * create_tree()
	{
		avl_tree * new_tree = (avl_tree *) malloc(sizeof(avl_tree));

		if(new_tree != NULL_REF)
		{
			new_tree->size = 0;
			new_tree->first_node = NULL_REF;
		}
		
		return new_tree;	//Not that this could be NULL if malloc fails.
		
	}

	int add_element(avl_tree * root, int value)
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

	static tree_node * add_node(tree_node * node, int value)
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

	int remove_element(avl_tree * root, int value)
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

	static tree_node * remove_node(tree_node * node, int value)
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

	tree_node ** inorder_node_list(avl_tree * root)
	{
		tree_node ** array = node_array(root);
		inorder(root->first_node, array, 0);
		return array;
	}
	
	tree_node ** preorder_node_list(avl_tree * root)
	{
		tree_node ** array = node_array(root);
		preorder(root->first_node, array, 0);
		return array;
	}
	
	tree_node ** postorder_node_list(avl_tree * root)
	{
		tree_node ** array = node_array(root);
		postorder(root->first_node, array, 0);
		return array;
	}

	static int inorder(tree_node * node, tree_node ** array, int pos)
	{
		if(node_exists(node))
		{
			pos = inorder(node->left_child, array, pos);
			array[pos] = node;
			pos++;
			pos = inorder(node->right_child, array, pos);
		}
		
		return pos;
		
	}

	static int preorder(tree_node * node, tree_node ** array, int pos)
	{
		if(node_exists(node))
		{
			array[pos] = node;
			pos++;
			pos = inorder(node->left_child, array, pos);
			pos = inorder(node->right_child, array, pos);
		}
		
		return pos;
			
	}

	static int postorder(tree_node * node, tree_node ** array, int pos)
	{
		if(node_exists(node))
		{
			pos = inorder(node->left_child, array, pos);
			pos = inorder(node->right_child, array, pos);
			array[pos] = node;
			pos++;
		}
		
		return pos;
	}

/**** BASIC OPERATIONS END ***/


/**** META DATA FUNCTIONS BEGIN ****/

	int tree_empty(avl_tree * root)
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

	int tree_size(avl_tree * root)
	{
		return root->size;
	}

	int largest_value_in_tree(avl_tree * root)
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

	int smallest_value_in_tree(avl_tree * root)
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

	int tree_is_complete(avl_tree * root)	//Definition: a empty tree is not complete.
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

	int value_exists(avl_tree * root, int value)
	{
		return node_from_value(root->first_node, value) != NULL_REF;
	}

	int complete_tree_size(avl_tree * root)
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

	int tree_height(avl_tree * root)
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
	
	static int node_exists(tree_node * node)
	{
		return node != NULL_REF;
	}

	static tree_node * node_from_value(tree_node * node, int value)
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

/**** META DATA FUNCTIONS END ****/


/**** BALANCING FUNCTIONS BEGIN ****/

	static void balance_node(tree_node ** node_pointer)
	{
		tree_node * node = *node_pointer;
		
		if(unbalanced(node))
		{
			rotate(node_pointer);
		}
	}

	static void rotate(tree_node ** node_pointer)
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

	static void rotate_left_left(tree_node ** node_pointer)
	{
		tree_node * root = *node_pointer;
		tree_node * pivot = root->left_child;
		
		*node_pointer = pivot;
		root->left_child = pivot->right_child;
		pivot->right_child = root;	
	}

	static void rotate_left_right(tree_node ** node_pointer)
	{
		tree_node * node = *node_pointer;
		tree_node * root = node->left_child;
		tree_node * pivot = root->right_child;

		node->left_child = pivot;
		root->right_child = pivot->left_child;
		pivot->left_child = root;
	}

	static void rotate_right_right(tree_node ** node_pointer)
	{
		tree_node * root = *node_pointer;
		tree_node * pivot = root->right_child;
		
		*node_pointer = pivot;
		root->right_child = pivot->left_child;
		pivot->left_child = root;
	}
	
	static void rotate_right_left(tree_node ** node_pointer)
	{
		tree_node * node = *node_pointer;
		tree_node * root = node->right_child;
		tree_node * pivot = root->left_child;

		node->right_child = pivot;
		root->left_child = pivot->right_child;
		pivot->right_child = root;
	}

	static int left_leaning(tree_node * node)
	{
		return balance_factor(node) < 0;
	}

	static int right_leaning(tree_node * node)
	{
		return balance_factor(node) > 0;
	}

	static int unbalanced(tree_node * node)
	{
		return balance_factor(node) > 1 || balance_factor(node) < -1;
	}

	static int balance_factor(tree_node * node)
	{
		return node_height(node->right_child) - node_height(node->left_child);
	}

/**** BALANCING FUNCTIONS END ****/


/**** PRINT FUNCTIONS BEGIN ****/

	void print_node_value(tree_node * node)
	{
		printf("%d", node->value);
	}

	void array_print(avl_tree * root)
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

	void pretty_print_tree(avl_tree * root)
	{
		if(tree_empty(root))
		{
			printf("\nTree empty!\n");
		}
		else
		{
			int row = 0;
			unsigned int value_string_size = max_integer_string_size(largest_value_in_tree(root), smallest_value_in_tree(root));
			tree_node ** tree_array = tree_2_array(root);
			
			int start_space = (int)pow(2, tree_height(root));
			
			printf("\n****** TREE *******\n\n");

			for(row = 0; row < (tree_height(root) + 1); row++)
			{
				pretty_print_tree_row(row, tree_array, start_space, value_string_size);
								
				print_n_chars((tree_height(root) - row), '\n'); 
				start_space = start_space / 2;
			}
			
			printf("\n\n*******************\n");
			
		}
	}

	static void print_n_chars(int n, char c)
	{
		int i;
		for(i = 0; i < n; i++)
		{
			printf("%c", c);
		}
	}

	static void pretty_print_tree_row(int row, tree_node ** tree_array, int start_space, int value_string_size)
	{
		int nodes_in_row = (int)(pow(2, row));
		int start_node = (int)(pow(2, row) - 1);
		int node_space = 2 * start_space - 1;
		int j;
		
		print_n_chars((start_space * value_string_size), ' ');
		
		for(j = 0; j < nodes_in_row; j++)
		{
			if(node_exists(tree_array[start_node + j]))
			{
				int n = (value_string_size - integer_string_size(tree_array[start_node + j]->value));
				print_n_chars(n, ' ');
				
				print_node_value(tree_array[start_node + j]);
			}
			else
			{
				print_n_chars(value_string_size, ' ');
			}

			print_n_chars(( node_space * value_string_size ), ' ');
		}

	}
	
/**** PRINT FUNCTIONS END ****/


/**** HELPER FUNCTIONS BEGIN ****/

	static int log_2(int x)
	{
		return (int)floor(log(x)/log(2));
	}

	static int log_10(int x)
	{
		return (int)floor(log(x)/log(10));
	}

	static void init_zero_tree_array(avl_tree * root, tree_node ** array)
	{
		int i;
		for(i = 0; i< complete_tree_size(root); i++)
		{	
			array[i] = NULL_REF;
		}
	}

	static void fill_array(tree_node * node, tree_node ** array, int pos)
	{
		if(node_exists(node))
		{
			array[pos] = node;
			fill_array(node->left_child, array, 2*pos+1);
			fill_array(node->right_child, array, 2*pos+2);
		}
	}

	static tree_node ** tree_2_array(avl_tree * root)
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
	
	static int node_height(tree_node * node)
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

	static unsigned int integer_string_size(int value)
	{
		if(value == 0)
		{
			return 1;
		}
		else
		{
			if(value < 0)
			{
				value *= -10; //Gives one more 'space'.
			}
			return (unsigned int)floor(log_10(value)) + 1;
		}
	}

	static unsigned int max_integer_string_size(int a, int b)
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
	
	static int node_has_no_children(tree_node * node)
	{
		return !node_exists(node->left_child) && !node_exists(node->right_child);
	}

	static int node_has_only_left_child(tree_node * node)
	{
		return node_exists(node->left_child) && !node_exists(node->right_child);
	}
	
	static int node_has_only_right_child(tree_node * node)
	{
		return !node_exists(node->left_child) && node_exists(node->right_child);
	}

	static int node_has_2_children(tree_node * node)
	{
		return node_exists(node->left_child) && node_exists(node->right_child);
	}

	static tree_node ** node_array(avl_tree * root)
	{
		if(!tree_empty(root))
		{
			return (tree_node **)malloc(sizeof(tree_node) * tree_size(root));
		}
		
		return NULL_REF;
	} 

/**** HELPER FUNCTIONS END ****/

