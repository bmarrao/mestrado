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
   float size ;
   unsigned int n_survive;
   unsigned int type_gc_eden;
   void (*c_eden)(List*);
   unsigned int type_gc_tenured;
   void (*c_tenured)(List*);
   Heap* eden;
   Heap* tenured;
} generation_gc;


typedef struct 
{
   unsigned int size;
   char*        base;
   char*        top;
   char*        limit;
   List*        freeb;
   void (*collector)(List*);
   unsigned int type_collector;
} HeapBase;

typedef struct 
{
   HeapBase*    baseHeap;
   generation_gc* ggc;
} Heap;


void heap_init(Heap* heap, unsigned int size, void (*collector)(List*), unsigned int i,   generation_gc* ggc);

void heap_destroy(Heap* heap);

void* my_malloc(unsigned int nbytes);

void* my_heap_malloc(HeapBase* myHeap ,unsigned int nbytes) ;


#endif
