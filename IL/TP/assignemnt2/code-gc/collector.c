/*
 * collector.c
 */

// cd mestrado/IL/TP/assignemnt2/code-gc/

#include <stdio.h>
#include "globals.h"
#include "list.h"
#include "heap.h"
#include "bistree.h"
#include <stdlib.h>
#include <stddef.h> 
#include <errno.h>
#include <string.h>
int count_collector = 0;
size_t tamanho = sizeof(_block_header);

void relocate(char* start, char* end);
void* moveToTenured(_block_header* toMove);
void markFromRoots(HeapBase* hb, List* roots);
void sweep(HeapBase*hb, char* start, char* end);
void compact (HeapBase* hb ,char* start, char* end, List* roots);
void computeLocations(HeapBase* hb,char* start, char* end, char* toRegion);
void updateReferences(char* start, char* end, List* roots);
int mark(HeapBase* hb, List* workList , int j);

void mark_sweep_gc(HeapBase* hb) 
{
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
   */
   printf("MarkFromRoots\n");
   markFromRoots(hb,roots);

   /*
    * sweep phase:
    * go through entire heap,
    * add unmarked to free list
    */
   printf("Sweep\n");   
   sweep(hb, hb->base, hb->top);

   printf("gcing()...\n");
   return;
 }

void sweep(HeapBase*hb, char* start, char* end)
{
   //TODO TEM ALGO DE ERRADO AQUI
   char* scan = start;
   int i  = 0;
   int j = 0;
   while (scan < end)
   {
      _block_header* q = (_block_header*)(scan);
      if (q->marked == 0 && q->collected == 0)
      {
         q->collected= 1;
         list_addlast(hb->freeb,q+1);
      }
      else 
      {
         j++;
         q->marked = 0;
      }
      scan = scan + tamanho + q->size;
      i++;
   }
   printf(" Marked %d : Total : %d\n", j, i);
}


void mark_compact_gc(HeapBase* hb)
{
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */

   printf("MarkFromRoots\n");
   markFromRoots(hb,roots);
   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */
   printf("Compact\n");   
   compact(hb,hb->base,hb->top,roots);

   printf("gcing()...\n");
   return;
}


void markFromRoots(HeapBase* hb, List* roots)
{
   List* workList  = (List*)malloc(sizeof(List));
   list_init(workList);
   
   Heap* eden ;
   if(heap->ggc != NULL)
   {
      eden = heap->ggc->eden;
   }
   int j = 0;
   for (int i = 0; i < list_size(roots); i++)
   {

      BisTree* b = (BisTree*)(list_get(roots, i));
      char* node = (char*)b->root;

      if (node != NULL )
      {

         _block_header* q = ((_block_header*)node)-1;
         char* compare_header = (char*)q;

         if (hb->base <= compare_header && compare_header  <= hb->limit)
         {
            q->marked=1;
            q->survived++;
            j++;
            if (heap->ggc != NULL)
            {
               if(heap->ggc->n_survive == q->survived &&  eden->baseHeap->base <= compare_header && compare_header <= eden->baseHeap->limit)
               {
                  b->root= moveToTenured(q);
                  // TALVEZ ALTERAR ESSA PARTE 
                  q->marked = 0 ;
                  q->survived++;
               }
            }

         }
         list_addlast(workList,b->root);
         j = mark(hb,workList,j);
      }
      
   }
   free(workList);
}

int mark(HeapBase* hb, List* workList , int j)
{
   
   Heap* eden ;
   if(heap->ggc != NULL)
   {
      eden = heap->ggc->eden;
   }
   while(! list_isempty(workList))
   {
      BiTreeNode* node = list_getfirst(workList);
      list_removefirst(workList);
      _block_header*  q = ((_block_header*)node->left) - 1;
      if (node->left != NULL && q->marked != 1)
      {
         char* compare_header = (char*)q;

         if (hb->base <= compare_header && compare_header<= hb->limit)
         {
            q->marked=1;
            q->survived++;
            j++;
            if (heap->ggc != NULL)
            {
               if(heap->ggc->n_survive == q->survived && eden->baseHeap->base <= compare_header && compare_header <= eden->baseHeap->limit)
               {
                  node->left= moveToTenured(q);
                  // TALVEZ ALTERAR ESSA PARTE 
                  q->marked = 0 ;
                  q->survived++;
               }
            }
         }
         
         list_addlast(workList,node->left);
      }
      q = ((_block_header*)node->right) - 1;


      if (node->right != NULL && q->marked != 1)
      {
         char* compare_header = (char*)q;

         if (hb->base <= compare_header && compare_header <= hb->limit)
         {
            q->marked=1;
            q->survived++;
            j++;
            if (heap->ggc != NULL)
            {
               
               if(heap->ggc->n_survive == q->survived &&  eden->baseHeap->base <= compare_header && compare_header<= eden->baseHeap->limit)
               {
                  node->right= moveToTenured(q);
                  // TALVEZ ALTERAR ESSA PARTE 
                  q->marked = 0 ;
                  q->survived++;
               }

            }
         }
         list_addlast(workList,node->right);
      }

   
   }
   return j;
}




