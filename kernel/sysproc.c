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

uint64
sys_vmprint(void)
{
vmprint(myproc()->pagetable);
return 0;
}

uint64
sys_vmperms(void)
{
vmperms(myproc()->pagetable);
return 0;
}

uint64
sys_vmstat(void)
{
vmstat(myproc()->pagetable);
return 0;
}

uint64
sys_stackdepth(void){
  int count = 0;
  struct proc *p = myproc();
  uint64 fp = p->trapframe->s0;
  uint64 down = PGROUNDDOWN(fp);
  uint64 up = PGROUNDUP(fp);
  while(fp < up && fp > down){
    count++;
    uint64 prev_fp;
    if (fetchaddr(fp - 16, &prev_fp) < 0) break;
    fp = prev_fp;
  }
  return count;
}

uint64
sys_frames(void){
  int count = 0;
  struct proc *p = myproc();
  uint64 fp = p->trapframe->s0;
  uint64 down = PGROUNDDOWN(fp);
  uint64 up = PGROUNDUP(fp);

  printk("%p %ld\n", (void*)p->trapframe->ra, fp - p->trapframe->sp);
  count++;

  uint64 next_fp;
  if (fetchaddr(fp - 16, &next_fp) < 0) return count;
  fp = next_fp;

  while(fp < up && fp > down){
    uint64 ra, next_fp;
    if(fetchaddr(fp - 8, &ra) < 0 || fetchaddr(fp - 16, &next_fp) < 0)
      break;
    if (ra == 0) 
      break; 
    uint64 size = next_fp - fp;
    printk("%p  %ld\n", (void*)ra, size);
    fp = next_fp;
    count++;
  }
  return count;
}

uint64 
sys_btrange(void){
  struct proc *p = myproc();
  uint64 fp = p->trapframe->s0;
  uint64 top = PGROUNDUP(fp); 

  int skip, max;
  argint(0, &skip);
  argint(1, &max);

  if (skip < 0 || max < 0){
    return -1;
  }

  int index = 0;
  int count = 0;

  if (index >= skip && count < max) {
    printk("%p\n", (void*)p->trapframe->ra);
    count++;
    }
  index++;  

  while(fp != 0 && fp < top){
    uint64 ra, prev;
    if (fetchaddr(fp - 8, &ra) < 0 || fetchaddr(fp - 16, &prev) < 0)
      break;
    if (ra == 0) 
      break;
    if (index >= skip && count < max) {
      printk("%p\n", (void*)ra);
      count++;
    }
    if (count >= max){
      break;
    }
    index++;

    fp = prev;
  }
  return count;
}
```
