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
   unsigned int survived;
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
   unsigned int type_collector;
   generation_gc* ggc;
} Heap;

typedef struct 
{   
   char* tenured;
   unsigned int n_surive;
   void (*c_eden)(List*);
   void (*c_tenured)(List*);

} generation_gc;

void heap_init(Heap* heap, unsigned int size, void (*collector)(List*), unsigned int i,   generation_gc* ggc);

void heap_destroy(Heap* heap);

void* my_malloc(unsigned int nbytes);

#endif
