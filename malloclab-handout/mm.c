/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Choi Min Yeop",
    /* First member's email address */
    "okas832@kaist.ac.kr",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* SIZE_T_SIZE == 8 */
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define BK(p) *((void ** )((size_t *)p + 1))
#define FD(p) *((void ** )((size_t *)p + 2))
#define HEAD(p) *((size_t *)p)
#define FOOT(p,size) *((size_t *)p + (size / 4) - 1)


// is ptr in heap?
#define IN_HEAP(ptr) (mem_heap_lo() + 4 <= ptr && ptr <= mem_heap_hi())


// head and tail of free chunk Double linked list
void * head;

// max of size of free chunk 
size_t bmax;

// return one of the argument that is largest
size_t max(size_t p1, size_t p2){
	if(p1 > p2) return p1;
	return p2;
}
/*
 * malloc_chunk - make a malloc chunk putting header and footer
 */

void malloc_chunk(void * p, size_t size){
	HEAD(p     ) = size | 1;
	FOOT(p,size) = size | 1;
}

/*
 * free_chunk - make a free chunk putting header, footer, bk, fd
 */
void free_chunk(void * p, size_t size,void * bk, void * fd){
	HEAD(p     ) = size;
	BK(p) = bk;
	FD(p) = fd;
    FOOT(p,size) = size;
}

/*
 * free_nol_chunk - make a free chunk but not on list because size is not big enough(< 16 bytes)
 *                  only put header and footer
 */
void free_nol_chunk(void *p, size_t size){
	HEAD(p     ) = size;
	FOOT(p,size) = size;
}

/*
 * fd_bk_conn_p - connect the middle chunk with back chunk and front chunk(only for pointing p)
 *
 */

void fd_bk_conn_p(void * p, void * bk, void * fd){
	if(IN_HEAP(bk)) FD(bk) = p;
	if(IN_HEAP(fd)) BK(fd) = p;
}

/*
 * fd_bk_conn -  connect fd and bk(like delete node)
 */

void fd_bk_conn(void * bk, void * fd){
	if(IN_HEAP(bk)) FD(bk) = fd;
	if(IN_HEAP(fd)) BK(fd) = bk;
}

/*
 * free_to_alloc - change free chunk to allocated chunk
 *                 and remake a free chunk for the free rest of the chunk
 */

void free_to_alloc(void * p, size_t size){
	size_t free_size = HEAD(p);
	void * free_bk = BK(p);
    void * free_fd = FD(p);
	int sw = 0;
	if(head == p){
		head = free_fd;
		sw = 1;
	}
	malloc_chunk(p,size);
	void * new_free_addr = (void *)((char *)p + size);
	if(free_size - size >= 16){ // big enough to make a free chunk and put in the list
		free_chunk(new_free_addr,free_size - size,free_bk,free_fd);
		fd_bk_conn_p(new_free_addr,free_bk,free_fd);
		if(sw) head = new_free_addr;
	}
	else if(free_size - size == 0){ // use all to make allocated chunk
		fd_bk_conn(free_bk,free_fd);
	}
	else{ // rest size is not 0, but not enought to make a normal free chunk
		free_nol_chunk(new_free_addr, free_size - size);
		fd_bk_conn(free_bk,free_fd);
	}
	
}

/*
 * find_free - search the free chunk that is big enough to make allocated chunk in free chunk
 *             return the address of the free chunk or not
 *           + if last free chunk is the lastest chunk in the heap,
 *           + extend the free chunk and return it (better than allocating all size)  
 */
void * find_free(newsize){
	if(bmax < newsize) return 0;
	void * it = head;
	void *pit = NULL;
	while(it != 0 && IN_HEAP(it)){
		if(HEAD(it) >= newsize){
			return it;
		}
		pit = it;
		it = FD(it);
	}
	if(pit != NULL){ // this is plus part
		if(!IN_HEAP((void*)((char *)pit + HEAD(pit))) && HEAD(pit) > 64){
			mem_sbrk(ALIGN(newsize - HEAD(pit)));
			free_chunk(pit,newsize,BK(pit),FD(pit));
			return pit;
		}
	}
	return 0;
}



/*
 * find_bk - find a nearest free chunk on the back that is on the list. (p is not a pointer that pointing free chunk)
 */

