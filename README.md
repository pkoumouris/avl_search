# AVL self-balancing tree
AVL self-balancing binary search tree with unrolled linked list memory management.

This code is good if you have a volume of incoming key-value pairs (perhaps as a stochastic process) that must be both stored and the keys ordered for fast logarithmic searching. An example might be a keyword search engine, with the keys being the keywords and the values being the addresses.

You can use any data type for both the key and value, including predefined structs (for strings as keys, structs are recommended). You just need to pass in the key and value arguments cast as `(void*)` pointers.

Suppose you have a key struct as:
```
typedef struct {
  char str[64];
} word_t;
```
and a value included in a reference struct as:
```
typedef struct {
  word_t word_key;
  char value[128];
} ref_t;
```

## Setting the comparison function
While you can arbitrarily set the data type of the key, a function must be employed to set key order. You need to do this yourself in the `argcmp` function, which takes two `(void*)` pointer arguments:

```
int argcmp(void* arg1, void* arg2){
  /*
      Do your comparison here, and don't forget to cast the (void*) pointers
      as your key data type. An example for integer comparison is shown:
  */
  return *((int*)arg1) == *((int*)arg2) ? 0 : (*((int*)arg1) < *((int*)arg2) ? 1 : -1);
}
```

You simply need to make the function return a positive value if `arg1` comes before `arg2`, a negative value if `arg2` comes before `arg1`, and 0 if the arguments are equal.

A simple string order function `strord` is provided, which can be called in `argcmp` by casting the respective arguments as `(char*)` pointers.

## Initialisation
All data (both key and value reference) is held or pointed to by an `avl_tree_t` type.

To initialise, create a tree, set the max elements per unrolled linked list node (say 500), and set the size of the reference value type:

```
avl_tree_t tree;
initialise_avl_tree(&tree, maxElements, sizeof(ref_t));
```

## Adding elements

To add n elements from a reference struct `ref_t refs[n]` array (with each element containing both key and value), you will need to pass the keys in as a separate array of key-type pointers (in the example case, `(word_t*)`), with each element pointing to the key at that index in the reference struct array. You can create that pointer array as such:

```
word_t *keys[n];
for (int i = 0 ; i < n ; i++){
  keys[i] = (word_t*)&(refs[i].word_key);
}
```

You can then insert the reference structs and keys to be added to the list and the tree, and the tree to be balanced:

```
avl_key_value_inserts(&tree, (void*)keys, (void*)refs, n);
```

## Searching
To logarithmically find the AVL node closest to `word_t key` we need to pass the key address in as a `(void*)` pointer:

```
avl_node_t* node = search_closest(&tree, (void*)&key);
```

The function `search_closest` (as well as `search_exact`) returns an `(avl_node_t*)` pointer contained in `tree`. From this node pointer, to find the returned key, use:

```
word_t* returned_word = (word_t*)node->key;
printf("The returned word was %s\n", returned_word->str);
```

To find the corresponding value in the reference struct:

```
ref_t* val = (ref_t*)node->ref;
printf("The value is %s\n", val->value);
```

## Notes
* Still need to add delete node function (as of 24 May 2021)
* This code is designed to be flexible, and thus relies on `void` pointers. As a consequence, it won't compile with C++.
* You can roughly print the AVL tree by passing its pointer into the `print_avl_stack` function and setting the number of levels within that function in the `int levels` variable.
