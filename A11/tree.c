/*----------------------------------------------
 * Author: Idan Hussain
 * Date: November 30 2024
 * This program implements a binary search tree
 ---------------------------------------------*/

#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node *createNode(const char *name)
{
  struct tree_node *new_node = (struct tree_node *)malloc(sizeof(struct tree_node));
  if (new_node == NULL)
  {
    printf("Memory allocation failed.\n");
    return NULL;
  }
  strncpy(new_node->data.name, name, 64);
  new_node->left = new_node->right = NULL;
  return new_node;
}

struct tree_node *find(const char *name, struct tree_node *root)
{
  if (root == NULL || strncmp(root->data.name, name, 64) == 0)
  {
    return root;
  }

  if (strcmp(name, root->data.name) < 0)
  {
    return find(name, root->left);
  }
  else if (strcmp(name, root->data.name) > 0)
  {
    return find(name, root->right);
  }
  else
  {
    return root;
  }
}

struct tree_node *insert(const char *name, struct tree_node *root)
{
  if (root == NULL)
  {
    return createNode(name);
  }
  if (strcmp(name, root->data.name) < 0)
  {
    root->left = insert(name, root->left);
  }
  else if (strcmp(name, root->data.name) > 0)
  {
    root->right = insert(name, root->right);
  }
  return root;
}

void clear(struct tree_node *root)
{
  if (root == NULL)
  {
    return;
  }

  clear(root->left);
  clear(root->right);

  free(root);
}

void print(struct tree_node *root)
{
  if (root == NULL)
  {
    return;
  }

  printf("%s\n", root->data.name);

  if (root->left)
  {
    printf("l: ");
    print(root->left);
  }
  if (root->right)
  {
    printf("r: ");
    print(root->right);
  }
}

void printSorted(struct tree_node *root)
{
  if (root == NULL)
  {
    return;
  }

  printSorted(root->left);
  printf("%s\n", root->data.name);
  printSorted(root->right);
}