void * find_bk(void * p){
	size_t size;
	if(head == NULL || p < head) return 0;
	do{
		do{
			size = *((size_t *)p - 1) & (~0x7);
			if(size == 0) return 0;
			p = (void *)((char *)p - size);
		}while(p >= mem_heap_lo() + 4 && (HEAD(p) & 1) == 1);
	}while((HEAD(p) & (~0x7)) < 16);
	if(p < mem_heap_lo() + 4) return 0;
	return p;
}

/*
 * find_fd - find a nearest free chunk on the front that is on the list. (p is not a pointer that pointing free chunk)
 */

void * find_fd(void * p){
	size_t size = HEAD(p) & (~0x7);
	do{
		do{	
			p = (void *)((char *)p + size);
			size = HEAD(p) & (~0x7);
			if(size == 0) return 0;
		}while(p <= mem_heap_hi() && (HEAD(p) & 1) == 1); // free chunk?
	}while((HEAD(p) & (~0x7)) < 16); // on the list free chunk?
	if(p > mem_heap_hi()) return 0;
	return p;
}

/*
 * merge - coalescing free chunks next to the free chunk that p is pointing
 *         also considered the chunks that is smaller than 16 bytes
 *         
 */

void merge(void *p){
	int sw = 1;
	while(sw){ // repeat until merging ended (have to repeat because of small chunks)
		size_t size = HEAD(p) & (~0x7);
		void *bk = BK(p);
		void *fd = (void *)((char *)p + size);
		sw = 0;
		if(IN_HEAP(bk) && (HEAD(bk) & 1) == 0){ // back chunk is free chunk? 
			size_t bk_size = HEAD(bk) & (~0x7);
			if(p == (void *)((char *)bk + bk_size)){ // is that chunk stick to p?
				if(bk_size >= 16){ // free chunk on list
					free_chunk(bk, size + bk_size,BK(bk),FD(p));
					fd_bk_conn(bk,FD(p));
					p = bk;
					size = HEAD(p) & (~0x7);
				}
				else{ // not on list
					free_chunk(bk, size + bk_size,BK(p),FD(p));
					p = bk;
					size = HEAD(p) & (~0x7);
				}
				bmax = max(bmax, size + bk_size);
				sw = 1;
			}
		}
		if(IN_HEAP(fd) && (HEAD(fd) & 1) == 0){ // front chunk is free chunk
			size_t fd_size = HEAD(fd) & (~0x7);
			if(fd == (void *)((char *)p + size)){  // is that chunk stick to p?
				if(fd_size >= 16){ // free chunk on list
					free_chunk(p,size + fd_size,BK(p),FD(fd));
					fd_bk_conn(p,FD(fd));
				}
				else{ // not on list
					free_chunk(p,size + fd_size,BK(p),FD(p));
				}
				bmax = max(bmax, size + fd_size);
				sw = 1;
			}
		}
	}
}

/*
 * rest_to_free - make some trash space to free chunk if it is big
 */
void rest_to_free(void *ptr,size_t size){
	void * bk = find_bk(ptr);
	void * fd = find_fd(ptr);
	if(head == NULL){
		head = ptr;
	}
	if(bk == NULL) head = ptr;
	free_chunk(ptr,size,bk,fd);
	fd_bk_conn_p(ptr,bk,fd);
}



int mm_init(void)
{
	mem_sbrk(4); // for the double word align
	//this is for the small free chunk
	//by Testing some cases, this is more good 
	head = mem_sbrk(64); 
	free_chunk(head,64,NULL,NULL);
	
	bmax = 64;
	return 0;
}

/* 
 * mm_malloc - make a allocated chunk!
 *             1. if there is a free chunk that is big enough, allocate into it
 *             2. else, get more heap and make an allocated chunk in it
 */
