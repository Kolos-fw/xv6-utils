```c
static void
vmprint_rec(pagetable_t pagetable, int level, uint64 prefix)
{
  for (int i = 0; i < 512; i++) {
    pte_t pte = pagetable[i];
    if ((pte & PTE_V) == 0)
      continue;
    uint64 va = prefix | ((uint64)i << PXSHIFT(level));
    uint64 pa = PTE2PA(pte);
    for (int j = 0; j < 3 - level; j++)
      printk(" ..");
    int is_leaf = (pte & (PTE_R | PTE_W | PTE_X));
    if (is_leaf)
      printk("%d: pte %p pa %p va %p\n", i, (void *)pte, (void *)pa, (void *)va);
    else
      printk("%d: pte %p pa %p\n", i, (void *)pte, (void *)pa);
    if (!is_leaf && (level > 0)) {
      pagetable_t child = (pagetable_t)pa;
      vmprint_rec(child, level - 1, va);
    }
  }
}

void
vmprint(pagetable_t pagetable)
{
  printk("page table %p\n", (void *)pagetable);
  vmprint_rec(pagetable, 2, 0);
}

static void
vmperms_rec(pagetable_t pagetable, int level, uint64 prefix)
{
  for (int i = 0; i < 512; i++) {
    pte_t pte = pagetable[i];
    if ((pte & PTE_V) == 0) {continue;}
    uint64 va = prefix | ((uint64)i << PXSHIFT(level));
    uint64 pa = PTE2PA(pte);
    int is_leaf = (pte & (PTE_R | PTE_W | PTE_X));
    char r = '-';
    char w = '-';
    char x = '-';
    char u = '-';
    if (pte & PTE_R){ r = 'r'; }
    if (pte & PTE_W){ w = 'w'; }
    if (pte & PTE_X){ x = 'x'; }
    if (pte & PTE_U){ u = 'u'; }
    if (is_leaf) {
      printk("va %p -> pa %p %c%c%c%c\n", (void *)va, (void *)pa, r, w, x, u);
      continue;
    }
    if (!is_leaf && (level > 0)) {
      pagetable_t child = (pagetable_t)pa;
      vmperms_rec(child, level - 1, va);
    }
  }
}

void
vmperms(pagetable_t pagetable)
{
  vmperms_rec(pagetable, 2, 0);
}

static int lvl_2 = 1;
static int lvl_1 = 0;
static int lvl_0 = 0;
static int with_u = 0;
static int without_u = 0;
static int count_leaf = 0;

static void
vmstat_rec(pagetable_t pagetable, int level, uint64 prefix)
{ 
  for (int i = 0; i < 512; i++) {
    pte_t pte = pagetable[i];
    if ((pte & PTE_V) == 0) {continue;}
    uint64 va = prefix | ((uint64)i << PXSHIFT(level));
    uint64 pa = PTE2PA(pte);

    if (level == 2){ lvl_1++; }
    if (level == 1){ lvl_0++; }
    int is_leaf = (pte & (PTE_R | PTE_W | PTE_X));
    if (is_leaf){
      count_leaf++;
      if (pte & PTE_U) { with_u++; }
      else { without_u++; }
      continue;
    }
    if (!is_leaf && (level > 0)) {
      pagetable_t child = (pagetable_t)pa;
      vmstat_rec(child, level - 1, va);
    }
  }
}

void
vmstat(pagetable_t pagetable)
{
  vmstat_rec(pagetable, 2, 0);
  int ves = (lvl_2 + lvl_1 + lvl_0) * 4;
  printk("таблиц: уровень 2 — %d, уровень 1 — %d, уровень 0 — %d (всего %d КБ)\n",
  lvl_2, lvl_1, lvl_0, ves);
  printk("листьев: %d (с U: %d, без U: %d)\n",
  count_leaf, with_u, without_u);
}
```
