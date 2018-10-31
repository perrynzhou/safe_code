/*************************************************************************
  > File Name: dict.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 二 10/23 10:42:34 2018
 ************************************************************************/

#ifndef _DICT_H
#define _DICT_H
#include <stdint.h>
#include <stdio.h>
typedef struct dict_item
{
  void *key;
  size_t key_hash;
  union {
    void *ptr;
    uint64_t u64;
    int64_t s64;
    double d;
  } val;
  uint8_t  type;
  struct dict_item *next;
} dict_item;
typedef struct dict
{
  dict_item **table;
  void *context;
  uint8_t  *link_level;
  uint8_t  link_level_size;
  uint64_t table_size;
  uint64_t (*hash_func)(const void *key,size_t len);
  size_t (*key_len)(const void *key);
  int (*key_cmp)(const void *key1, const void *key2);
  void (*key_destroy)(void *key);
  void (*val_destroy)(void *val);
} dict;

dict *dict_create(void *context, uint64_t size,int level_limit_size);
int dict_add(dict *dt, void *key, void *val,int item_type);
void *dict_fetch(dict *dt, void *key);
int dict_del(dict *dt,void *key);
void dict_destroy(dict *d);
#endif
