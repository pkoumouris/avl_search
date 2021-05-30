#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "partial_list.h"
#include "avl.h"

/*
Makefile:
gcc -o avl avl.c partial_list.c
*/

int main(int argc, char* argv[]){
  /*avl_tree_t tree;
  initialise_avl_tree(&tree, 20, 50);*/
  //TEST_insert();
  TEST_another();
  //avl_resize_tree(&tree, 500);
  return 0;
}




void TEST_another(){
  srand(320948);
  avl_tree_t tree;
  int i, j, len = 150;
  ref_t refs[len];
  for (i = 0 ; i < len ; i++){
    j = rand() % 32565;
    refs[i].key = j;
    sprintf(refs[i].word_key.str, "%d", (int)refs[i].key);
    if (i == 125){
      printf("At 125 it is %d\n", (int)refs[i].key);
    }
  }
  initialise_avl_tree(&tree, 100, sizeof(ref_t));
  //char **strs = malloc(len*sizeof(char));
  word_t *words[len];
  for (i = 0 ; i < len ; i++){
    words[i] = (word_t*)&(refs[i].word_key);
  }
  avl_key_value_inserts(&tree, (void*)words, (void*)refs, len);

  avl_node_t* node = search_closest(&tree, (void*)(&(refs[125].word_key)));
  printf("Node key: %s\n", ((word_t*)node->key)->str);
}

void TEST_new(){
  srand(219831290);

  avl_tree_t tree;
  int i, j, len = 150;
  ref_t refs[len];
  for (i = 0 ; i < len ; i++){
    j = rand() % 32565;
    if (i < 5){
      printf("i%d = %d, ", i, j);
    }
    refs[i].key = j;
    sprintf(refs[i].word_key.str, "%d", (int)refs[i].key);
  }
  printf("A1\n");
  initialise_avl_tree(&tree, len, sizeof(ref_t));
  printf("A2\n");
  int *keys[len];
  for (i = 0 ; i < len ; i++){
    keys[i] = &(refs[i].key);
  }
  printf("**first key: %d\n", **keys);
  avl_key_value_inserts(&tree, (void*)keys, (void*)refs, len);
  printf("A3, root node key = %d\n", (int)tree.root_node->key);

  avl_node_t* node = search_closest(&tree, (void*)(&(refs[35].key)));
  printf("Node key: %d, ref key: %d, root key: %d\n", *((int*)node->key), (int)refs[35].key, *((int*)tree.root_node->key));

  /*avl_node_t* node = search_closest(&tree, 5000);
  printf("Node key = %d, valstr = %s\n", (int)node->key, ((ref_t*)(node->ref))->str);
  print_avl_stack(&tree);*/
  //printf("Ref key %d, node value %d\n", (int)refs[13].key, (int)node->key);
}

// may be doubling up in main file -- consider removing from main file in make
void strcpylen(char *dest, int len){
  int i = 0;
  while (i < len){
    dest[i] = 'a';
    i++;
  }
  dest[i] = '\0';
}

avl_node_t* search_exact(avl_tree_t *tree, void* key){
  avl_node_t *node = tree->root_node;
  int cmp;
  while (1){
    //printf("A");
    cmp = argcmp(key, node->key);
    if (!cmp){//key == node->key){ // argcmp == 0
      return node;
    } else if (cmp < 0){//key > node->key){
      // go right
      if (node->right == NULL){ // argcmp
        return NULL;
      }
      node = node->right;
    } else if (cmp > 0){//key < node->key){ // argcmp
      // go left
      if (node->left == NULL){
        return NULL;
      }
      node = node->left;
    }
  }
}

avl_node_t* search_closest(avl_tree_t *tree, void* key){
  avl_node_t *node = tree->root_node;
  int cmp;
  while (1){
    //printf("B");
    cmp = argcmp(key, node->key);
    if (!cmp){//key == node->key){ // argcmp == 0
      return node;
    } else if (cmp < 0){//key > node->key){ // argcmp
      // go right
      if (node->right == NULL){
        return node;
      }
      node = node->right;
    } else if (cmp > 0){//key < node->key){ // argcmp
      // go left
      if (node->left == NULL){
        return node;
      }
      node = node->left;
    }
  }
  return NULL;
}

void avl_update_height(avl_node_t *node){
  if (node->left == NULL){
    if (node->right == NULL){
      node->height = 0;
    } else {
      node->height = node->right->height + 1;
    }
  } else if (node->right == NULL){
    node->height = node->left->height + 1;
  } else {
    node->height = (node->left->height > node->right->height ? node->left->height : node->right->height) + 1;
  }
}

