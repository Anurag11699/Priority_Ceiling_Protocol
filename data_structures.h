#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

enum COLOR {Red, Black};

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


struct tree_node;
typedef struct tree_node tree_node;

struct red_black_tree;
typedef struct red_black_tree red_black_tree;

struct job_list;
typedef struct job_list job_list;


struct kernel
{
    int system_priority_ceiling;
};


/*
ADT for a Resource
*/
struct resource{
    
    int resource_number;
    int total_instances;
    int remaining_instances;
    int priority_ceil_of_resource;

    //a priority queue of jobs that currently hold this resource. This is needed so that we can make the highest priority job inherit the priority of a job in resource contention scenarios
    red_black_tree* currently_used_by;
};

/*
ADT for a Resource Set
*/
struct resource_list{

    int number_of_resources;
    resource* *resource_list_head;
};

/*
ADT to store the resource usage information of a job for a single resource
*/
struct resource_usage{

    resource* resource_name;
    int instances_needed;
    int time_held_for;
    int time_acquired_at_since_execution;
    int remaining_time_needed;
    int is_currently_holding; //is the resource currently being held
    int is_usage_over;
    int priority_before_acquiring; //Used to implement this --> When the requesting job J becomes blocked, the job Jl which blocks J inherits the current priority π(t) of J . The job Jl executes at its inherited priority π(t) until it releases R; at that time, the priority of Jl returns to its priority πl(t) at the time t when it acquires the resource R.
};


/*
ADT for the array of resource usage information structures, one element per resource that the job uses
*/
struct req_resource_info{
    int number_of_resources;
    resource_usage* *list_head;
};

/*
ADT for a Task
*/
struct task{
    int phase;
    int period;
    int execution_time;
    int relative_deadline;
    int task_number;
};

/*
ADT for a Task Set
*/
struct task_set{

    int num_tasks;
    task* *list_head;

};

/*
ADT for a Job
*/
struct job{
    int assigned_priority;
    int current_priority;
    int arrival_time;
    int execution_time;
    int remaining_execution_time;
    int absolute_deadline;
    int task_number;
    int job_number;
    int state;

    //this has all the resource usage information
    req_resource_info* resources_needed;
};


/*
ADT for list of Jobs
*/
struct job_list
{
    int number_of_jobs;
    job* *job_list_head;
};

/*
ADT for a RB Tree Node
*/
struct tree_node
{
	job *job_object;
	
    struct tree_node *right;
    struct tree_node *left;
    struct tree_node *parent;
    enum COLOR color;
};


/*
ADT for a Red Black Tree
*/
struct red_black_tree 
{
  tree_node *root;
  tree_node *NIL;
};



#endif /*DATA_STRUCTURES_H*/