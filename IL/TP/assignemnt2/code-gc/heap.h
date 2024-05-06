/*
 * heap.h
 */

#ifndef HEAP_H
#define HEAP_H

#include "list.h"

typedef struct _block_header _block_header;

typedef struct _block_header
{
   unsigned int marked;
   unsigned int size;
   unsigned int collected;
   _block_header* forwardingAdress;
} _block_header;

typedef struct 
{
   unsigned int size;
   char*        base;
   char*        top;
   char*        limit;
   List*        freeb;
   void (*collector)(List*);
} Heap;

void heap_init(Heap* heap, unsigned int size, void (*collector)(List*));

void heap_destroy(Heap* heap);

void* my_malloc(unsigned int nbytes);

#endif