void avl_update_heights(avl_tree_t *tree, avl_node_t *origin){
  avl_node_t *node = origin;
  int ct = 0;
  while (1){
    ct++;
    avl_update_height(node);
    if (tree->root_node == node){
      break;
    }
    node = node->parent;
  }
}

void avl_key_value_inserts(avl_tree_t *tree, void **new_keys, void* new_values, int num_new){
  appends_partial_list(&(tree->values), new_values, num_new);
  ref_t* rr = (ref_t*)access_partial_list_element(&(tree->values), 0);

  avl_node_t nodes[num_new];
  int i;
  for (i = 0 ; i < num_new ; i++){
    nodes[i].key = new_keys[i]; // even deref'd new_keys is a void* ptr
    nodes[i].ref = access_partial_list_element(&(tree->values), tree->count + i);
    nodes[i].parent = NULL;
    nodes[i].left = NULL;
    nodes[i].right = NULL;
    nodes[i].height = 0;
  }

  appends_partial_list(&(tree->nodes), nodes, num_new);
  if (tree->count == 0){
    tree->root_node = (avl_node_t*)access_partial_list_element(&(tree->nodes), 0);
    tree->count = 1;
    i = 1;
  } else {
    i = 0;
  }

  for ( ; i < num_new ; i++){
    avl_insert(tree, (avl_node_t*)access_partial_list_element(&(tree->nodes), tree->count));
  }
}

avl_node_t* avl_insert(avl_tree_t *tree, avl_node_t *new_node){
  avl_node_t *node = search_closest(tree, new_node->key);

  avl_node_t *added_node = (avl_node_t*)access_partial_list_element(&(tree->nodes), tree->count);//&(tree->nodes[tree->count]);
  tree->count += 1;
  if (argcmp(new_node->key, node->key) < 0){//new_node->key > node->key){ // argcmp
    node->right = added_node;
  } else {
    node->left = added_node;
  }
  added_node->parent = node;
  added_node->left = NULL;
  added_node->right = NULL;
  added_node->height = 0;
  avl_update_heights(tree, node);
  while (1){
      if (avl_balance_factor(node) > 1){
        if (avl_balance_factor(node->right) < 0){
          node = avl_rotate_right_left(node, node->right);
          if (node->left == tree->root_node){
            tree->root_node = node;
          }
          avl_update_height(node->right);
          avl_update_heights(tree, node->left);
        } else {
          node = avl_rotate_left(node, node->right);
          if (node->left == tree->root_node){
            tree->root_node = node;
          }
          avl_update_heights(tree, node->right);
        }
      } else if (avl_balance_factor(node) < -1){
        if (avl_balance_factor(node->left) > 0){
          node = avl_rotate_left_right(node, node->left);
          if (node->right == tree->root_node){
            tree->root_node = node;
          }
          avl_update_height(node->left);
          avl_update_heights(tree, node->right);
        } else {
          node = avl_rotate_right(node, node->left);
          if (node->right == tree->root_node){
            tree->root_node = node;
          }
          avl_update_heights(tree, node->left);
        }
      }
    if (tree->root_node == node){
      break;
    }
    node = node->parent;
  }
  return node;
}

void initialise_avl_tree(avl_tree_t *tree, int allocate, size_t el_size){
  initialise_partial_list(&(tree->nodes), allocate + 10, sizeof(avl_node_t));
  initialise_partial_list(&(tree->values), allocate + 10, el_size);
  tree->allocated = allocate;
  tree->root_node = NULL;
  tree->count = 0;
}

void free_avl_tree(avl_tree_t *tree){
  free_partial_list(&(tree->nodes));
  free_partial_list(&(tree->values));
}

avl_node_t* avl_rotate_left(avl_node_t *x, avl_node_t *z){
  avl_node_t *t23 = z->left;
  x->right = t23;
  if (t23 != NULL){
    t23->parent = x;
  }
  z->left = x;
  z->parent = x->parent;
  avl_node_t *anc = x->parent;
  if (anc != NULL){
    // change ancestor
    if (anc->left == x){
      anc->left = z;
    } else {
      anc->right = z;
    }
  }
  x->parent = z;

  if (x->left == NULL && x->right == NULL){
    x->height = 0;
  } else if (x->right == NULL){
    x->height = x->left->height + 1;
  } else {
    x->height = x->right->height + 1;
  }

  if (z->left == NULL){
    z->height = z->right->height + 1;
  } else {
    z->height = (z->left->height > z->right->height ? z->left->height : z->right->height) + 1;
  }

  return z;
}

