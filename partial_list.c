#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partial_list.h"

// This is a mem alloc test

void TEST1();

//int main(int argc, char* argv[]){
  /*partial_list_t list;
  initialise_partial_list(&list, 100, sizeof(content_t));
  content_t news[LENGTH];
  int i;
  for (i = 0 ; i < LENGTH ; i++){
    news[i].x = i*2;
    news[i].y = i*3;
  }
  printf("List content pointer = %d\n", (int)list.elements);

  appends_partial_list(&list, (void*)news, LENGTH);
  printf("Length is %d\n", list_length(&list));
  content_t *res = (content_t*)access_partial_list_element(&list, INDEX);
  printf(" res pointer = %d\n", (int)res);
  printf("Result x = %d, x = %d, should be %d\n", res->x, ((content_t*)(list.elements + list.el_size*INDEX))->x, news[INDEX].x);
  free_partial_list(&list);*/
//  TEST1();
//  return 0;
//}

void TEST1(){
  partial_list_t list;
  int x[15] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28};
  initialise_partial_list(&list, 10, sizeof(int));
  appends_partial_list(&list, (void*)x, 15);
  remove_partial_list(&list, 2);
  int r = *((int*)access_partial_list_element(&list, 5));
  printf("r = %d\n", r);
}

void* access_partial_list_element(partial_list_t* list, int index){
  while (1){
    //printf("Z list length %d ind %d\n",list->length, index);
    if (list->length > index){
      return (void*)(list->elements + list->el_size*index);
    }
    index -= list->length;
    list = list->next;
    if (list == NULL){
      return NULL;
    }
  }
  return NULL;
}

int list_length(partial_list_t* root_list){
  int length = 0;
  partial_list_t* list = root_list;
  while (1){
    length += list->length;
    if (list->next == NULL){
      break;
    } else {
      list = list->next;
    }
  }
  return length;
}

void appends_partial_list(partial_list_t* list, void* new_els, int num_new){
  int i = 0;
  while (1){
    if (list->allocated - list->length > 0){
      if (list->allocated - list->length < num_new - i){
        //printf("a %d ", ((content_t*)(new_els + i*list->el_size))->x);
        memcpy((void*)(list->elements + list->el_size*list->length), (new_els + i*list->el_size), (list->allocated - list->length)*list->el_size);
        //printf("%d\n",((content_t*)(list->elements + list->el_size*4))->x);//((content_t*)(list->elements + list->el_size*list->length))->x);
        i += (list->allocated - list->length);
        list->length = list->allocated;
      } else {
        memcpy((void*)(list->elements + list->el_size*list->length), (new_els + i*list->el_size), (num_new - i)*list->el_size);
        list->length += num_new - i;
        i += num_new - i;
      }
    }
    if (num_new > i){
      if (list->next == NULL){
        // link a new list
        list->next = malloc(sizeof(partial_list_t));
        list->next->allocated = list->allocated; //assume same allocation as previous
        list->next->el_size = list->el_size;
        list = list->next;
        list->elements = malloc(list->allocated * list->el_size);
        list->length = 0;
        list->next = NULL;
      }
    } else {
      break;
    }
  }
}

int remove_partial_list(partial_list_t* root_list, int index){
  int i = 0, j;
  partial_list_t* list = root_list;
  while (1){
    if (index < i + list->length){
      // remove
      for (j = index - i ; j < list->length - 1 ; j++){
        // memcpy here 1 space back
        memcpy((void*)(list->elements + list->el_size*j), (void*)(list->elements + list->el_size*(j+1)), (size_t)list->el_size);
      }
      list->length--;
      return index;
    } else {
      if (list->next != NULL){
        i += list->length;
        list = list->next;
      } else {
        return -1;
      }
    }
  }
}

void free_partial_list(partial_list_t* root_list){
  partial_list_t* list = root_list;
  int i = 0, n = 0;
  while (list->next != NULL){
    list = list->next;
    n++;
  }
  partial_list_t* dels[n];
  list = root_list;
  while (list->next != NULL){
    dels[i] = list;
    list = list->next;
    i++;
  }
  for (i = n - 1 ; i >= 0 ; i--){
    list = dels[i];
    free(list->elements);
    if (i < n - 1){
      free(list->next);
    }
  }
  // Cannot free root partial list here
}

void initialise_partial_list(partial_list_t* list, int allocate, size_t el_size){
  list->next = NULL;
  //char* tmp = (char*)malloc(allocate*el_size);
  list->elements = malloc(allocate*el_size);
  list->allocated = allocate;
  list->el_size = el_size;
  list->length = 0;
}
