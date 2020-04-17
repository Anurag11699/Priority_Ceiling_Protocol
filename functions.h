#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data_structures.h"
#include "configuration.h"

//global variables
FILE* output_fd;

//macros
#define max(x,y) ((x>y)?x:y)
#define min(x, y) ((x)<(y)?(x):(y))
#define RM 0
#define EDF 1
#define NOT_IN_SYSTEM 0 //either terminated or not arrived
#define READY 1
#define EXECUTING 2


//Ready queue related functions
// extern tree_node* new_tree_node(job*);
// extern red_black_tree* new_red_black_tree();
// extern void left_rotate(red_black_tree *, tree_node *);
// extern void right_rotate(red_black_tree *, tree_node *);
// extern void insertion_fixup(red_black_tree *, tree_node *);
// extern void insert(red_black_tree *, job *);
// extern void rb_transplant(red_black_tree *, tree_node *, tree_node *);
// extern tree_node* minimum(red_black_tree *, tree_node *);
// extern void rb_delete_fixup(red_black_tree *, tree_node *);  
// extern void rb_delete(red_black_tree *, tree_node *);
// extern void rb_delete_job(red_black_tree*,job*);
// extern void inorder(red_black_tree *, tree_node *);
// extern void update_job_priority(red_black_tree*,resource_list*,job*,int);
// extern tree_node* rb_tree_search(red_black_tree*,job*);
// extern job* RB_tree_remove_min(red_black_tree*);
// extern job* RB_tree_get_min(red_black_tree*);
// extern int is_ready_queue_empty(red_black_tree*);
extern priority_queue* initialize_priority_queue();
extern void insert_priority_queue(priority_queue*,job*);
extern job* get_max_priority_queue(priority_queue*);
extern job* remove_max_priority_queue(priority_queue*);
extern int delete_job_priority_queue(priority_queue*,job*);
extern int is_priority_queue_empty(priority_queue*);


//functions needed to calculate hyperperiod
extern int gcd(int, int);
extern int hyperperiod(task_set *);


//scheduling functions
extern job_list* initialize_job_list(task_set *,resource_list *,int); 
extern void scheduler_priority_ceiling(kernel*,task_set *,resource_list*,int);
extern void update_priority_ceiling(kernel*,job_list*, resource_list*);
extern void print_job_list(job_list*);
extern void print_priority_ceilings(kernel*,resource_list*);


#endif /*FUNCTIONS_H*/