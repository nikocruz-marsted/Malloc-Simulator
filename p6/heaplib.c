#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* Useful shorthand: casts a pointer to a (char *) before adding */
#define ALIGNMENT 8
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))
#define ALIGN(n) (n + (ALIGNMENT-(n%ALIGNMENT)))
#define ALIGN_PTR(p) ((void *)(ALIGN((uintptr_t) p)))

typedef struct _block_info_t {
  unsigned int block_size;
  void *block;
  bool in_use;
  bool touched;
} block_info_t;

typedef struct _heap_header_t {
  unsigned int heap_size;
  block_info_t first_block;
} heap_header_t ;

void print_debug_heap_header(heap_header_t *header) {
    #ifdef PRINT_DEBUG
  printf("heap starts at addr %p\n"   // C printing trick.
	 "heap_size = %d\n",            // Notice: no commas between lines
	 header, header->heap_size);

    #endif
}
/*void print_debug_block_block(heap_header_t *header, int which_block) {
  #ifdef PRINT_DEBUG
  int i, num_chars = header->blocks[which_block].block_size;
  char *block = header->blocks[which_block].block;
  printf("block:\n");
  for (i = 0; i < num_chars; i++) {
    printf("\t%c\n", block[i]);
  }
  #endif
  }*/

void print_debug_alloc(void *block_addr) {
    #ifdef PRINT_DEBUG
  printf("will give user block beginning @: %p\n, misaligned by %d]n", block_addr, (int)(((uintptr_t)block_addr) % ALIGNMENT));
    #endif
}

void print_debug_entering_init() {
    #ifdef PRINT_DEBUG
  printf("Entering hl_init()\n");
    #endif
}

void print_debug_entering_alloc(int size) {
    #ifdef PRINT_DEBUG
  printf("Entering hl_alloc(), looking for block of size %d\n", size);
    #endif
}

void print_debug_num(unsigned int i) {
      #ifdef PRINT_DEBUG
  printf("Num is  %ud\nMisaligned by %d\n", i, i %ALIGNMENT);
      #endif
}

void print_debug_bool(bool b) {
    #ifdef PRINT_DEBUG
  printf(b ? "true\n" : "false\n");
    #endif
}

void print_debug_if() {
        #ifdef PRINT_DEBUG
  printf("Entering first if\n");
        #endif
}

void print_debug_new() {
          #ifdef PRINT_DEBUG
  printf("NEW ALLOC CALL\n");
          #endif
}

void print_debug_loop() {
      #ifdef PRINT_DEBUG
  printf("Entering loop\n");
      #endif
}

void print_debug_inside() {
        #ifdef PRINT_DEBUG
  printf("still in loop\n");
        #endif
}

void print_debug_finished() {
        #ifdef PRINT_DEBUG
  printf("finished\n");
        #endif
}

void print_debug_entering_release() {
    #ifdef PRINT_DEBUG
  printf("Entering hl_release()\n");
    #endif
}

