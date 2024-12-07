/*----------------------------------------------
 * Author: Idan Hussain
 * Date: December 6 2024
 * This program implements malloc using a free list
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk
{
  int size;
  int used;
  struct chunk *next;
};

struct chunk *flist = NULL;

void *malloc(size_t size)
{
  if (size == 0)
  {
    return NULL;
  }

  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL)
  {
    if (next->size >= size)
    {
      if (prev != NULL)
      {
        prev->next = next->next;
      }
      else
      {
        flist = next->next;
      }
      next->used = size;
      return (void *)(next + 1);
    }
    prev = next;
    next = next->next;
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *)-1)
  {
    return NULL;
  }

  struct chunk *new_chunk = (struct chunk *)memory;
  new_chunk->size = size;
  new_chunk->used = size;
  new_chunk->next = NULL;
  return (void *)(new_chunk + 1);
}

void free(void *memory)
{
  if (memory == NULL)
  {
    return;
  }

  struct chunk *free_chunk = (struct chunk *)((char *)memory - sizeof(struct chunk));
  free_chunk->next = flist;
  flist = free_chunk;
}