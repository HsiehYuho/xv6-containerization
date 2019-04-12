#include "types.h"
#include "container.h"
#include "defs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "stat.h"
#include "x86.h"

struct {
  struct spinlock lock;
  struct container containers[MAX_VCS];
}conttable;

void
continit(void)
{
  initlock(&conttable.lock, "conttable");

  // Init containers with id and state
  struct container *c;
  int i = 0;
  for(c = conttable.containers; c < &conttable.containers[MAX_VCS]; c++){
  	c->id = i++;
  	c->state = EMPTY;
  	c->root = 0;
  }
}

// set container to current process, and it runs on certain inode
// @param inode: the direct it runs on
// return 0 if success, else -1
int 
setcont(struct inode *root){
  struct container *c;
  for(c = conttable.containers; c < &conttable.containers[MAX_VCS] && (c->state != EMPTY); c++);

  // No empty container
  if(c >= &conttable.containers[MAX_VCS] || c->root){
  	return -1;
  }

  // The attached root is not directory or '/'
  if(!root || (root->type != T_DIR && root->inum != ROOTINO)){
  	return -1;
  }

  acquire(&conttable.lock);
  c->state = PAUSE;
  c->root = root; 
  release(&conttable.lock);

  // Set container on process
  struct proc *proc = myproc();

  proc->cont = c;
  return 0;
}
