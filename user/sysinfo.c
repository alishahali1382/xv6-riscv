#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc > 1){
    fprintf(2, "sysinfo takes no argument!\n");
    exit(1);
  }

  struct sysinfo_data info;
  if(sysinfo(&info) < 0){
    fprintf(2, "sysinfo failed!\n");
    exit(1);
  }
  printf("Running processes: %d\n", info.running_processes);
  printf("Free memory: %d KB\n", info.free_memory / 1024);

  exit(0);
}
