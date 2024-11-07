#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    printf("Testing ptree\n");
    int root_pid = getpid();
    if (fork() && fork()){
        if (fork() || fork()){
            if (getpid() == root_pid){
                exec("ptree", argv);
            }
        }
    }
    sleep(1);
    wait(0);
    wait(0);
    wait(0);
    wait(0);
    exit(0);
}
