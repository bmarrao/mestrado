/*
 * bistree.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "bool.h"
#include "bistree.h"
#include "heap.h"

void bistree_init(BisTree* tree) {
    tree->root = NULL;
    tree->size = 0;
    return;
}

bool bitreenode_lookup(BiTreeNode* node, int data) 
{
   if (node == NULL)
      return false;
   if (data < node->data)
      return bitreenode_lookup(node->left, data);
   if (data > node->data)
      return bitreenode_lookup(node->right, data);
   return true;
}

bool bistree_lookup(BisTree* tree, int data) {
    return bitreenode_lookup(tree->root, data);
}

BiTreeNode* bitreenode_insert(BiTreeNode* node, int data, BiTreeNode *pointer) 
{
   //printf("in this insert \n");
   if (node == NULL) 
   {
      //printf("pointer - %p\n", pointer);
      pointer->data = data;
      pointer->left = NULL;
      pointer->right= NULL;
      return pointer;
   }
   else if(data < node->data)
   {
   node->left = bitreenode_insert(node->left, data,pointer);

   }
   else
   {
   node->right = bitreenode_insert(node->right, data,pointer);
   }
   //printf("End Insert\n");
   return node;
}

bool bistree_insert(BisTree* tree, int data,BiTreeNode *pointer) 
{
   if (bistree_lookup(tree, data))
      return false;
   
   tree->root = bitreenode_insert(tree->root, data,pointer);
   tree->size = tree->size + 1;
   return true;
}

BiTreeNode* bitreenode_remove(BiTreeNode* node, int data) {
   if(data < node->data)
      node->left = bitreenode_remove(node->left, data);
   else if(data > node->data)
      node->right = bitreenode_remove(node->right, data);
   else if(node->left == NULL)
      node = node->right;
   else if(node->right == NULL)
      node = node->left;
   else {
     BiTreeNode* lnode = node->left;
     while(lnode->right != NULL)
        lnode = lnode->right;
     node->data = lnode->data;
     node->left = bitreenode_remove(node->left, lnode->data);
   }
   return node;
}

bool bistree_remove(BisTree* tree, int data) {
   if (!bistree_lookup(tree, data))
      return false;
   tree->root = bitreenode_remove(tree->root, data);
   tree->size = tree->size - 1;
   return true;
}

void bitreenode_inorder(BiTreeNode* node) {
   if(node == NULL)
      return;
   bitreenode_inorder(node->left);
   bitreenode_inorder(node->right);
}

void bistree_inorder(BisTree* tree) {
   bitreenode_inorder(tree->root);
}
