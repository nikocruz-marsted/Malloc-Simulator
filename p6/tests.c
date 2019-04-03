#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"

#define HEAP_SIZE 1024
#define NUM_TESTS 24
#define NPOINTERS 100

// TODO: Add test descriptions as you add more tests...
const char* test_descriptions[] = {
  /* our SPEC tests */
  /* 0 */ "single init, should return without error",
  /* 1 */ "single init then single alloc, should pass",
  /* 2 */ "single alloc which should fail b/c heap is not big enough",
  /* 3 */ "multiple allocs, verifying no hard-coded block limit",
  /* your SPEC tests */
  /* 4  */ "your description here",
  /* 5  */ "your description here",
  /* 6  */ "single init and single alloc, tests 8-byte aligned return addresses, should fail",
  /* 7  */ "your description here",
  /* 8  */ "your description here",
  /* 9  */ "your description here",
  /* 10 */ "your description here",
  /* 11 */ "your description here",
  /* 12 */ "your description here",
  /* 13 */ "your description here",
  /* 14 */ "your description here",
  /* 15 */ "your description here",
  /* STRESS tests */
  /* 16 */ "alloc & free, stay within heap limits",
  /* 17 */ "randmoized",
  /* 18 */ "superrandomized",
  /* 19 */ "super randomized but really big",
  /* 20 */ "testing data integrity tests",
  /* 21 */ "your description here",
  /* 22 */ "your description here",
  /* 23 */ "your description here",
};

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init
 * SPECIFICATION BEING TESTED:
 * hl_init should successfully complete (without producing a seg
 * fault) for a HEAP_SIZE of 1024 or more.
 *
 * MANIFESTATION OF ERROR:
 * A basic test of hl_init.  If hl_init has an eggregious programming
 * error, this simple call would detect the problem for you by
 * crashing.
 *
 * Note: this shows you how to create a test that should succeed.
 */