void compact (HeapBase* hb ,char* start, char* end, List* roots)
{
   printf("Compute Locations\n");
   computeLocations(hb,start,end,start);
   printf("Update References\n");
   updateReferences(start,end,roots);
   printf("Relocate\n");
   relocate(start,end);
}

void computeLocations(HeapBase* hb,char* start, char* end, char* toRegion)
{
   
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

   printf(" Previous heap->top %p\n", hb->top);
   printf(" New heap->top %p new heap->limit %p\n", free, hb->limit);
   hb->top = free;

}

void updateReferences(char* start, char* end, List* roots)
{
   
   for (int i = 0; i < list_size(roots); i++)
   {
      BisTree* b = (BisTree*)(list_get(roots, i));
      char* node = (char*)b->root;
      if (node != NULL )
      {
         _block_header* q = ((_block_header*)node) - 1;
         b->root = (BiTreeNode *)q->forwardingAdress;
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

            node->left = (BiTreeNode *)q->forwardingAdress;
         }

         if (node->right != NULL)
         {
            q= ((_block_header*)node->right) - 1;
            node->right = (BiTreeNode *)q->forwardingAdress;
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
         _block_header*  dest = ((_block_header*) q->forwardingAdress)-1;
         memcpy(dest,q, sizeof(BiTreeNode)+tamanho);
         dest->marked = 0;
         q->marked = 0;
      }
      scan = scan + tamanho + q->size;
   }
}

void* moveToTenured(_block_header* toMove)
{
   Heap* tenured= heap->ggc->tenured;
   _block_header* pointer =(_block_header*) my_heap_malloc(tenured->baseHeap,toMove->size);

   if (pointer == NULL)
   {
      fprintf(stderr, "Error: NO SPACE IN TENURED\n");
      exit(EXIT_FAILURE);
      return NULL;
   }
   else 
   {
      size_t size_transfer = sizeof(BiTreeNode)+sizeof(_block_header);
      memcpy(pointer-1, toMove, size_transfer);
      return pointer;
   }

}

void* normal_copy(HeapBase* hb, _block_header* fromRef, char** free, List* workList)
{
   _block_header* toRef = (_block_header*)*free;
   BiTreeNode* node =(BiTreeNode*) (fromRef+1);

   *free = *free + fromRef->size + tamanho;

   memcpy(toRef, fromRef, sizeof(BiTreeNode)+sizeof(_block_header));
   list_addlast(workList,toRef+1);
   node = (BiTreeNode*)(toRef+1);
   return toRef+1;
}
void* copy(HeapBase* hb, _block_header* fromRef, char** free, List* workList)
{
   if (heap->ggc != NULL)
   {
      fromRef->survived++;
      char* compareRef = (char*) fromRef;
      Heap* eden = heap->ggc->eden;
      if (hb->base <= compareRef && compareRef < (hb->top))
      {
         if(eden->baseHeap == hb && heap->ggc->n_survive == fromRef->survived)
         {
            fromRef->survived++;
            void* pointer = moveToTenured(fromRef);
            list_addlast(workList,pointer);
            return pointer;
         }
      }
      else 
      {
         list_addlast(workList,fromRef+1);
         return fromRef+1;
      }
   }
   return normal_copy(hb,fromRef,free,workList);
}
      

void copy_collection_gc(HeapBase* hb) 
{
   char* toSpace;
   char* fromSpace;

   if (hb->limit == hb->base + hb->size/2)
   {
      fromSpace = hb->base;
      toSpace = hb->base + hb->size/2;
      hb->limit = hb->base + hb->size;
   }
   else 
   {
      fromSpace = hb->base + hb->size/2;
      toSpace = hb->base;
      hb->limit = hb->base + hb->size/2;

   }
   char* free = toSpace;
   List* workList = (List*)malloc(sizeof(List));
   list_init(workList);
   printf("Roots\n");
   for (int i = 0; i < list_size(roots); i++)
   {
      BisTree* b = (BisTree*) list_get(roots,i);
      char* node = (char*)b->root;
      _block_header* q = ((_block_header*)node)-1;

      if (node != NULL )
      {
         b->root = copy(hb,q, &free, workList);
      }

   }
   printf("WORKLIST\n");
   while (!list_isempty(workList))
   {
      BiTreeNode* node  = list_getfirst(workList);
      list_removefirst(workList);
      _block_header* q = ((_block_header*)node->left)-1;
      
      if (node->left != NULL)
      {
         node->left = copy(hb,q, &free, workList);
      }
      q = ((_block_header*)node->right)-1;

      if (node->right != NULL)
      {
         node->right = copy(hb,q, &free, workList);
      }
   }
   count_collector++;
   printf(" Previous heap->top %p\n", hb->top);
   printf("FromSpace %p , toSpace %p , New heap->top %p new heap->limit %p, size allocated : %zd total space : %d percentage : %d\n",
       fromSpace, toSpace, free, hb->limit, (size_t)(hb->limit - free), hb->size/2,
       (int)(((size_t)(hb->limit - free) * 100) / (hb->size/2)));   hb->top = free;
   printf("gcing()...\n");
   return;
}
