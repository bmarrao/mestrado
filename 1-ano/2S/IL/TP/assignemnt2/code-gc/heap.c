/*
 * the heap
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "heap.h"
#include "globals.h"
#include "collector.h"

void generation_gc_init(generation_gc* ggc,unsigned int heap_size,float size,unsigned int n_survive,unsigned int type_gc_eden,void (*c_eden)(HeapBase*),unsigned int type_gc_tenured,void (*c_tenured)(HeapBase*)) 
{
    ggc->size = size;
    ggc->n_survive = n_survive;
    ggc->type_gc_eden = type_gc_eden;
    ggc->c_eden = c_eden;
    ggc->type_gc_tenured = type_gc_tenured;
    ggc->c_tenured = c_tenured;
    ggc->eden = malloc(sizeof(Heap));
    ggc->tenured = malloc(sizeof(Heap));
    int size_eden_heap = heap_size * size;
    heap_init(ggc->eden,size_eden_heap,c_eden,type_gc_eden,NULL);
    heap_init(ggc->tenured, heap_size - size_eden_heap, c_tenured, type_gc_tenured, NULL);
}

void heap_init(Heap* heap, unsigned int size, void (*collector)(HeapBase*), unsigned int i, generation_gc* ggc) {
    if (ggc == NULL) {
        HeapBase* hb = malloc(sizeof(HeapBase));
        hb->base  = mmap ( NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        hb->size  = size;
        if (i == 3) 
        {
            hb->limit = hb->base + (size / 2);
        } 
        else
        {
            hb->limit = hb->base + size;
        }
        hb->top = hb->base;
        hb->freeb = (List*)malloc(sizeof(List));
        list_init(hb->freeb);
        hb->collector = collector;
        hb->type_collector = i;
        heap->baseHeap = hb;
        heap->ggc = ggc;
    } 
    else 
    {
        heap->ggc = ggc;
        heap->baseHeap = NULL;
    }
    return ;
}

void my_heap_destroy(HeapBase* heap) 
{
    munmap(heap->base, heap->size);
    return ;
}

void heap_destroy(Heap* heap) 
{
    if (heap->ggc != NULL) 
    {
        Heap* eden = heap->ggc->eden;
        Heap* tenured = heap->ggc->tenured;
        my_heap_destroy(eden->baseHeap);
        my_heap_destroy(tenured->baseHeap);
    }
    else
    {
        my_heap_destroy(heap->baseHeap);
    }
}

void* getTopHeap(HeapBase* hb, unsigned int nbytes) 
{
    _block_header* q = (_block_header*)(hb->top);
    q->marked = 0;
    q->size = nbytes;
    q->collected = 0;
    q->survived = 0;
    q->forwardingAdress = NULL;
    char* p = hb->top + sizeof(_block_header);
    hb->top = hb->top + sizeof(_block_header) + nbytes;
    return p;
}

void* my_heap_malloc(HeapBase* hb, unsigned int nbytes) 
{
    if (hb->top + sizeof(_block_header) + nbytes < hb->limit) 
    {
        return getTopHeap(hb, nbytes);
    } 
    else 
    {
        if (hb->type_collector == 3) 
        {
            printf("my_malloc: not enough space, performing GC...\n");
            hb->collector(hb);
            printf("OUT OF GARBAGE COLLECTOR\n");
            if (hb->top + sizeof(_block_header) + nbytes < hb->limit)
            {
                printf("Successfully done GC..\n");
                return getTopHeap(hb, nbytes);
            }
            else 
            {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
        } 
        else if (hb->type_collector == 1) 
        {
            if (list_isempty(hb->freeb)) 
            {
                printf("my_malloc: not enough space, performing GC...\n");
                hb->collector(hb);
                if (list_isempty(hb->freeb))
                {
                    printf("my_malloc: not enough space after GC...\n");
                    return NULL;
                }
            }
            void* ret = list_getfirst(hb->freeb);
            _block_header* q = ((_block_header*)ret) - 1;
            list_removefirst(hb->freeb);
            q->collected = 0;
            q->survived = 0;
            return ret;
        } 
        else if (hb->type_collector == 2) 
        {
            printf("my_malloc: not enough space, performing GC...\n");
            hb->collector(hb);
            printf("OUT OF GARBAGE COLLECTOR\n");
            if (hb->top + sizeof(_block_header) + nbytes < hb->limit)
            {
                printf("Successfully done GC..\n");
                return getTopHeap(hb, nbytes);
            } else {
                printf("my_malloc: not enough space after GC...\n");
                return NULL;
            }
        }
    }
    return NULL;
}

void* my_malloc(unsigned int nbytes) 
{
    if (heap->ggc == NULL) 
    {
        return my_heap_malloc(heap->baseHeap, nbytes);
    } 
    else 
    {
        Heap* eden = heap->ggc->eden;
        return my_heap_malloc(eden->baseHeap, nbytes);
    }
}