void *mm_malloc(size_t size)
{
    
	int newsize = ALIGN(max(16,size + 8)); // least all chunk have to have more than 16 to make a free chunk
	if(16 <= size && size <= 512){
		newsize = 1;
		while(newsize < size) newsize *= 2;
		if((newsize * 85) / 100 > size) newsize = ALIGN(max(16,size + 8));
		else newsize += 8;
	}
	void * good_free_chunk = find_free(newsize);
	if(!IN_HEAP(good_free_chunk)){
		void *p = mem_sbrk(newsize);
		if(p == (void *) -1){
			return NULL;
		}
		else{
			malloc_chunk(p, newsize);
			return (void *)((char *)p + 4);
		}
	}
	else{
		free_to_alloc(good_free_chunk,newsize);
		return (void *)((char *)good_free_chunk + 4);	
	}
}



/*
 * mm_free - free the allocated chunk
 *           make free, add on list, merge
 */
void mm_free(void *ptr)
{	
	ptr = (void *)((char *)ptr - 4);	
	size_t size = HEAD(ptr) & (~0x7);
	bmax = max(bmax,size);
	void * bk = find_bk(ptr);
	void * fd;
	if(bk == NULL) fd = head;
	else fd = FD(bk);

	if(head == NULL){
		head = ptr;
	}
	if(bk == NULL) head = ptr;
	free_chunk(ptr,size,bk,fd);
	fd_bk_conn_p(ptr,bk,fd);
	merge(ptr);
}

/*
 * mm_realloc - try to keep the chunk to not move to other address
 *              1. if new size is smaller than before size -> just change to smaller chunk and make a free chunk to rest
 *              2. if not, if this chunk is the lastest chunk, increase the size of the chunk
 *              3. if not, if the next chunk is free chunk and can cover the size that chunk need, overwrap the free chunk
 *              4. if not, do the worst case, mm_malloc(new) -> move data(old -> new) -> mm_free(old)
 */
void *mm_realloc(void *ptr, size_t size)
{
	ptr = (void *)((char *)ptr - 4);
	size_t save_size = size;
	size = ALIGN(size) + 8;
	size_t bef_size = HEAD(ptr) & (~0x7);
	void * rest_ptr;
	size_t rest_size;
	if(size <= bef_size){ // user want smaller
		malloc_chunk(ptr,size);
		rest_ptr = (void *)((char *)ptr + size);
		rest_size = bef_size - size;
		if(rest_size >= 16){
			rest_to_free(rest_ptr,rest_size);
			merge(rest_ptr);
		}
		else if(rest_size != 0){
			free_nol_chunk(rest_ptr,rest_size);
		}
		return (void *)((char *)ptr + 4);
	}
	else{
		void * next_ptr = (void *)((char *)ptr + bef_size);
		if(!IN_HEAP(next_ptr)){ // is this lastest chunk?
			rest_size = size - bef_size;
			mem_sbrk(rest_size);
			malloc_chunk(ptr,size);
			return (void *)((char *)ptr + 4);
		}
		else if(size <= bef_size + (HEAD(next_ptr) & (~0x7)) && (HEAD(next_ptr) & 1) == 0){
            // is there a free chunk next to it and it is big size to carry on?
			size_t next_size= HEAD(next_ptr) & (~0x7);
			malloc_chunk(ptr,size);
			rest_ptr = (void *)((char *)ptr + size);
			rest_size = bef_size + next_size - size;
			if(rest_size >= 16){
				rest_to_free(rest_ptr,rest_size);
				merge(rest_ptr);
			}
			else if(rest_size != 0){
				free_nol_chunk(rest_ptr,rest_size);
			}
			return (void *)((char *)ptr + 4);
		}
		else{ // just make new chunk...
			void * new_ptr = mm_malloc(save_size);
			if(new_ptr == NULL) return NULL;
			memcpy((char *)new_ptr,(char *)ptr + 4,bef_size-8);
			mm_free((void *)((char *)ptr + 4));
			return new_ptr;
		}
	}
}

int mm_check(){
	void *pit = NULL;
	void * it = head;
	//Check Doubled Linked List connection, free chunk header and free chunk merge
	while(IN_HEAP(it)){
		//connection
		if(IN_HEAP(BK(it)) && BK(it) != pit) return 0;
		if(pit != NULL && it != FD(pit)) return 0;
		//free chunk header
		if((HEAD(it) & 1) == 1) return 0;
        //coalescing
		if(pit != NULL && (void *)((char *)pit + HEAD(pit)) == it) return 0;
		pit = it;
		it = FD(it);
	}
	
	return 1;
}