int test00() {

  // simulated heap is just a big array
  char heap[HEAP_SIZE];

  hl_init(heap, HEAP_SIZE);

  return SUCCESS;
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is room in the heap for a request for a block, hl_alloc
 * should sucessfully return a pointer to the requested block.
 *
 * MANIFESTATION OF ERROR:
 * The call to hl_alloc will return NULL if the library cannot find a
 * block for the user (even though the test is set up such that the
 * library should be able to succeed).
 */
int test01() {

  // simulated heap is just a big array
  char heap[HEAP_SIZE];

  hl_init(heap, HEAP_SIZE);

  // if this returns null, test01 returns FAILURE (==0)
  return (hl_alloc(heap, HEAP_SIZE/2) != NULL);
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is definitely too big
 * for the library to find. If hl_alloc returns a pointer, the library is flawed.
 *
 * Notice that heaplame's hl_alloc does NOT return NULL. (This is one
 * of many bugs in heaplame.)
 *
 * Note: this shows you how to create a test that should fail.
 *
 * Surely it would be a good idea to test this SPEC with more than
 * just 1 call to alloc, no?
 */
int test02() {

  // simulated heap is just an array
  char heap[HEAP_SIZE];

  hl_init(heap, HEAP_SIZE);

  // if this returns NULL, test02 returns SUCCESS (==1)
  return !hl_alloc(heap, HEAP_SIZE*2);

}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * There should be no hard-coded limit to the number of blocks heaplib
 * can support. So if the heap gets larger, so should the number of
 * supported blocks.
 *
 * MANIFESTATION OF ERROR:
 * See how many blocks are supported with heap size N. This number should
 * increase with heap size 2N. Otherwise fail!
 *
 * Note: unless it is fundamentally changed, heaplame will always fail
 * this test. That is fine. The test will have more meaning when run on
 * your heaplib.c
 */
int test03() {

  // now we simulate 2 heaps, once 2x size of the first
  char heap[HEAP_SIZE], heap2[HEAP_SIZE*2];
  int num_blocks = 0;
  int num_blocks2 = 0;

  hl_init(heap, HEAP_SIZE);

  while(true) {
    int *array = hl_alloc(heap, 8);
    if (array)
      num_blocks++;
    else
      break;
  }

  hl_init(heap2, HEAP_SIZE*2);

  while(true) {
    int *array = hl_alloc(heap2, 8);
    if (array)
      num_blocks2++;
    else
      break;
  }
  #ifdef PRINT_DEBUG
  printf("%d blocks (n), then %d blocks (2n) allocated\n", num_blocks, num_blocks2);
  #endif

  // hoping to return SUCCESS (==1)
  return (num_blocks2 > num_blocks);
}

/* ------------------ YOUR SPEC TESTS ------------------------- */

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: 
 *  hl_init, hl_alloc, hl_resize
 * SPECIFICATION BEING TESTED:
 *  hl_resize will return NULL when unable to 
 *  satify a resize request
 *
 * MANIFESTATION OF ERROR:
 *  This test will fail if a request to resize a
 *  block that is satisfiable is not satisfied, or
 *  if an unsatisfiable request returns null.
 */
int test04() {
  char heap[HEAP_SIZE];

  hl_init(heap, HEAP_SIZE);

  void* p1 = hl_alloc(heap, HEAP_SIZE/4);
  hl_alloc(heap, HEAP_SIZE/4);
  void* p2 = hl_resize(heap, p1, HEAP_SIZE/3);
  void* p3 = hl_resize(heap, p2, HEAP_SIZE/4);
  return !hl_resize(heap, p3, HEAP_SIZE*2) && p2 && p3;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: 
 *  hl_init, hl_alloc, hl_release
 * SPECIFICATION BEING TESTED:
 *  Released memory should be able to be 
 *  allocated again by subsequent calls to hl_alloc
 *
 * MANIFESTATION OF ERROR:
 *  This test will fail if an attempt to allocate
 *  released memory returns null, or if an attempt to
 *  allocate said memory prior to it being released
 *  returns a non-null value.
 */
int test05() {
  char heap[HEAP_SIZE];
  hl_init(heap, HEAP_SIZE);
  void* block = hl_alloc(heap, 8);
  //fill heap
  while(true) {
    void *array = hl_alloc(heap, 8);
    if (!array){
      break;
    }
  }
  if(hl_alloc(heap, 20)){
    return FAILURE;
  }
  hl_release(heap, block);
  return (hl_alloc(heap, 20) == NULL);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_init, hl_alloc
 * SPECIFICATION BEING TESTED:
 *  The returned address (in this case of hl_alloc) should be aligned at 
 *  multiples of 8 bytes, but heap is not required to be 8-byte aligned.
 *
 * MANIFESTATION OF ERROR:
 *  This test fails when an allocation request to a non-8-byte 
 *  aligned heap returns a non-8-byte aligned pointer
 *  
 */
int test06() {
  char heap_std[HEAP_SIZE + 1];
  char* heap = &heap_std[1];
  hl_init(heap, HEAP_SIZE);
  void* p1 = hl_alloc(heap, 10);
  void* p2 = hl_alloc(heap, 3);
  return (
    ((((uintptr_t) p1) % 8) == 0)
    && ((((uintptr_t) p2) % 8) == 0)
  );
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_init, hl_alloc, hl_release
 * SPECIFICATION BEING TESTED:
 *  The returned address (in this case of hl_release) should be aligned at 
 *  multiples of 8 bytes, but heap is not required to be 8-byte aligned.
 *
 * MANIFESTATION OF ERROR:
 *  This test fails when a resize request to a non-8-byte 
 *  aligned heap returns a non-8-byte aligned pointer
 *  
 */
int test07() {
  char heap_std[HEAP_SIZE + 1];
  char* heap = &heap_std[1];
  hl_init(heap, HEAP_SIZE);
  void* p1 = (hl_alloc(heap, 15));
  void* p2 = (hl_resize(heap, p1, 50));
  return ((((uintptr_t)p2) % 8) == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEsING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test08() {
  char heap[HEAP_SIZE];
  hl_init(heap, HEAP_SIZE);
  void* p = hl_alloc(heap, 2*HEAP_SIZE);
  return (p == NULL);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test09() {
  char heap[HEAP_SIZE];

  hl_init(heap, HEAP_SIZE);

  void* p1 = hl_alloc(heap, HEAP_SIZE/4);
  hl_alloc(heap, HEAP_SIZE/4);
  int* p3 = hl_alloc(heap, HEAP_SIZE/6);
  *p3 = 1234;
  hl_alloc(heap, HEAP_SIZE/6);
  hl_release(heap, p1);
  int i1 = *p3;
  int *p4 = hl_resize(heap, p3, HEAP_SIZE/4);

  int i2 = *p4;
  if(i1 == i2){
    return SUCCESS;
  }
  else{
    return FAILURE;
  }
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test10() {
  char heap[HEAP_SIZE];
  hl_init(heap, HEAP_SIZE);
  void* p1 = hl_alloc(heap, HEAP_SIZE/4);
  hl_alloc(heap, HEAP_SIZE/4);
  hl_alloc(heap, HEAP_SIZE/4);
  hl_alloc(heap, HEAP_SIZE/8);
  int* ret = hl_resize(heap, p1, HEAP_SIZE/2);
  if(ret == 0){
    return SUCCESS;
  }
  else{
    return FAILURE;
  }
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 * This test is adapted from test 03, which was provided
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test11() {
  char heap[HEAP_SIZE];
  hl_init(heap, HEAP_SIZE);
  int* p = hl_resize(heap, 0, HEAP_SIZE/4);
  p[2] = 0;
  return SUCCESS;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test12() {
  char heap[HEAP_SIZE];
  hl_init(heap, HEAP_SIZE);
  int* p1 = hl_alloc(heap, 8);
  p1[0] = 10;
  hl_release(heap, 0);
  return(p1[0] == 10);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test13() {

  return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test14() {

  return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test15() {
  int n_tries    = 10000;
  int block_size = 300;

  // 1024 bytes of padding
  // --------------------
  // 1024 bytes of "heap"
  // --------------------  <-- heap_start
  // 1024 bytes of "padding"char memarea[HEAP_SIZE*3];
  char memarea[HEAP_SIZE*3];
  char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
  char *pointers[NPOINTERS];

  // zero out the pointer array
  memset(pointers, 0, NPOINTERS*sizeof(char *));

  hl_init(heap_start, HEAP_SIZE);
  srandom(0);
  for (int i = 0; i < n_tries; i++) {
    int index = random() % NPOINTERS;
    if (pointers[index] == 0) {
      pointers[index] = hl_alloc(heap_start,  block_size);
      if(pointers[index] != 0){
	if(pointers[index] > heap_start+1024){
	  return FAILURE;
	}
	if(pointers[index] < heap_start){
	  return FAILURE;
	}
	if(pointers[index]+block_size > heap_start+1024){
	  return FAILURE;
	}
      }
    }
    else{
      hl_release(heap_start, pointers[index]);
      pointers[index] = 0;
    }
  }
  return SUCCESS;

}

/* ------------------ STRESS TESTS ------------------------- */

/* THIS TEST IS NOT FINISHED. It is a stress test, but it does not
 * actually test to see whether the library or the user writes
 * past the heap. You are encouraged to complete this test.
 *
 * FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:
 * The library should not give user "permission" to write off the end
 * of the heap. Nor should the library ever write off the end of the heap.
 *
 * MANIFESTATION OF ERROR:
 * If we track the data on each end of the heap, it should never be
 * written to by the library or a good user.
 *
 */
int test16() {
  int n_tries    = 10000;
  int block_size = 16;

  // 1024 bytes of padding
  // --------------------
  // 1024 bytes of "heap"
  // --------------------  <-- heap_start
  // 1024 bytes of "padding"
  char memarea[HEAP_SIZE*3];

  char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
  char *pointers[NPOINTERS];

  // zero out the pointer array
  memset(pointers, 0, NPOINTERS*sizeof(char *));

  hl_init(heap_start, HEAP_SIZE);
  srandom(0);
  for (int i = 0; i < n_tries; i++) {
    int index = random() % NPOINTERS;
    if (pointers[index] == 0) {
      pointers[index] = hl_alloc(heap_start,  block_size);
      if(pointers[index] != 0){
	if(pointers[index] > heap_start+1024){
	  return FAILURE;
	}
	if(pointers[index] < heap_start){
	  return FAILURE;
	}
	if(pointers[index]+block_size > heap_start+1024){
	  return FAILURE;
	}
      }
    }
    else{
      hl_release(heap_start, pointers[index]);
      pointers[index] = 0;
    }
  }
  return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test17() {
  int n_tries    = 10000;
  int block_size = 16;
  char *pointers[NPOINTERS];
  int resize = 0;

  // zero out the pointer array
  memset(pointers, 0, NPOINTERS*sizeof(char *));
  char heap[HEAP_SIZE*10];
  hl_init(heap, HEAP_SIZE);
  srandom(0);
  for (int i = 0; i < n_tries; i++){
    int index = random() % NPOINTERS;
    if (pointers[index] == 0) {
      pointers[index] = hl_alloc(heap,  block_size);
    }
    else {
      int newBlockSize = rand() % 4 + 1;
      pointers[index] = hl_resize(heap, pointers[index], newBlockSize);
    }
    resize++;
  }
  return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test18() {
  int n_tries    = 10000;
  char *pointers[NPOINTERS];
  int resize = 0;

  memset(pointers, 0, NPOINTERS*sizeof(char *));
  char heap[HEAP_SIZE*10];
  hl_init(heap, HEAP_SIZE);
  srandom(0);
  for (int i = 0; i < n_tries; i++){
    int index = random() % NPOINTERS;
    int index2 = random() % NPOINTERS;
    if (pointers[index] == 0) {
      int newBlockSize = rand() % 4 + 1;
      int newBlockSize2 = rand() % 4 + 1;
      pointers[index] = hl_alloc(heap, newBlockSize);
      pointers[index2] = hl_alloc(heap, newBlockSize2);
    }
    else {
      if(resize%3 != 0){
	hl_release(heap, pointers[index]);
	pointers[index] = 0;
      }
      else{
	int newBlockSize = rand() % 4 + 1;
	hl_resize(heap, pointers[index], newBlockSize);
      }
    }
    resize++;
  }
  return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test19() {
  int n_tries    = 1000000;
  char *pointers[NPOINTERS*NPOINTERS];
  int resize = 0;

  memset(pointers, 0, NPOINTERS*sizeof(char *));
  char heap[HEAP_SIZE*10];
  hl_init(heap, HEAP_SIZE);
  srandom(0);
  for (int i = 0; i < n_tries; i++){
    int index = random() % NPOINTERS;
    int index2 = random() % NPOINTERS;
    if (pointers[index] == 0) {
      int newBlockSize = rand() % 17 + 1;
      int newBlockSize2 = rand() % 4 + 1;
      pointers[index] = hl_alloc(heap, newBlockSize);
      pointers[index2] = hl_alloc(heap, newBlockSize2);
    }
    else {
      if(resize%10 == 0){
	hl_release(heap, pointers[index]);
	pointers[index] = 0;
      }
      else{
	int newBlockSize = rand() % 34 + 1;
	hl_resize(heap, pointers[index], newBlockSize);
      }
    }
    resize++;
  }
  return SUCCESS;

}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test20() {
  int n_tries    = 10000000;
  int *pointers[NPOINTERS*NPOINTERS];
  int resize = 0;
  int values[NPOINTERS*NPOINTERS];
  memset(values, 0, NPOINTERS*sizeof(int *));
  memset(pointers, 0, NPOINTERS*sizeof(char *));
  char heap[HEAP_SIZE*10];
  hl_init(heap, HEAP_SIZE);
  srandom(0);
  for (int i = 0; i < n_tries; i++){
    int index = random() % NPOINTERS;
    if (pointers[index] == 0) {
      int newData = rand() % 10 + 1;
      int newBlockSize = rand() % 4 + 1;
      pointers[index] = hl_alloc(heap, newBlockSize);
      if(pointers[index] != 0){
	int* p1 = pointers[index];
	*p1 = newData;
      }
      values[index] = newData;
    }
    else {
      if(resize%3 != 0){
	hl_release(heap, pointers[index]);
	pointers[index] = 0;
	values[index] = 0;
      }
      else{
	int newBlockSize = rand() % 4 + 1;
	hl_resize(heap, pointers[index], newBlockSize);
      }
    }
    resize++;
  }
  for(int i = 0 ; i < NPOINTERS ; i++){
    if(pointers[i] != 0){
      int current_val = *pointers[i];
      int actual_val = values[i];
      if(current_val != actual_val){
	return FAILURE;
      }
    }
  }
  return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test21() {

  return FAILURE;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test22() {

  return FAILURE;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test23() {

  return FAILURE;
}
