// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

char *superpool_start;
char *superpool_end;


void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct superrun {
  struct superrun *next;
};

struct {
  struct spinlock lock;
  struct superrun *freelist;
} superkmem;




void
superkfree(void *pa)
{
  struct superrun *r;


  if (((uint64)pa % SUPERPGSIZE) != 0 || 
      (char*)pa < superpool_start || 
      (char*)pa >= superpool_end)
    panic("superkfree: bad address");


  memset(pa, 1, SUPERPGSIZE);

  r = (struct superrun*)pa;

  acquire(&superkmem.lock);
  r->next = superkmem.freelist;
  superkmem.freelist = r;
  release(&superkmem.lock);
}

void*
superkalloc(void)
{
  struct superrun *r;

  acquire(&superkmem.lock);
  r = superkmem.freelist;
  if (r)
    superkmem.freelist = r->next;
  release(&superkmem.lock);

  if (r)
    memset((char*)r, 5, SUPERPGSIZE); 
  return (void*)r;
}

void
superkinit()
{
  initlock(&superkmem.lock, "superkmem");
  superpool_start = (char*)SUPERPOOL_BASE;
  superpool_end = (char*)(SUPERPOOL_BASE + SUPERPOOL_SIZE);
  for (int i = 0; i < 8; i++) {
    superkfree((void*)(superpool_start + i * SUPERPGSIZE));
  }
}
void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)SUPERPOOL_BASE);
  freerange((void*)(SUPERPOOL_BASE + SUPERPOOL_SIZE), (void*)PHYSTOP);
  superkinit();   
}



void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
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
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
