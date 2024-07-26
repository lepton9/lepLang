#include "../include/stack.h"
#include <stdlib.h>

stack* init_stack() {
  stack* s = malloc(sizeof(stack));
  s->front = 0;
  s->back = 0;
  s->size = 0;
  return s;
}

int full(stack* stack) {
  return (stack->size == MAX_SIZE);
}

int empty(stack* stack) {
  return (stack->size == 0);
}

void push(stack* stack, void* value) {
  if (full(stack)) {
    printf("Stack is full -> %d\n", stack->size);
    return;
  }
  if (empty(stack)) {
    stack->front = 0;
    stack->back = 0;
  }
  stack->data[stack->back] = value;
  stack->size++;
  stack->back++;
  if (stack->back == MAX_SIZE) stack->back = 0;
}

void* pop(stack* stack) {
  if (empty(stack)) {
    return NULL;
  }
  void* value = stack->data[stack->front];
  stack->front++;
  if (stack->front == MAX_SIZE) stack->front = 0;
  stack->size--;
  return value;
}

void clear(stack* stack) {
  while(!empty(stack)) {
    pop(stack);
  }
  stack->front = 0;
  stack->back = 0;
}
