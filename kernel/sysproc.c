```c
extern struct spinlock wait_lock;
uint64
sys_getppid(void){
  struct proc *p = myproc();
  acquire(&wait_lock);
  int ppid;
  if (p->parent == 0){
    printk("This is init");
    ppid = -1;
  }
  else {ppid = p->parent->pid;}
  release(&wait_lock);
  return ppid;
}

uint64
count_children(void){
  struct proc *p;
  struct proc *me = myproc();
  int count = 0;
  acquire(&wait_lock);
  for(p = proc; p < &proc[NPROC]; p++){
    if (p->parent == me){
      count++;
    }
  }
  release(&wait_lock);
  return count;
}
```
