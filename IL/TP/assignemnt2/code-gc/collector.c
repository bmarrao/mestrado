/*
 * collector.c
 */

#include <stdio.h>
#include "globals.h"
#include "list.h"
#include "heap.h"
#include "bistree.h"
#include <stdlib.h>

void mark_sweep_gc() {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
   */

   printf("MarkFromRoots\n");
   markFromRoots(roots);


   /*
    * sweep phase:
    * go through entire heap,
    * add unmarked to free list
    */
   printf("Sweep\n");   
   sweep(heap->base, heap->limit);

   printf("gcing()...\n");
   return;
 }

void mark_compact_gc() {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */


   markFromRoots(roots);
   //compact(heap->base,heap->limit,roots);
   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */
   printf("gcing()...\n");
   return;
 }

/*
void copy_collection_gc() {


   char* fromSpace = heap->base;
   char* extent = (heap->limit - heap->base)/ 2;
   char* toSpace = heap->base + extent;
   char* top = toSpace;
   char* free = fromSpace;
   flip(fromSpace,toSpace,top,extent,free);

   List* workList  = (List*)malloc(sizeof(List));
   list_init(workList);
   for (int i = 0; i < list_size(roots); i++)
   {
      process((list_get(roots, i)));
   }

   char* ref;
   while (!list_isempty(workList))
   {
      
   }
   printf("gcing()...\n");
   return;
}

void process (void* field)
{
   if (*field != NULL)
   {

   }   
}

void flip(char* fromSpace,char* toSpace,char* top, char* extent, char* free)
{
   fromSpace = toSpace;
   toSpace = fromSpace; 
   top = fromSpace + extent;
   free = fromSpace;
}
*/
void markFromRoots(List* roots)
{
   
   List* workList  = (List*)malloc(sizeof(List));
   list_init(workList);
   size_t tamanho = sizeof(_block_header);
   for (int i = 0; i < list_size(roots); i++)
   {
      BisTree* b = (BisTree*)(list_get(roots, i));
      BiTreeNode* node = b->root;
      _block_header* q = ((_block_header*)node)-1;
      q->marked= 1;
      list_addlast(workList,node);
      mark(workList);
   }
   free(workList);
}

void mark(List* workList )
{
   size_t tamanho = sizeof(_block_header);



   while(! list_isempty(workList))
   {
      BiTreeNode* node = list_get(workList, 0);
      _block_header* q = ((_block_header*)node) - 1;
      q->marked=1;
      list_removefirst(workList);
      if (node->left != NULL)
      {
         q= ((_block_header*)node->left) - 1;
         q->marked=1;
         list_addlast(workList,node->left);
      }
      if (node->right != NULL)
      {
         q= ((_block_header*)node->right) - 1;
         q->marked=1;
         list_addlast(workList,node->right);
      }

   
   }
}


void sweep(char* start, char* end)
{
   size_t tamanho = sizeof(_block_header);
   char* scan = start;
   while (scan < end)
   {
      _block_header* q = (_block_header*)(scan);
      if (q->marked == 0 && q->collected == 0)
      {
         list_addlast(heap->freeb,q+1);
      }
      else 
      {
         q->marked = 0;
         q->collected=1;
      }
      scan = scan + tamanho + q->size;
   }
}
/*

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
   for (int i = 0; i < list_size(roots); i++)
   {
      BisTree* b = (BisTree*)(list_get(roots, i));
      if (b!=NULL )
      {
         b = forwardinAdress(b);
      }
   }
   char* scan = start;
   while (scan < end )
   {
      _block_header* q = (_block_header*)(scan);
      if(q->marked == 1)
      {

      }
   }
}

void relocate(char* start, char* end)
{
   char* scan = start ;
   while (scan < end)
   {
      _block_header* q = (_block_header*)(scan);
      if (q->marked==1 )
      {
         char * dest = forwardingAdress(scan);
         move(scan,dest);
         q->marked= 0;
      }
      scan = scan + sizeof(_block_header) + q->size;
   }
}

*/