// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

// Each 'free page' representation in the freelist
struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  
  // Free list is list of physical memory pages available for allocation
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  // end is a symbol that is equal to the first address after kernel
  // PHYSTOP is the last block of memory - assumes xv6 has 16MB of physical memory

  // Add pages between end and PHYSTOP to allocator's freelist
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;

  // PGROUNDUP ensures 'free' addresses are multiples of 4096
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    // Adds address of memory page to freelist
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.

  // 'Dangling refs' are where code references memory
  // after freeing it - fill it with junk to break bad code faster
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);

  // Grab first run on freelist
  r = kmem.freelist;

  if(r)
    // Remove run from free list
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    // Catch dangling refs 
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

int get_freemem(void){
  int freemem = 0;

  // Grab first run on freelist
  struct run *r = kmem.freelist;
  while(r){
    r = r->next;
    freemem += 4096;
  }
  return freemem;
}

