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

    if (ggc == NULL)
    {
        HeapBase* hb = malloc(sizeof(HeapBase));
        hb->base  = mmap ( NULL, size, PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
        hb->size  = size;
        if (i == 3)
        {
            hb->limit = hb->base + size/2;
        }
        else 
        {
            hb->limit = hb->base + size;
        }
        hb->top   = hb->base;
        hb->freeb = (List*)malloc(sizeof(List));
        list_init(hb->freeb);
        hb->collector = collector;
        hb->type_collector = i;
        heap->baseHeap = hb;
        heap->ggc = ggc;
    }
    else 
    {
        ggc->eden = (Heap*)malloc(sizeof(Heap));
        heap_init(ggc->eden, size * ggc->size, ggc->c_eden, ggc->type_gc_eden,NULL);
        ggc->tenured = (Heap*)malloc(sizeof(Heap));
        heap_init(ggc->tenured, size - (size * ggc->size), ggc->c_tenured, ggc->type_gc_tenured,NULL);
        heap->ggc = ggc;
        heap->baseHeap = NULL;
    }
    
    return;
}

/*
void heap_destroy(Heap* heap) 
{

    if (heap->ggc == NULL)
    {
        munmap(heap->baseHeap->base, heap->baseHeap->size);
    }
    else 
    {
        // TODO ISSO TALVEZ DE ERRO , NÂO SEI SE NAO PRECISO DESTRUIR CADA HEAP INDIVIDUAL
        munmap(heap->ggc->eden->base, heap->ggc->tenured>limit);
    }
    munmap(heap->base, heap->size);
    return;
}

*/

void* getTopHeap(HeapBase* hb,unsigned int nbytes)
{
    _block_header* q = (_block_header*)(hb->top);
    q->marked = 0;
    q->size   = nbytes;
    q->collected = 0;
    q->survived =0;
    q->forwardingAdress = NULL;
    char *p = hb->top + sizeof(_block_header);
    hb->top = hb->top + sizeof(_block_header) + nbytes;
    return p;
}

void* my_heap_malloc(HeapBase* myHeap ,unsigned int nbytes) 
{
    if( myHeap->top + sizeof(_block_header) + nbytes < myHeap->limit ) 
    {
        return getTopHeap(myHeap,nbytes);
    } 
    else 
    {

        if (myHeap->type_collector ==1 )
        {
            printf("my_malloc: not enough space, performing GC...\n");
            myHeap->collector(roots);
            if ( list_isempty(myHeap->freeb) ) 
            {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
            void* ret = list_getfirst(myHeap->freeb);
            _block_header* q ;
            list_removefirst(myHeap->freeb);
            q = ((_block_header*)ret) - 1;
            q->collected =0;
            q->survived = 0;
            return ret;
        }
        else if(myHeap->type_collector ==2 )
        {
            printf("my_malloc: not enough space, performing GC...\n");
            myHeap->collector(roots);
            printf("OUT OF GARBAGE COLLECTOR\n");
            if( myHeap->top + sizeof(_block_header) + nbytes < myHeap->limit ) 
            {
                printf("Sucessfully done GC..\n");
                return getTopHeap(myHeap,nbytes);
            }
            else 
            {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
        
        }
        else if (myHeap->type_collector == 3)
        {
            printf("my_malloc: not enough space, performing GC...\n");
            myHeap->collector(roots);
            printf("OUT OF GARBAGE COLLECTOR\n");
            if( myHeap->top + sizeof(_block_header) + nbytes < myHeap->limit ) 
            {
                printf("Sucessfully done GC..\n");
                return getTopHeap(myHeap,nbytes);
            }
            else 
            {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
        }
        else 
        {
            
        }

    }
}


void* my_malloc(unsigned int nbytes) 
{
    if(heap->ggc == NULL)
    {
        return my_heap_malloc(heap->baseHeap,nbytes);
    }
    else 
    {
        return my_heap_malloc(heap->ggc->eden, nbytes);
    }
}

