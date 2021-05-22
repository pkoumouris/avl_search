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
  TEST_new();
  //avl_resize_tree(&tree, 500);
  return 0;
}


// Got to fix up TESTs
void TEST_insert(){
  /*avl_tree_t tree;
  //int len = 8;
  int vals[8] = {19, 4, 11, 2, 6, 14, 20, 13};
  ref_t x[8];
  int i;
  for (i = 0 ; i < 8 ; i++){
    x[i].key = vals[i];
    strcpylen(x[i].str, vals[i]);
  }
  avl_node_t new_node;
  initialise_avl_tree(&tree, 50, 500);
  for (i = 0 ; i < 8 ; i++){
    new_node.key = vals[i];
    new_node.ref = x[i].str;
    printf("\n--- starting %d (adding %d)---\n", i, (int)new_node.key);
    avl_insert(&tree, new_node);
    print_avl_stack(&tree);
  }
  avl_node_t *res = search_exact(&tree, 14);
  printf("Result: %s\n", res->ref);*/
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
    //strcpy(refs[i].str, "\naaa");
    sprintf(refs[i].str, "%d", (int)refs[i].key);
  }
  printf("A1\n");
  initialise_avl_tree(&tree, len, sizeof(ref_t), 100, 50);
  printf("A2\n");
  int keys[len];
  for (i = 0 ; i < len ; i++){
    keys[i] = refs[i].key;
  }
  avl_key_value_inserts(&tree, keys, (void*)refs, len);
  printf("A3, root node key = %d\n", (int)tree.root_node->key);
  avl_node_t* node = search_closest(&tree, 5000);
  printf("Node key = %d, valstr = %s\n", (int)node->key, ((ref_t*)(node->ref))->str);
  //printf("Ref key %d, node value %d\n", (int)refs[13].key, (int)node->key);
}

void TEST_big_volume(){
  /*srand(1203843829);
  avl_tree_t tree;
  int i, j, len = 256;//100000;
  int vals[len];
  for (i = 0 ; i < len ; i++){
    vals[i] = rand() % 32656;
  }
  initialise_avl_tree(&tree, 1000, 5000);
  avl_node_t new_node;
  for (i = 0 ; i < len ; i++){
    if (1){
      printf("\n===\nUp to %d (adding %d)\n===\n", i, (int)vals[i]);
    }
    print_avl_stack(&tree);
    new_node.key = vals[i];
    new_node.ref = NULL;
    avl_insert(&tree, new_node);
  }
  //printf("Finished!\n");
  avl_node_t *res = search_exact(&tree, vals[98]);
  printf("Val: %d, result: %d\n", vals[98], (int)res->key);*/
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

avl_node_t* search_exact(avl_tree_t *tree, int key){
  avl_node_t *node = tree->root_node;
  while (1){
    printf("A");
    if (key == node->key){
      return node;
    } else if (key > node->key){
      // go right
      if (node->right == NULL){
        return NULL;
      }
      node = node->right;
    } else if (key < node->key){
      // go left
      if (node->left == NULL){
        return NULL;
      }
      node = node->left;
    }
  }
}

avl_node_t* search_closest(avl_tree_t *tree, int key){
  avl_node_t *node = tree->root_node;
  while (1){
    if (key == node->key){
      return node;
    } else if (key > node->key){
      // go right
      if (node->right == NULL){
        return node;
      }
      node = node->right;
    } else if (key < node->key){
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

void avl_key_value_inserts(avl_tree_t *tree, int *new_keys, void* new_values, int num_new){
  appends_partial_list(&(tree->values), new_values, num_new);

  avl_node_t nodes[num_new];
  int i;
  for (i = 0 ; i < num_new ; i++){
    nodes[i].key = new_keys[i];
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
  if (new_node->key > node->key){
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

void initialise_avl_tree(avl_tree_t *tree, int allocate, size_t el_size, int min_delta, int max_delta){
  initialise_partial_list(&(tree->nodes), allocate + 10, sizeof(avl_node_t));
  initialise_partial_list(&(tree->values), allocate + 10, el_size);
  tree->allocated = max_delta;
  tree->root_node = NULL;
  tree->max_delta = max_delta;
  tree->min_delta = min_delta;
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