void print_debug_entering_resize() {
    #ifdef PRINT_DEBUG
  printf("Entering hl_resize()\n");
    #endif
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_init(void *heap, unsigned int heap_size) {

  

  heap_header_t *header = (heap_header_t *)heap;
  header->heap_size = heap_size;
  header->first_block.block_size = 0;
  header->first_block.block = NULL;
  header->first_block.in_use = false;


  
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_alloc(void *heap, unsigned int block_size) {
  
  heap_header_t *header = (heap_header_t *)heap;

  //if no block available
  if(block_size >= header->heap_size){
    return NULL;
  }

  void* traverse = heap; //a pointer that traverses the heap
  int size_of_header_metadata = sizeof(heap_header_t) + sizeof(block_info_t);

  int size_of_metadata = sizeof(block_info_t); //doesn't account for first-block specific meta-data


  //check to see if first block is in use, code here is essentially exactly the same as in the while loop, numbers are just a little different
  //since the header info is slightly bigger. first optimaztion we can do is to make change how metadata is stored
  if(!(header->first_block.in_use) && (header->first_block.block_size-size_of_header_metadata >= block_size)){
    char* aligned_block_ptr = ALIGN_PTR(ADD_BYTES(heap, size_of_header_metadata));

    header->first_block.block_size = size_of_header_metadata + block_size;
    header->first_block.in_use = true;
    header->first_block.touched = true;
    header->first_block.block = aligned_block_ptr;

    //get pointer to next block after the header
    block_info_t *nextBlock = (block_info_t *) aligned_block_ptr;

    //check to see if this block has been "touched", ie if we have gone through the heap before and changed the size of this block
    if(((nextBlock->block_size-size_of_header_metadata) >= block_size) && !(nextBlock->touched)){

      //if it hasnt been touched before, then create a new block there that is the same size as all of the remaining space in the heap
      nextBlock->block = aligned_block_ptr;
      nextBlock->in_use = false;
      nextBlock->touched = false;
      nextBlock->block_size = header->heap_size-size_of_metadata;
    }
    return aligned_block_ptr;
  }

  //if the first block is already in use, move to the pointer to the next block
  traverse = ALIGN_PTR(ADD_BYTES(traverse,header->first_block.block_size));

  //this while loop essentially does the exact same thing as the if statement, except it actually can traverse through the whole heap if needed

  

  while(traverse+block_size < heap+(header->heap_size)){

    //get the current block we are at
    block_info_t *curBlock = (block_info_t *)(traverse);

    //if current block hasn't been touched and doesnt go over the edge of the heap, allocate it
    if(((curBlock->block_size-size_of_metadata) >= block_size) && (curBlock->in_use == false)){
      curBlock->block = traverse+size_of_metadata;
      curBlock->in_use = true;
      curBlock->block_size = block_size+size_of_metadata;
      if(curBlock->touched == false){
	curBlock->touched = true;
      }

      //check to see if next block has been touched
      if(traverse+block_size+size_of_metadata < heap+(header->heap_size)){
	block_info_t *nextBlock = (block_info_t *)(traverse+block_size+size_of_metadata);
	if(nextBlock->touched != true){
	  nextBlock->block = (traverse+size_of_metadata);
	  nextBlock->in_use = false;
	  nextBlock->touched = false;
	  nextBlock->block_size = header->heap_size-(traverse-heap);
	}
      }
      return curBlock->block;
    }

    //if we couldnt allocate the current block, get pointer to the next block
    traverse = ALIGN_PTR(ADD_BYTES(traverse,curBlock->block_size));
  }

  //if we couldnt allocate the block, return 0
  return NULL;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */

block_info_t* get_block_info(void *heap, void *block) {
  if(block == 0){
    return 0;
  }

  heap_header_t *header = (heap_header_t *) heap;

  void* traverse = heap;
  if(header->first_block.block == block){
    return (&(header->first_block));
  }

  traverse = ALIGN_PTR(ADD_BYTES(traverse, header->first_block.block_size));
  block_info_t* cur_block = (block_info_t*)(traverse);
  while(traverse + cur_block->block_size < heap+(header->heap_size)){
    if(cur_block->block == block){
      return cur_block;
    }
    traverse = ALIGN_PTR(ADD_BYTES(traverse, (cur_block->block_size)));
    cur_block = (block_info_t*)(traverse);
  }

  return 0;
}

void hl_release(void *heap, void *block) {
  block_info_t* cur_block = get_block_info(heap, block);
  if(cur_block == 0){
    return;
  }

  cur_block->in_use = false;
  return;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {
  if(block == 0){
    return hl_alloc(heap, new_size);
  }
  block_info_t *cur_block = get_block_info(heap, block);
  if(cur_block == 0){
    return 0;
  }
  
  int size_of_metadata = sizeof(block_info_t);

  hl_release(heap, block);
  void* new = hl_alloc(heap, new_size);
  if(new == NULL){
    return 0;
  }
  else{
    memmove(new,block,cur_block->block_size-size_of_metadata);
    return new;
  }
  
}
