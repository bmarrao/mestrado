/*
 * collector.c
 */

#include <stdio.h>

#include "list.h"

void mark_sweep_gc(List* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
   List* isMarked  = (List*)malloc(sizeof(List));
   list_init(isMarked);
   markFromRoots(roots, isMarked);

    */

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
   List* isMarked  = (List*)malloc(sizeof(List));
   list_init(isMarked);
   markFromRoots(roots,isMarked);
    */

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

/*
List* markFromRoots(List* roots, List* isMarked)
{
   List* workList  = (List*)malloc(sizeof(List));
   list_init(workList);
   for (int i = 0; i< list_size(roots); i++)
   {
         BisTree* chosen  = list_get(roots, i);
   }
}
*/
