#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include "gen_queue.h"
#include "util.h"

#define DEFAULT_Q_SIZE 100
#define THREAD_SAFE

int nulls_ok = 0;

/******************************************************************************/
/******************************************************************************/
GEN_Q *create_queue(int capacity){
  const char* METHOD_NM = "create_queue";
  GEN_Q *queue = (GEN_Q *)malloc(sizeof(GEN_Q));;

  if(capacity <= 0){
    capacity = DEFAULT_Q_SIZE;
  }

  queue->capacity = capacity;
  queue->queue_elements = (Q_ELEM *)malloc(sizeof(Q_ELEM)*capacity);
  memset(queue->queue_elements, 0, sizeof(Q_ELEM)*capacity);

  if(queue->queue_elements == NULL){
    printf("%s malloc failed", METHOD_NM);
    return;
  }

  queue->size = 0;
  queue->tail_id = 0;
  queue->curr_id = 0;

  return queue;

}

/******************************************************************************/
/* Increments the tail */
/******************************************************************************/
BOOL add(GEN_Q *queue, void *element){
  if(queue == NULL || element == NULL){
    return FALSE;
  }

  /* if at capacity, can't add more */
  if(queue->size >= queue->capacity){
    return FALSE;
  }

  /* check if we are the end of the circular queue */
  if(queue->tail_id >= queue->capacity){
    queue->tail_id = 1;
  } else {
    queue->tail_id++;
  }

  queue->size++;
  queue->queue_elements[queue->tail_id - 1].id = queue->tail_id;
  queue->queue_elements[queue->tail_id - 1].used = FALSE;
  queue->queue_elements[queue->tail_id - 1].value = element;

  return TRUE;
}

/******************************************************************************/
/* get_next returns the next element and iterates the curr pointer            */
/* This is a circular queue                                                   */
/******************************************************************************/

/* having problems with this function */
void *get_next(GEN_Q *queue){
  BOOL used = FALSE;
  int next_id = 0;

  if(queue == NULL){
    return NULL;
  }

  next_id = queue->curr_id + 1;

  /* reset for circular queue */
  if(next_id > queue->capacity){
    next_id = 1;
  }

  if(queue->curr_id == queue->tail_id || next_id == queue->tail_id){
    return NULL;
  }

  used = queue->queue_elements[queue->curr_id - 1].used; 
  if(used){
    return NULL;
  }

  queue->curr_id = next_id;
  queue->size--;
  queue->queue_elements[queue->curr_id - 1].used = TRUE;

  return queue->queue_elements[queue->curr_id - 1].value;
}

/******************************************************************************/
/******************************************************************************/
void *get_element(int index){
  return NULL;
}

/******************************************************************************/
/******************************************************************************/
void print_queue(GEN_Q *queue){
  void *element = NULL;
  char *elem = NULL;

  int size = queue->size;
  
  int i = 0;
  while((element = get_next(queue)) != NULL){
    elem = (char *)element;
    printf("Element %d [%s]\n",i,elem);
    i++;
  }
  printf("\n");

}

void print_actuals(GEN_Q *queue){
  printf(" capacity [%d]\n", queue->capacity);
  printf(" size [%d]\n", queue->size);
  printf(" tail_id [%d]\n", queue->tail_id);
  printf(" curr_id [%d]\n\n", queue->curr_id);
}