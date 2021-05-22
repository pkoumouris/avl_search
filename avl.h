
typedef struct {
  char str[64];
  unsigned long int key;
} ref_t;

typedef struct avl_node_t {
  struct avl_node_t *parent;
  struct avl_node_t *left;
  struct avl_node_t *right;
  void *ref;
  long int key; // can change this type
  char balance_factor; // shouldn't need more than [-128, 127]
  char height; // shouldn't need more than [-128, 127]
} avl_node_t;

typedef struct {
  unsigned long int count;
  unsigned long int allocated;
  unsigned int min_delta;
  unsigned int max_delta;
  avl_node_t *root_node;
  //avl_node_t *nodes; // partial_list_t
  partial_list_t nodes;
  partial_list_t values;
} avl_tree_t;

void initialise_avl_tree(avl_tree_t *tree, int allocate, size_t el_size, int min_delta, int max_delta);
void free_avl_tree(avl_tree_t *tree);
int avl_resize_tree(avl_tree_t *tree, int add);

void avl_update_height(avl_node_t *node);
void avl_update_heights(avl_tree_t *tree, avl_node_t *origin);
void avl_key_value_inserts(avl_tree_t *tree, int *new_keys, void* new_values, int num_new);
avl_node_t* avl_insert(avl_tree_t *tree, avl_node_t *new_node);
int avl_balance_factor(avl_node_t *node);
avl_node_t* search_exact(avl_tree_t *tree, int key);
avl_node_t* search_closest(avl_tree_t *tree, int key);

avl_node_t* avl_rotate_left(avl_node_t *x, avl_node_t *z);
avl_node_t* avl_rotate_right(avl_node_t *x, avl_node_t *z);
avl_node_t* avl_rotate_left_right(avl_node_t *x, avl_node_t *z);
avl_node_t* avl_rotate_right_left(avl_node_t *x, avl_node_t *z);

void bubble_sort(int list[], int n);

void TEST_insert();
void TEST_big_volume();
void TEST_new();
void strcpylen(char *dest, int len);

void print_avl_stack(avl_tree_t *tree);
