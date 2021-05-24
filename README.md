# AVL self-balancing tree
AVL self-balancing binary search tree with basic unrolled linked list memory management.

Each node has a key, which is ordered, and a value associated with that key. The value can be of any data type, including a predefined struct. You simply need to define the size of the type upon initialisation, cast the type as a `(void*)` pointer type when inputting, and cast the output value as your data type.

Suppose you have some integer key value that you want to order that links to some struct reference `ref_t`.

## Initialisation
All data (both key and value reference) is held or pointed to by an `avl_tree_t` type.

To initialise, create a tree, set the max elements per unrolled linked list node (say 500), and set the size of the reference value type:

```
avl_tree_t tree;
initialise_avl_tree(&tree, maxElements, sizeof(ref_t), 0, 0);
```

## Adding elements

To add n elements, with keys in an n-length `keys` array and reference values in an n-length `vals` array:

```
avl_key_value_inserts(&tree, keys, (void*)vals, n);
```

## Searching
To find the AVL node closest to ```key```:

```
avl_node_t* node = search_closest(&tree, key);
```

From this node pointer, to find the returned key, use ```node->key``` (will return as an ```int```). To find the value reference:

```
ref_t* val = (ref_t*)node->ref;
```

## Notes
* Still need to add delete node function (as of 24 May 2021)
* Could make the key based on a custom type by making it a void pointer in the `avl_node_t` struct, and allowing comparison on the basis of a customisable function that accepts two `void` pointers but casts them as the key type pointer in the function for comparison
* This code is designed to be flexible, and thus relies on `void` pointers. As a consequence, it won't compile with C++.
