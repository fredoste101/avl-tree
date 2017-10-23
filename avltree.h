
typedef struct n
{
	int value;
	struct tree_node * lchild;
	struct tree_node * rchild;
} tree_node;

typedef struct t
{
	int size;
	tree_node * tree;
} tree;

/**** BASIC OPERATIONS BEGIN ****/

	tree * create_tree();

	int add_element(tree * root, int value);
	int remove_element(tree * root, int value);

/**** BASIC OPERATIONS END ***/

/**** META DATA FUNCTIONS BEGIN ****/

	int tree_empty(tree * root);
	int tree_size(tree * root);
	int largest_value_in_tree(tree * root);
	int smallest_value_in_tree(tree * root);
	int tree_is_complete(tree * root);
	int value_exists(tree * root, int value);
	int value_in_tree(tree_node * node, int value);
	int complete_tree_size(tree * root);
	
	tree_node ** inorder_node_list(tree * root);

/**** META DATA FUNCTIONS END ****/


/**** PRINT FUNCTIONS BEGIN ****/

	void array_print(tree * root);
	void pretty_print_tree(tree * root);

/**** PRINT FUNCTIONS END ****/

