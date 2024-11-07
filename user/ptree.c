#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct treenode {
	struct process_data data;
	struct treenode *next, *child;
};

struct treenode* get_procs_list(){
	struct treenode *head = malloc(sizeof(struct treenode));
	if (!head || next_process(-1, &head->data) == -1) return 0;

	struct treenode *current = head;
	while (1) {
		struct treenode *node = malloc(sizeof(struct treenode));
		if (!node) return 0;

		int result=next_process(current->data.pid, &node->data);
		if (result == -1) {
			fprintf(2, "Error: failed to get next process\n");
			free(node);
			break;
		}
		if (result == 0) {
			free(node);
			break;
		}
		current->next = node;
		current = node;
	}
	current->next = 0;
	return head;
}

int main(int argc, char *argv[]){
	if (argc != 1) {
		fprintf(2, "Usage: ptree\n");
		exit(1);
	}

	struct treenode *procs = get_procs_list();
	if (procs == 0) {
		fprintf(2, "Error: failed to get process list\n");
		exit(1);
	}
	
	// Traverse and print the process list
	struct treenode *current = procs;
	while (current != 0) {
		fprintf(1, "pid: %d, ppid: %d\n", current->data.pid, current->data.parent_pid);
		struct treenode *temp = current;
		current = current->next;
		free(temp);  // Free memory to prevent leaks
	}
	
	exit(0);
}
