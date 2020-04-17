#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

struct kernel;
typedef struct kernel kernel;

struct resource;
typedef struct resource resource;

struct resource_list;
typedef struct resource_list resource_list;

struct resource_usage;
typedef struct resource_usage resource_usage;

struct req_resource_info;
typedef struct req_resource_info req_resource_info;

struct task;
typedef struct task task;

struct task_set;
typedef struct task_set task_set;

struct job;
typedef struct job job;

struct job_list;
typedef struct job_list job_list;

/*
ADT for Priority Queue Node

job* job_object; -> pointer to job obeject
struct priority_queue_node *next;

*/
struct priority_queue_node
{
	job* job_object;
	struct priority_queue_node *next;
};

typedef struct priority_queue_node priority_queue_node;

/*
ADT for Priority Queue Dummy Head

int number_of_jobs; -> number of jobs in the priority queue
priority_queue_node *next; -> pointer to the first node of the priority queue.

*/
typedef struct priority_queue_dummy_head
{
	int number_of_jobs;
	priority_queue_node *next;
}priority_queue_dummy_head;


/*
ADT for Priority Queue

priority_queue_dummy_head* priority_queue_head; -> pointer to the priority queue dummy header
priority_queue_node* priority_queue_tail; -> pointer to the tail of the priority queue

*/
typedef struct priority_queue
{
    
    priority_queue_dummy_head* priority_queue_head;
    priority_queue_node* priority_queue_tail;

}priority_queue;


/*
ADT for Kernel

int system_priority_ceiling; -> priority ceiling of the system according to the priority ceiling protocol
priority_queue* ready_queue; -> job ready queue
job* currently_executing_job; -> pointer to the job that is currently executing in the system.

*/
struct kernel
{
    int system_priority_ceiling;
    priority_queue* ready_queue;
    job* currently_executing_job;
};


/*
ADT for a Resource

int resource_number; -> Unique identifier of the resource.
int total_instances; -> Number of total instances of the resource.
int remaining_instances; -> Number of instances of the resource that remain.
int priority_ceil_of_resource; -> priority ceil of the resource according to the priority ceiling protocol
priority_queue* currently_used_by; -> a priority queue which contains the jobs that are currently using this resource.

*/
struct resource
{
    
    int resource_number;
    int total_instances;
    int remaining_instances;
    int priority_ceil_of_resource;
    priority_queue* currently_used_by;
};


/*
ADT for a Resource Set

int number_of_resources; -> Number of resources the system has
resource* *resource_list_head; -> pointer to array of the pointer to the resources

*/
struct resource_list
{

    int number_of_resources;
    resource* *resource_list_head;
};


/*
ADT to store the resource usage information of a job for a single resource

resource* resource_name; -> Pointer to the resource
int instances_needed; -> number of instances of the resource we need.
int time_held_for; -> the amount of time we need the resoure for
int time_acquired_at_since_execution; -> the time from the start of the jobs execution at which we will need this resource.
int remaining_time_needed; -> how much more time we need the resource for.
int is_currently_holding; -> is the job currently holding this resource
int is_usage_over; -> has the job finished using this resource.
int priority_before_acquiring; -> what is the priority of the job before it acquires the resource. This is important to maintain as we need to give the job this priority after it releases this resource.

*/
struct resource_usage
{

    resource* resource_name;
    int instances_needed;
    int time_held_for;
    int time_acquired_at_since_execution;
    int remaining_time_needed;
    int is_currently_holding; //is the resource currently being held
    int is_usage_over;
    int priority_before_acquiring; 
};


/*
ADT for the array of resource usage information structures, one element per resource that the job uses

int number_of_resources; -> number of resources that the job needs
resource_usage* *list_head; -> pointer to array containing pointer to the resource usage info by the job

*/
struct req_resource_info
{
    int number_of_resources;
    resource_usage* *list_head;
};


/*
ADT for a Task

int phase; -> phase of the task.
int period; -> period of the task.
int execution_time; -> execution time of the task.
int relative_deadline; -> relative deadline of the task.
int task_number; -> this number uniquely identifies the task

*/
struct task
{
    int phase;
    int period;
    int execution_time;
    int relative_deadline;
    int task_number;
};


/*
ADT for a Task Set

int num_tasks; -> number of tasks in the system.
task* *list_head; -> pointer to the array containf pointers to tasks.

*/
struct task_set
{

    int num_tasks;
    task* *list_head;

};


/*
ADT for a Job

int assigned_priority; -> priority initally assigned to this job based on the the scheduling algorithm.
int current_priority; -> the current job priority. this changes due to resource contention as determined by the alogrithm used to manage resource contention.
int arrival_time; -> arrival time of the job in the system.
int execution_time; -> how long the job executes for.
int remaining_execution_time; -> how much execution time of the job is remaining.
int absolute_deadline; -> deadline of the job
int task_number; -> the task to which the job belongs to 
int job_number; -> the job number. {task number, job number} is used to uniquely identify a job in the system.
int state; -> state of the job. READY, EXECUTING and NOT_IN_SYSTEM are the possibl states of a job.
req_resource_info* resources_needed; -> this gives the details of the resource usage of the job.

*/
struct job
{
    int assigned_priority;
    int current_priority;
    int arrival_time;
    int execution_time;
    int remaining_execution_time;
    int absolute_deadline;
    int task_number;
    int job_number;
    int state;
    req_resource_info* resources_needed;
};


/*
ADT for list of Jobs

int number_of_jobs; -> total number of jobs that come within the specified time frame.
job* *job_list_head; -> pointer to an array containing pointer to each job.

*/
struct job_list
{
    int number_of_jobs;
    job* *job_list_head;
};


#endif /*DATA_STRUCTURES_H*/