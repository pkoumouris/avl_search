
#define LENGTH 850
#define INDEX 732

typedef struct {
  int x;
  int y;
} content_t;

typedef struct partial_list_t {
  int length;
  int allocated;
  size_t el_size;
  struct partial_list_t *next;
  void* elements;
} partial_list_t;

void initialise_partial_list(partial_list_t* list, int allocate, size_t el_size);
void free_partial_list(partial_list_t* root_list);
void appends_partial_list(partial_list_t* root_list, void* new_els, int num_new);
int remove_partial_list(partial_list_t* root_list, int index);
void* access_partial_list_element(partial_list_t* list, int index);
int list_length(partial_list_t* root_list);
