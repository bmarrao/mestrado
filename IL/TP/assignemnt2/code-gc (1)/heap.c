/*
 * the heap
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "heap.h"
#include "globals.h"
#include "collector.h"


void heap_init(Heap* heap, unsigned int size, void (*collector)(List*), unsigned int i,   generation_gc* ggc)
{
    heap->base  = mmap ( NULL, size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
    heap->size  = size;
    if (i == 3)
    {
        heap->limit = heap->base + size/2;
    }
    else 
    {
        heap->limit = heap->base + size;
    }
    heap->top   = heap->base;
    heap->freeb = (List*)malloc(sizeof(List));
    list_init(heap->freeb);
    heap->collector = collector;
    heap->ggc = ggc;
    heap->type_collector = i;
    return;
}

void heap_destroy(Heap* heap) 
{
    munmap(heap->base, heap->size);
    return;
}

void* getTopHeap(Heap* hb, unsigned int nbytes)
{
    _block_header* q = (_block_header*)(heap->top);
    q->marked = 0;
    q->size   = nbytes;
    q->collected = 0;
    q->survived =0;
    q->forwardingAdress = NULL;
    char *p = hb->top + sizeof(_block_header);
    heap->top = hb->top + sizeof(_block_header) + nbytes;
    return p;
}
void* my_heap_malloc(Heap* hb,unsigned int nbytes) 
{
    if( hb->top + sizeof(_block_header) + nbytes < hb->limit ) 
    {
        return getTopHeap(hb,nbytes);
    } 
    else 
    {
        if (hb->type_collector ==1 )
        {
            printf("my_malloc: not enough space, performing GC...\n");
            hb->collector(hb);
            if ( list_isempty(hb->freeb) ) 
            {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
            void* ret = list_getfirst(hb->freeb);
            _block_header* q ;
            list_removefirst(hb->freeb);
            q = ((_block_header*)ret) - 1;
            q->collected =0;
            q->survived = 0;
            return ret;
        }
        else if(hb->type_collector ==2 )
        {
            printf("my_malloc: not enough space, performing GC...\n");
            hb->collector(hb);
            printf("OUT OF GARBAGE COLLECTOR\n");
            if( hb->top + sizeof(_block_header) + nbytes < hb->limit ) 
            {
                printf("Sucessfully done GC..\n");
                return getTopHeap(hb,nbytes);
            }
            else 
            {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
        
        }
        else if (hb->type_collector == 3)
        {
            printf("my_malloc: not enough space, performing GC...\n");
            hb->collector(hb);
            printf("OUT OF GARBAGE COLLECTOR\n");
            if( hb->top + sizeof(_block_header) + nbytes < hb->limit ) 
            {
                printf("Sucessfully done GC..\n");
                return getTopHeap(hb,nbytes);
            }
            else 
            {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
        }
    }
    return NULL;
}

void* my_malloc(unsigned int nbytes) 
{
    if(heap->ggc == NULL)
    {
        //return my_heap_malloc(heap->baseHeap,nbytes);
        return my_heap_malloc(heap,nbytes);

    }
    else 
    {
        //return my_heap_malloc(heap->ggc->eden, nbytes);
        return NULL;
    }
}
    
