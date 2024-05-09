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

void generational_gc() 
{

}
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

   printf("MarkFromRoots\n");
   markFromRoots(roots);
   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */
   printf("Compact\n");   
   compact(heap->base,heap->limit,roots);

   printf("gcing()...\n");
   return;
 }


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
      BisTree* b = (BisTree*) list_get(roots,i);
      char* node = b->root;

      if (node != NULL )
      {
         b->root = forward(b->root, free, workList);
      }
   }

   char* ref;
   while (!list_isempty(workList))
   {
      BiTreeNode* node  = list_getfirst(workList);
      list_removefirst(workList);
      node->left = forward(node->left, free, workList);
      node->right = forward(node->right, free, workList);
   }
   printf("gcing()...\n");
   return;
}

/*
void* forward( _block_header* fromRef, char*free,List* workList)
{
   TAKING THIS OUT BECAUSE IT ISNT NECESSARY AS THE SAME OBJECT ISNT REFERENCED MORE THAN ONCE
   void* toRef = (fromRef->forwardingAdress);
   if (toRef != NULL)
   {
      toRef = copy(fromRef, free, workList);
   }
   return toRef;
}
*/

void flip(char* fromSpace,char* toSpace,char* top, ptrdiff_t  extent, char* free)
{
   fromSpace = toSpace;
   toSpace = fromSpace; 
   top = fromSpace + extent;
   free = fromSpace;
}


void* copy( _block_header* fromRef, char* free, List* workList)
{
    _block_header* toRef = free;
   free = free+ fromRef->size + sizeof(_block_header);
   memcpy(fromRef+1, toRef+1, toRef->size);
   // TAKING THIS OUT AS WELL fromRef->forwardingAdress = toRef;
   list_addlast(workList,toRef);
   return toRef;
}

void markFromRoots(List* roots)
{
   List* workList  = (List*)malloc(sizeof(List));
   list_init(workList);
   size_t tamanho = sizeof(_block_header);
   int j = 0;
   for (int i = 0; i < list_size(roots); i++)
   {
      BisTree* b = (BisTree*)(list_get(roots, i));

      char* node = b->root;

      if (node != NULL )
      {
         _block_header* q = ((_block_header*)node)-1;

         list_addlast(workList,node);

         j = mark(workList,j);

      }
      
   }
   printf(" Marked : %d\n", j);
   free(workList);
}

int mark(List* workList , int j)
{
   size_t tamanho = sizeof(_block_header);

   while(! list_isempty(workList))
   {

      BiTreeNode* node = list_getfirst(workList);

      _block_header* q = ((_block_header*)node) - 1;
      j++;
      q->marked=1;
      q->survived++;

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
   return j;
}


void sweep(char* start, char* end)
{
   size_t tamanho = sizeof(_block_header);
   char* scan = start;
   int i  = 0;
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
      i++;
   }
   printf(" Total : %d", i);
}


void compact (char* start, char* end, List* roots)
{
   printf("Compute Locations\n");
   computeLocations(start,end,start);
   printf("Update References\n");
   updateReferences(start,end,roots);
   printf("Relocate\n");
   relocate(start,end);
}

void computeLocations(char* start, char* end, char* toRegion)
{
   size_t tamanho = sizeof(_block_header);
   char * scan = start;
   char * free = toRegion;
   int i  = 0;
   int j = 0;
   while (scan < end )
   {
      _block_header* q = (_block_header*)(scan);
      if (q->marked == 1)
      {
         q->forwardingAdress = ((_block_header*)free)+1;
         free = free + tamanho + q->size;
         i++;
      }
      scan = scan + tamanho + q->size;
      j++;

   }
   printf("%d %d %d\n", i, (j-i), j);
   /*
   while (free < end )
   {
      _block_header* q = (_block_header*)(free);
      if (q->collected == 0 )
      {
         q->collected= 1;
         list_addlast(heap->freeb,q+1);
      }
      free = free + tamanho + q->size;

   }
   */
   printf(" Previous heap->top %p\n", heap->top);
   printf(" New heap->top %p\n", free);
   heap->top = free;
}

void updateReferences(char* start, char* end, List* roots)
{
   size_t tamanho = sizeof(_block_header);
   for (int i = 0; i < list_size(roots); i++)
   {
      BisTree* b = (BisTree*)(list_get(roots, i));
      char* node = b->root;

      if (node != NULL )
      {
         _block_header* q = ((_block_header*)node) - 1;
         b->root = q->forwardingAdress;
      }
   }
   char* scan = start;
   while (scan < end)
   {
      _block_header* q = (_block_header*)(scan);
      if (q->marked == 1)
      {
         void* pointer = q+1; 
         BiTreeNode* node = (BiTreeNode*)(pointer);
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
         //TALVEZ Q+1 esteja errado
         memcpy(dest, (q + 1), q->size);
         q->marked = 0;
      }
      scan = scan + sizeof(_block_header) + q->size;
   }
}

