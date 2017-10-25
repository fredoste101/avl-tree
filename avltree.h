
typedef struct tree_node_struct
{
	int value;
	struct tree_node * lchild;
	struct tree_node * rchild;
} tree_node;

typedef struct avl_tree_struct
{
	int size;
	tree_node * tree;
} avl_tree;

/**** BASIC OPERATIONS BEGIN ****/

	avl_tree * create_tree();

	int add_element(avl_tree * root, int value);
	int remove_element(avl_tree * root, int value);

/**** BASIC OPERATIONS END ***/

/**** META DATA FUNCTIONS BEGIN ****/

	int tree_empty(avl_tree * root);
	int tree_size(avl_tree * root);
	int largest_value_in_tree(avl_tree * root);
	int smallest_value_in_tree(avl_tree * root);
	int tree_is_complete(avl_tree * root);
	int value_exists(avl_tree * root, int value);
	int complete_tree_size(avl_tree * root);
	int tree_height(avl_tree * root);
	
	tree_node ** inorder_node_list(avl_tree * root);
	tree_node ** preorder_node_list(avl_tree * root);
	tree_node ** postorder_node_list(avl_tree * root);

/**** META DATA FUNCTIONS END ****/


/**** PRINT FUNCTIONS BEGIN ****/

	void array_print(avl_tree * root);
	void pretty_print_tree(avl_tree * root);

/**** PRINT FUNCTIONS END ****/

