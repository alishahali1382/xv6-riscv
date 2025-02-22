typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

struct sysinfo_data {
    uint32 free_memory;
    uint32 running_processes;
};

// enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
// struct process_data {
//   int pid;
//   int parent_pid;
//   int heap_size;
//   enum procstate state;
//   char name[16];
// };
