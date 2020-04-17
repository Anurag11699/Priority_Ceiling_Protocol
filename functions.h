#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data_structures.h"
#include "configuration.h"

//global variables
FILE* output_fd;

//macros
#define max(x,y) ((x>y)?x:y)
#define min(x, y) ((x)<(y)?(x):(y))

//scheduling algos
#define RM 0
#define EDF 1

//states of a job
#define NOT_IN_SYSTEM 0 //either terminated or not arrived
#define READY 1
#define EXECUTING 2


//Ready queue related functions
extern void update_job_priority(kernel*,resource_list*,job*,int);
extern priority_queue* initialize_priority_queue();
extern void insert_priority_queue(priority_queue*,job*);
extern job* get_max_priority_queue(priority_queue*);
extern job* remove_max_priority_queue(priority_queue*);
extern int delete_job_priority_queue(priority_queue*,job*);
extern int is_priority_queue_empty(priority_queue*);
extern void print_priority_queue(priority_queue*);


//functions needed to calculate hyperperiod
extern int gcd(int, int);
extern int hyperperiod(task_set *);


//scheduling functions
extern job_list* initialize_job_list(task_set *,resource_list *,int); 
extern void scheduler_priority_ceiling(kernel*,task_set *,resource_list*,int);
extern void update_priority_ceiling(kernel*, resource_list*);
extern void print_job_list(job_list*);
extern void print_priority_ceilings(kernel*,resource_list*);


#endif /*FUNCTIONS_H*/