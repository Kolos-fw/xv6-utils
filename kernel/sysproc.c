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
```