avl_node_t* avl_rotate_right(avl_node_t *x, avl_node_t *z){
  avl_node_t *t23 = z->right;
  x->left = t23;
  if (t23 != NULL){
    t23->parent = x;
  }
  z->right = x;
  z->parent = x->parent;
  avl_node_t *anc = z->parent;
  if (anc != NULL){
    if (anc->left == x){
      anc->left = z;
    } else {
      anc->right = z;
    }
  }
  x->parent = z;

  if (x->left == NULL && x->right == NULL){
    x->height = 0;
  } else if (x->left == NULL) {
    x->height = x->right->height + 1;
  } else {
    x->height = x->left->height + 1;
  }

  if (z->left == NULL){
    z->height = z->right->height + 1;
  } else {
    z->height = (z->left->height > z->right->height ? z->left->height : z->right->height) + 1;
  }

  return z;
}

avl_node_t* avl_rotate_left_right(avl_node_t *x, avl_node_t *z){
  avl_node_t *y = z->right;
  avl_node_t *t2 = y->left;
  z->right = t2;
  if (t2 != NULL){
    t2->parent = z;
  }
  y->left = z;
  z->parent = y;
  avl_node_t *t3 = y->right;
  x->left = t3;
  if (t3 != NULL){
    t3->parent = x;
  }
  y->right = x;
  avl_node_t *anc = x->parent;
  if (anc != NULL){
    if (anc->left == x){
      anc->left = y;
    } else {
      anc->right = y;
    }
  }
  x->parent = y;
  y->parent = anc;

  return y;
}

avl_node_t* avl_rotate_right_left(avl_node_t *x, avl_node_t *z){
  avl_node_t *y = z->left;
  avl_node_t *t3 = y->right;
  z->left = t3;
  if (t3 != NULL){
    t3->parent = z;
  }
  y->right = z;
  z->parent = y;
  avl_node_t *t2 = y->left;
  x->right = t2;
  if (t2 != NULL){
    t2->parent = x;
  }
  y->left = x;
  avl_node_t *anc = x->parent;
  if (anc != NULL){
    if (anc->left == x){
      anc->left = y;
    } else {
      anc->right = y;
    }
  }
  x->parent = y;
  y->parent = anc;

  return y;
}

int avl_balance_factor(avl_node_t *node){
  if (node->left == NULL){
    if (node->right == NULL){
      return 0;
    } else {
      return node->right->height + 1;
    }
  } else if (node->right == NULL){
    return -1 * (node->left->height + 1);
  } else {
    return node->right->height - node->left->height;
  }
}

void bubble_sort(int list[], int n){
  int c, d, t;
  for (c = 0 ; c < n - 1; c++) {
    for (d = 0 ; d < n - c - 1; d++) {
      if (list[d] > list[d+1]) {
        /* Swapping */
        t = list[d];
        list[d] = list[d+1];
        list[d+1] = t;
      }
    }
  }
}

int argcmp(void* arg1, void* arg2){
  // Customise your comparison here
  // arg1 before arg2: return +ve, arg2 before arg1: return -ve, arg1 == arg2: return 0
  // Simple example for ints:
  // return *((int*)arg1) == *((int*)arg2) ? 0 : (*((int*)arg1) < *((int*)arg2) ? 1 : -1);
  // Simple example for strings:
  return strord((char*)arg1, (char*)arg2);
}

int strord(char* str1, char* str2){
  // positive if str1 before str2, negative if str2 before str1, 0 if same
  // ASSUMES char is between 97 and 122 (lower case)
  int i = 0;
  while (1){
    if (str1[i] == '\0'){
      return str2[i] == '\0' ? 0 : 1;
    } else if (str2[i] == '\0'){
      return -1;
    } else if (str1[i] < str2[i]){
      return 1;
    } else if (str2[i] < str1[i]){
      return -1;
    }
    i++;
  }
}

void print_avl_stack(avl_tree_t* tree){
  int levels = 3;
  int i, j, k, e = 1;
  int bitmask;
  avl_node_t* node;
  printf("AVL stack:\n");
  for (i = 0 ; i < levels ; i++){
    // for each level, print out the line
    for (j = 0 ; j < e ; j++){
      // for each node on the level
      node = tree->root_node;
      bitmask = (1 << (i));
      for (k = 0 ; k < i ; k++){
        // for each node in the path to that node
        if (k & bitmask){
          // go right
          node = node->right;
        } else {
          // go left
          node = node->left;
        }
        bitmask = bitmask >> 1;
      }
      if (node == NULL){
        printf(" NULL ");
      } else {
        printf(" %d ", (int)node->key);
      }
    }
    printf("\n");
    e *= 2;
  }
}
