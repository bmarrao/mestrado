/*
 * the mutator
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "bool.h"
#include "heap.h"
#include "list.h"
#include "bistree.h"
#include "globals.h"
#include "collector.h"

#define  MAX_NODES      100
#define  MIN_NODES      5
#define  MAX_KEY_VALUE  100

#define HEAP_SIZE    (1024 * 1024)  /* 1 MByte */

Heap* heap;
List* roots;

static bool mutate;

static void sigint_handler() {
   mutate = false;
}

int main(int argc, char** argv) {
   /*
    * ^C to stop the program 
    */
   if (signal(SIGINT, sigint_handler) == SIG_ERR) {
      printf("signal: cannot install handler for SIGINT\n");
      return 1;
   }

   float threshold = atof(argv[1]);  /* a value in the interval (0,1) */

   heap  = (Heap*)malloc(sizeof(Heap));
   roots = (List*)malloc(sizeof(List));
   if (argc > 3)
   {
      float size = atof(argv[2]);  /* a value in the interval (0,1) */
      int n_survive = atoi(argv[3]);
      generation_gc* ggc = (generation_gc*)malloc(sizeof(generation_gc));

      if (atoi(argv[4]) == 1 && atoi(argv[5]) == 1)
      {
         generation_gc_init(ggc,HEAP_SIZE,size,n_survive,1,mark_sweep_gc,1,mark_sweep_gc);
      }
      else if (atoi(argv[4]) == 1 && atoi(argv[5]) == 3)
      {
         generation_gc_init(ggc,HEAP_SIZE,size,n_survive,1,mark_sweep_gc,3,copy_collection_gc);
      }
      else if (atoi(argv[4]) == 2 && atoi(argv[5]) == 2)
      {
         generation_gc_init(ggc,HEAP_SIZE,size,n_survive,1,mark_compact_gc,3,mark_compact_gc);
      }
      else if (atoi(argv[4]) == 3 && atoi(argv[5]) == 1)
      {
         generation_gc_init(ggc,HEAP_SIZE,size,n_survive,3,copy_collection_gc,1,mark_sweep_gc);
      }
      else 
      {
         generation_gc_init(ggc,HEAP_SIZE,size,n_survive,3,copy_collection_gc,3,copy_collection_gc);
      }

      heap_init(heap, HEAP_SIZE, NULL,0,ggc);

   }
   else 
   {
      if (atoi(argv[2]) == 1)
      {
         heap_init(heap, HEAP_SIZE, mark_sweep_gc,1,NULL);
      }
      else if (atoi(argv[2]) == 2)
      {
         heap_init(heap, HEAP_SIZE, mark_compact_gc,2,NULL);
      }
      else 
      {
         heap_init(heap , HEAP_SIZE, copy_collection_gc,3,NULL);
      }
   }

   list_init(roots);

   srandom(getpid());
   mutate = true;
   BiTreeNode* pointer = (BiTreeNode*)my_malloc(sizeof(BiTreeNode));
   while(mutate) {
      float toss = (float)random() / RAND_MAX;
      if( toss > threshold ) 
      {
          // add nodes
         /* build new bistree */
         BisTree* t = (BisTree*)malloc(sizeof(BisTree));
         bistree_init(t);
         /* preserve root */
         list_addlast(roots,t);
         /* prepare to insert up to 100 nodes, a minimum of 5 */
         int number_nodes = MIN_NODES + random() % (MAX_NODES - MIN_NODES);
         bool inserted ;
         for(int i = 0; i < number_nodes; i++) 
         {            
            //printf("Before insert - %p \n", pointer);
            /* populate tree with keys between 0-100 */
            inserted = bistree_insert(t, random() % MAX_KEY_VALUE, pointer);
            //printf("after insert\n");
            if (inserted)
            {
               pointer = (BiTreeNode*)my_malloc(sizeof(BiTreeNode));
               if (pointer == NULL)
               {
                  printf("END OF SPACE\n");
                  return -1;
               }
            }
         }
         //fprintf(stdout, "INSERTED tree size is %d\n", bistree_size(t));

         //fprintf(stdout, "(inorder traversal)\n");
         //bistree_inorder(t);
      } 
      else 
      { // remove nodes
         /* skip if there are no roots to manipulate */
         if (list_isempty(roots))
            continue;
         /* otherwise, choose random root to operate on */
         int index = random() % list_size(roots);
         BisTree* chosen  = list_get(roots, index);
         int number_nodes = bistree_size(chosen);
         int number_tries = random() % number_nodes;
         for(int i = 0; i < number_tries; i++ ) {
             /* remove key from tree if key exists in it */
             /* this is checked in bistree_remove */
             bistree_remove(chosen, random() % MAX_KEY_VALUE);
         }
         //fprintf(stdout, "REMOVED tree size is %d was %d\n", bistree_size(chosen),number_nodes);
         //fprintf(stdout, "(inorder traversal)\n");
         //bistree_inorder(chosen);
      }
   }
   /* caught ^C ! */
   printf("quiting...\n");
   heap_destroy(heap);
   return 0;
}
