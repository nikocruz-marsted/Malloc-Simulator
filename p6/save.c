#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* Useful shorthand: casts a pointer to a (char *) before adding */
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))


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
  printf("will give user block beginning @: %p\n", block_addr);
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
  printf("Entering if %ud\n", i);
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

void print_debug_exited() {
      #ifdef PRINT_DEBUG
  printf("exited loop\n");
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

typedef struct _block_info_t {
  unsigned int block_size;
  void *block;
  bool in_use;
  bool touched;
} block_info_t;

typedef struct _heap_header_t {
  unsigned int heap_size;
  unsigned int numBlocks;
  block_info_t first_block;
} heap_header_t ;
/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_init(void *heap, unsigned int heap_size) {

  print_debug_entering_init();

  heap_header_t *header = (heap_header_t *)heap;
  header->heap_size = heap_size;
  header->numBlocks = 0;
  header->first_block.block_size = 0;
  header->first_block.block = NULL;
  header->first_block.in_use = false;
  

  print_debug_heap_header(header);
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_alloc(void *heap, unsigned int block_size) {
  print_debug_new();
  heap_header_t *header = (heap_header_t *)heap;

  print_debug_bool(header->first_block.in_use);

  void* traverse = heap;
  
  if(block_size >= header->heap_size){
    return 0;
  }
  // begin by pointing to the first block
  void *next_free_byte = ADD_BYTES(heap,
				   sizeof(unsigned int) /* heapsize */ +
				   sizeof(unsigned int) /* numBlocks */ +
				   sizeof(block_info_t) /* block info */);
				   
  print_debug_entering_alloc(block_size);
  int size_of_first_metadata = sizeof(heap_header_t) /* heapsize */ + sizeof(block_info_t) /* block info */;
  int size_of_metadata = sizeof(block_info_t) /* block info */;

  print_debug_bool(header->first_block.in_use);
  if((header->first_block.in_use) == false){
    print_debug_if();
    

    header->first_block.block_size = size_of_first_metadata+block_size;
    

    print_debug_bool(header->first_block.in_use);
    header->first_block.in_use = true;
    print_debug_bool(header->first_block.in_use);

    header->first_block.touched = true;
    header->first_block.block = heap+size_of_first_metadata;
    block_info_t *nextBlock = (block_info_t *)(heap+size_of_metadata);
    if(((nextBlock->block_size-size_of_metadata) >= block_size) && (nextBlock->in_use == false)){
      
      
      nextBlock->block = (traverse+size_of_first_metadata);
      nextBlock->in_use = false;
      nextBlock->touched = false;
      nextBlock->block_size = header->heap_size-size_of_metadata;
    }
    print_debug_alloc(traverse+size_of_metadata);
    return next_free_byte;
  }
  
  traverse = ADD_BYTES(traverse,header->first_block.block_size); 

  print_debug_loop();
  
  while(traverse+block_size < heap+(header->heap_size)){

    print_debug_inside();

    block_info_t *curBlock = (block_info_t *)(traverse);
    if(((curBlock->block_size-size_of_metadata) >= block_size) && (curBlock->in_use == false)){
      curBlock->block = traverse+size_of_metadata;
      curBlock->in_use = true;
      curBlock->block_size = block_size+size_of_metadata;
      if(curBlock->touched == false){
	curBlock->touched = true;
      }
      
      block_info_t *nextBlock = (block_info_t *)(traverse+block_size+size_of_metadata);
      if(nextBlock->touched != true){
	nextBlock->block = (traverse+size_of_metadata);
	nextBlock->in_use = false;
	nextBlock->touched = false;
	nextBlock->block_size = header->heap_size-(traverse-heap);
      }
      print_debug_alloc(traverse+size_of_metadata);
      return curBlock->block;
    }
    traverse = ADD_BYTES(traverse,curBlock->block_size); 
  }

  print_debug_exited();


  print_debug_heap_header(header);

  return NULL;
  
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_release(void *heap, void *block) {
  if(block == 0){
    return;
  }

  heap_header_t *header = (heap_header_t *)heap;

  void* traverse = heap;
  
  if(header->first_block.block == block){
    header->first_block.in_use = false;
  }

  traverse = ADD_BYTES(traverse,header->first_block.block_size);
  
  while(traverse < heap+(header->heap_size)){
    block_info_t *curBlock = (block_info_t *)(traverse);
    if(curBlock->block == block){
      curBlock->in_use = false;
      return;
    }
    traverse = ADD_BYTES(traverse,curBlock->block_size);
  }
  
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {

    return FAILURE;
}

