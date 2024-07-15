#ifndef HASHTAB_H
#define HASHTAB_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define INIT_SIZE 1024

typedef struct {
  char* key;
  void* value;
} ht_e;

typedef struct {
  ht_e** items;
  size_t size;
  size_t n;
} hashtab;

ht_e* init_item(const char* key, void* value);
void free_item(ht_e* item);

hashtab* init_hashtab();
void free_hashtab(hashtab* ht);
void* ht_get(hashtab* ht, const char* key);
void ht_insert(hashtab* ht, const char* key, void* value);
bool ht_delete(hashtab* ht, const char* key);
static uint64_t hash(const char* key);
static bool ht_expand(hashtab* ht);

#endif
