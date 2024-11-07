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
struct treenode* create_tree(struct treenode *procs, int parent_pid){
	struct treenode *head = 0, *tail = 0;
	for (struct treenode *current = procs; current; current = current->next) {
		if (current->data.parent_pid == parent_pid) {
			struct treenode *node = malloc(sizeof(struct treenode));
			if (!node) return 0;
			node->data = current->data;
			node->child = create_tree(procs, current->data.pid);
			node->next = 0;
			if (!head) head = node;
			if (tail) tail->next = node;
			tail = node;
		}
	}
	return head;
}

void print_process(struct process_data *proc){
	static char *states[] = {
		[UNUSED]    "unused",
		[USED]      "used",
		[SLEEPING]  "sleep ",
		[RUNNABLE]  "runnable",
		[RUNNING]   "running",
		[ZOMBIE]    "zombie"
	};
	printf("%s: pid=%d, heap_size=%d, state=%s\n", proc->name, proc->pid, proc->heap_size, states[proc->state]);
}
void print_tree(struct treenode *node, int depth){
	for (struct treenode *current = node; current; current = current->next) {
		for (int i = 0; i < depth; i++) printf("  ");
		print_process(&current->data);
		print_tree(current->child, depth + 1);
	}
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
	
	// for (struct treenode *current = procs; current; current = current->next) print_process(&current->data);
	struct treenode *root = create_tree(procs, 0);
	if (root == 0) {
		fprintf(2, "Error: failed to create process tree\n");
		exit(1);
	}
	print_tree(root, 0);
	
	exit(0);
}
