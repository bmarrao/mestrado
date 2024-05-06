/*
 * collector.c
 */

#include <stdio.h>
#include "globals.h"
#include "list.h"
#include "heap.h"
#include "bistree.h"
#include <stdlib.h>
#include <stddef.h> 

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
void copy_collection_gc() 
{
   List* workList = (List*)malloc(sizeof(List));
   char* fromSpace = heap->base;
   ptrdiff_t extent = (ptrdiff_t)(heap->limit - heap->base) / 2;
   char* toSpace = heap->base + extent;
   char* top = toSpace;
   char* free = fromSpace;

   flip(fromSpace,toSpace,top,extent,free);
   list_init(workList);
   for (int i = 0; i < list_size(roots); i++)
   {
      process((list_get(roots, i)), free, workList);
   }

   char* ref;
   while (!list_isempty(workList))
   {
      void* ref  = list_getfirst(workList);
      list_removefirst(workList);
      scan(ref, free, workList);
   }
   printf("gcing()...\n");
   return;
}

void process (void* field, char* free, List* workList)
{
   // TODO VER AQUI O Q É APONTADOR E O Q NÂO É 
   if (field != NULL)
   {
      field = forward(field , free, workList);
   }   
}

void* forward( _block_header* fromRef, char*free,List* workList)
{
   void* toRef = (fromRef->forwardingAdress);
   if (toRef != NULL)
   {
      toRef = copy(fromRef, free, workList);
   }
   return toRef;
}
void flip(char* fromSpace,char* toSpace,char* top, ptrdiff_t  extent, char* free)
{
   fromSpace = toSpace;
   toSpace = fromSpace; 
   top = fromSpace + extent;
   free = fromSpace;
}

void scan(void *ref, char*free, List* workList)
{
   BiTreeNode* node = ref;
   process(node->left, free, workList);
   process(node->right, free, workList);
}

void* copy( _block_header* fromRef, char* free, List* workList)
{
   void *toRef = free;
   free = free+ fromRef->size;
   memcpy(fromRef, toRef, sizeof(_block_header));
   fromRef->forwardingAdress = toRef;
   list_addlast(workList,toRef);
   return toRef;
}
*/
void markFromRoots(List* roots)
{
   
   List* workList  = (List*)malloc(sizeof(List));
   list_init(workList);
   size_t tamanho = sizeof(_block_header);
   for (int i = 0; i < list_size(roots); i++)
   {
      printf("1 - \n");
      BisTree* b = (BisTree*)(list_get(roots, i));
      printf("2 - \n");
      printf("%d - Size\n", b->size);
      char* node = b->root;
      if (node != NULL )
      {
         printf("3 - %s\n", node);
         //printf("%d - data \n", node->data);
         _block_header* q = ((_block_header*)node)-1;
         q->marked= 1;
         printf("4 - \n");
         list_addlast(workList,node);
         printf("5 - \n");
         mark(workList);
      }
      
   }
   free(workList);
}

void mark(List* workList )
{
   size_t tamanho = sizeof(_block_header);

   while(! list_isempty(workList))
   {
      BiTreeNode* node = list_getfirst(workList);
      _block_header* q = ((_block_header*)node) - 1;
      q->marked=1;
      list_removefirst(workList);
      if (node->left != NULL)
      {
         list_addlast(workList,node->left);
      }
      if (node->right != NULL)
      {
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
         q->collected= 1;
         list_addlast(heap->freeb,q+1);
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
         q->forwardingAdress = free;
         free = free + tamanho + q->size;
      }
      scan = scan + tamanho + q->size;

   }

}

void updateReferences(char* start, char* end, List* roots)
{
   size_t tamanho = sizeof(_block_header);
   for (int i = 0; i < list_size(roots); i++)
   {
      void* b = (list_get(roots, i));
      if (b!=NULL )
      {
         _block_header* q = ((_block_header*)b) - 1;
         b = ((_block_header*)q->forwardingAdress) + 1;
      }
   }
   char* scan = start;
   while (scan < end)
   {
      _block_header* q = (_block_header*)(scan);
      BiTreeNode* node = (BiTreeNode*)(q+1);

      if (q->marked == 1)
      {
         if (node->left != NULL)
         {
            q= ((_block_header*)node->left) - 1;
            node->left = q->forwardingAdress;
         }
         if (node->right != NULL)
         {
            q= ((_block_header*)node->right) - 1;
            node->right = q->forwardingAdress;
         }
      }
      scan = scan + tamanho + q->size;

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
         void* dest = q->forwardingAdress;
         memcpy(dest, (q + 1), q->size);
         q->marked = 0;
      }
      scan = scan + sizeof(_block_header) + q->size;
   }
}

