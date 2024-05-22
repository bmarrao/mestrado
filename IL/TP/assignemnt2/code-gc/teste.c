void copy_collection_gc(HeapBase* hb) 
{
   char* toSpace;
   char* fromSpace;
   if (hb->top >= hb->base + hb->size/2)
   {
      fromSpace = hb->base + hb->size/2;
      toSpace = hb->base;
      hb->limit = hb->base + hb->size/2;
   }
   else 
   {
      fromSpace = hb->base;
      toSpace = hb->base + hb->size/2;
      hb->limit = hb->base + hb->size;

   }
   char* free = toSpace;
   List* workList = (List*)malloc(sizeof(List));

   list_init(workList);
   printf("Roots\n");
   for (int i = 0; i < list_size(roots); i++)
   {
      BisTree* b = (BisTree*) list_get(roots,i);
      char* node = b->root;
      _block_header* q = ((_block_header*)node)-1;

      if (node != NULL )
      {
         b->root = copy(hb,q, &free, workList);
         q->survived++;
      }
   }
   printf("WORKLIST\n");
   char* ref;
   while (!list_isempty(workList))
   {
      //printf("1\n");
      BiTreeNode* node  = list_getfirst(workList);
      list_removefirst(workList);
      _block_header* q = ((_block_header*)node->left)-1;

      //printf("2\n");
      if (node->left != NULL)
      {
         node->left = copy(hb,q, &free, workList);
      }
      //printf("3\n");
      q = ((_block_header*)node->right)-1;
      if (node->right != NULL)
      {
         node->right = copy(hb,q, &free, workList);
      }
   }
   printf(" Previous heap->top %p\n", hb->top);
   printf(" New heap->top %p new heap->limit %p\n", free, hb->limit);
   hb->top = free;
   printf("gcing()...\n");
   return;
}