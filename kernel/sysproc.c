#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sysinfo(void)
{
  struct sysinfo_data result;
  uint64 result_addr;
  
  argaddr(0, &result_addr);

  result.free_memory = free_memory_pages() * PGSIZE;
  result.running_processes = running_processes();

  struct proc *p = myproc();
  return copyout(p->pagetable, result_addr, (char *)&result, sizeof(result));
}

uint64
sys_next_process(void)
{
  int before_pid;
  uint64 proc_data_addr;

  argint(0, &before_pid);
  argaddr(1, &proc_data_addr);

  struct proc *next_process = find_next_process(before_pid);
  if(!next_process)
    return 0;

  struct process_data result;
  result.pid = next_process->pid;
  memmove(result.name, next_process->name, 16);
  result.heap_size = next_process->sz;
  result.state = next_process->state;
  result.parent_pid = next_process->parent ? next_process->parent->pid : 0;
  result.niceness = next_process->niceness;

  struct proc *p = myproc();
  if (copyout(p->pagetable, proc_data_addr, (char *)&result, sizeof(result)) < 0)
    return -1;
  return 1;
}