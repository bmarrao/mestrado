/*
 * collector.c
 */

#include <stdio.h>
#include "globals.h"
#include "list.h"
#include "heap.h"
#include "bistree.h"

void mark_sweep_gc(List* roots, Heap* heap) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
   */

   markFromRoots(roots);

   sweep(heap->base, heap->limit);


   /*
    * sweep phase:
    * go through entire heap,
    * add unmarked to free list
    */
   printf("gcing()...\n");
   return;
 }

void mark_compact_gc(List* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */


   markFromRoots(roots);
   compact(heap->base,heap->limit,roots);
   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */
   printf("gcing()...\n");
   return;
 }

void copy_collection_gc(List* roots) {
   /*
    * go throught all roots,
    * traverse trees in from_space,
    * copy reachable to to_space
    */
   printf("gcing()...\n");
   return;
}


void markFromRoots(List* roots)
{
   list_print(roots);

   List* workList  = (List*)malloc(sizeof(List));
   list_init(workList);
   size_t tamanho = sizeof(_block_header);
   for (int i = 0; i < list_size(roots); i++)
   {

      _block_header* q = (_block_header*)(list_get(roots, i) - tamanho);
      q->marked= 1;
      list_addlast(workList,list_get(roots,i));
      mark(workList);
   }
   free(workList);
}

void mark(List* workList )
{
   size_t tamanho = sizeof(_block_header);
   size_t tamanho1 = sizeof(BisTree);
   size_t tamanho2 = sizeof(BiTreeNode);


   while(! list_isempty(workList))
   {
      _block_header* q = (_block_header*)(list_get(workList, 0) - tamanho);
      list_removefirst(workList);
      if (q->size == tamanho1 )
      {
         BisTree* b = (BisTree*)(q+tamanho);
         q= (_block_header*)(b->root - tamanho);
         q->marked=1;
         list_addlast(workList,b->root);
      }
      else 
      {
         BiTreeNode* b = (BiTreeNode*)(q+tamanho);
         q= (_block_header*)(b->left - tamanho);
         q->marked=1;
         list_addlast(workList,b->left);
         q= (_block_header*)(b->right - tamanho);
         q->marked=1;
         list_addlast(workList,b->right);
      }
   }
}

typedef struct 
{
   unsigned int size;
   char*        base;
   char*        top;
   char*        limit;
   List*        freeb;
   void (*collector)(List*);
} Heap;

void sweep(char* start, char* end)
{
   size_t tamanho = sizeof(_block_header);
   char* scan = start;
   while (scan < end)
   {
      _block_header* q = (_block_header*)(scan);
      if (q->marked == 0)
      {
         free(q);
         free(q+tamanho);
      }
      else 
      {
         q->marked = 0;
      }
      scan = scan + tamanho + q->size;
   }
}

void compact (char* start, char* end, List* roots)
{
   computeLocations(start,end,start);
   updateReferences(start,end,roots);
   relocate(start,end);
}

void computeLocations(char* start, char* end, char* toRegion)
{
   size_t tamanho = sizeof(_block_header);
   char * scan = start;
   char * free = toRegion;
   while (scan < end )
   {
      _block_header* q = (_block_header*)(scan);
      if (q->marked = 1)
      {
         //TODO forwardinAdress ?
         free = free + tamanho + q->size;
      }
      scan = scan + tamanho + q->size;

   }

}

void updateReferences(char* start, char* end, List* roots)
{

}

void relocate(char* start, char* end)
{

}
