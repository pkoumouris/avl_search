
typedef struct {
  char str[64];
} word_t;

typedef struct {
  word_t word_key;
  char value[128];
  int key;
} ref_t;

typedef struct avl_node_t {
  struct avl_node_t *parent;
  struct avl_node_t *left;
  struct avl_node_t *right;
  void *ref;
  void* key;
  char balance_factor; // shouldn't need more than [-128, 127]
  char height; // shouldn't need more than [-128, 127]
} avl_node_t;

typedef struct {
  unsigned long int count;
  unsigned long int allocated;
  avl_node_t *root_node;
  partial_list_t nodes;
  partial_list_t values;
} avl_tree_t;

void initialise_avl_tree(avl_tree_t *tree, int allocate, size_t el_size);
void free_avl_tree(avl_tree_t *tree);
int avl_resize_tree(avl_tree_t *tree, int add);
int argcmp(void* arg1, void* arg2);

void avl_update_height(avl_node_t *node);
void avl_update_heights(avl_tree_t *tree, avl_node_t *origin);
void avl_key_value_inserts(avl_tree_t *tree, void **new_keys, void* new_values, int num_new);
avl_node_t* avl_insert(avl_tree_t *tree, avl_node_t *new_node);
int avl_balance_factor(avl_node_t *node);
avl_node_t* search_exact(avl_tree_t *tree, void* key);
avl_node_t* search_closest(avl_tree_t *tree, void* key);

avl_node_t* avl_rotate_left(avl_node_t *x, avl_node_t *z);
avl_node_t* avl_rotate_right(avl_node_t *x, avl_node_t *z);
avl_node_t* avl_rotate_left_right(avl_node_t *x, avl_node_t *z);
avl_node_t* avl_rotate_right_left(avl_node_t *x, avl_node_t *z);

int strord(char* str1, char* str2);
void bubble_sort(int list[], int n);

void TEST_new();
void TEST_another();
void strcpylen(char *dest, int len);

void print_avl_stack(avl_tree_t *tree);
