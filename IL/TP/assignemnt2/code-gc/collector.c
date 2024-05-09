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
   BisTree* b = (BisTree*)field;

   char* node = b->root;

   if (node != NULL )
   {
      _block_header* q = ((_block_header*)node)-1;

      list_addlast(workList,node);

      j = mark(workList,j);

   }
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

