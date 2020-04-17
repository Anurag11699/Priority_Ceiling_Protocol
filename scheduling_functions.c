#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<time.h>
#include "functions.h"

/*Comparator used to sort the jobs based on arrival time*/
int comparator_arrival_time_job(const void *p, const void *q) 
{ 
    job **a = (job **)p;
    job **b = (job **)q;
    return (*a)->arrival_time - (*b)->arrival_time;
} 

/* Used to calculate gcd of two numbers*/
int gcd(int a, int b) 
{ 
    if (b == 0) 
    {
        return a; 
    }
    return gcd(b, a % b); 
} 


/*
PreConditions
Inputs: {pointer to task set object}

Purpose of the Function: Calculate the hyperperiod of the task set

PostConditions
Return Value: Hyperperiod of the task set
The LRU way in the given index is replaced.
*/
int hyperperiod(task_set *schedule_task_set)
{
    
    
    int ans = schedule_task_set->list_head[0]->period;

    for (int i = 1; i < schedule_task_set->num_tasks; i++)
    {
        ans = (((schedule_task_set->list_head[i]->period * ans)) /(gcd(schedule_task_set->list_head[i]->period, ans))); 
    } 
       
  
    return ans; 
}


/*
PreConditions
Inputs: {pointer to task set object, pointer to resource list object, hyperperiod of the task set}

Purpose of the Function: This function initializes all jobs for each task in a given hyperperiod for a task set. It also assigns resources to each job at random.

PostConditions
Return Value: Pointer to the list of jobs that need to be executed within a given hyperperiod
*/
job_list* initialize_job_list(task_set *task_set_object,resource_list *resource_list_object ,int hyperperiod)
{
    job_list* job_list_object = (job_list*)malloc(sizeof(job_list));

    int num_job_in_hyperperiod=0;
    int i,j,k;

    for(i=0;i<task_set_object->num_tasks;i++)
    {
        num_job_in_hyperperiod=num_job_in_hyperperiod+ (hyperperiod/task_set_object->list_head[i]->period);
    }

    //fprintf(output_fd,"NUM JOBS: %d\n",num_job_in_hyperperiod);

    job_list_object->number_of_jobs=num_job_in_hyperperiod;
    job_list_object->job_list_head = (job **)malloc(sizeof(job *)*num_job_in_hyperperiod);

    int num_jobs_of_task;
    int job_num=0;
    for(i=0;i<task_set_object->num_tasks;i++)
    {
        num_jobs_of_task = (hyperperiod/task_set_object->list_head[i]->period);
        for(j=0;j<num_jobs_of_task;j++)
        {
            //fprintf(output_fd,"Task %d | Job %d\n",i,j);

            job* job_object = (job *)malloc(sizeof(job));
            job_object->arrival_time=(task_set_object->list_head[i]->period)*j + task_set_object->list_head[i]->phase;
            job_object->execution_time=(task_set_object->list_head[i]->execution_time);
            job_object->absolute_deadline=job_object->arrival_time+task_set_object->list_head[i]->relative_deadline;
            job_object->remaining_execution_time=job_object->execution_time;
            job_object->assigned_priority=task_set_object->list_head[i]->period;
            job_object->current_priority=job_object->assigned_priority;
            job_object->task_number=i;
            job_object->job_number=j;
            job_object->state=NOT_IN_SYSTEM;
            
            
            
            
            //assigning resources at random to this job
            req_resource_info* req_resource_info_object = (req_resource_info *)malloc(sizeof(req_resource_info));

            


            job_object->resources_needed = req_resource_info_object;

            //to get a random number of resources the job needs
            int num_resources = (rand() % (resource_list_object->number_of_resources + 1));
            
            
            
            //fprintf(output_fd,"Number of Resources: %d\n",num_resources);

            req_resource_info_object->number_of_resources=num_resources;

            req_resource_info_object->list_head = (resource_usage **)malloc(num_resources * sizeof(resource_usage *));

            

            //generate num_resources random numbers between 0 and num_resouces-1 which are the resources used by this job. 
            int random_numbers[num_resources];
            int count=0;

            //done so that each resource used by the job will be unique 
            while(count < num_resources) 
            {
                int randNum =(rand() % resource_list_object->number_of_resources); //Generate a random number in range
                int found =0; 

                //make sure the random number hasnt already been generated
                for (int m = 0; m < count; m++) {
                    if(random_numbers[m] == randNum) 
                    {
                        found =1;
                        break; //once we have located the number we dont need to continue checking
                    }
                }
            
                //we have a new random number
                if(!found) 
                {
                    random_numbers[count] =randNum;
                    count++;
                }
            }

            

            for(k=0;k<num_resources;k++)
            {
                

                resource_usage * resource_usage_object = (resource_usage *)malloc(sizeof(resource_usage));
                req_resource_info_object->list_head[k]=resource_usage_object;
               
                req_resource_info_object->list_head[k]->resource_name=resource_list_object->resource_list_head[random_numbers[k]];

                
                req_resource_info_object->list_head[k]->is_currently_holding=0;

                req_resource_info_object->list_head[k]->is_usage_over=0;

                req_resource_info_object->list_head[k]->instances_needed = (rand() % (resource_list_object->resource_list_head[random_numbers[k]]->total_instances) + 1);

                req_resource_info_object->list_head[k]->time_acquired_at_since_execution = (rand() % (job_object->execution_time));

                req_resource_info_object->list_head[k]->time_held_for = (rand() % (job_object->execution_time - req_resource_info_object->list_head[k]->time_acquired_at_since_execution)+1);

                req_resource_info_object->list_head[k]->remaining_time_needed = req_resource_info_object->list_head[k] -> time_held_for;

                req_resource_info_object->list_head[k]->priority_before_acquiring=job_object->current_priority;
            }

            
            

            job_list_object->job_list_head[job_num]=job_object;
            job_num++;

            
        }

    }
    

    return job_list_object;
}


void update_priority_ceiling(kernel* kernel_object,job_list* job_list_object, resource_list* resource_list_object)
{

    int need_to_ceil[resource_list_object->number_of_resources];
    int max_job_priority_needing_resource[resource_list_object->number_of_resources];
    int i,j;

    kernel_object->system_priority_ceiling=__INT16_MAX__;

    for(i=0;i<resource_list_object->number_of_resources;i++)
    {
        need_to_ceil[i]=0;
        max_job_priority_needing_resource[i]=__INT16_MAX__;
    }

    priority_queue_node* walker=kernel_object->ready_queue->priority_queue_head->next;

    //all jobs in the ready queue
    while(walker!=NULL)
    //for(i=0;i<job_list_object->number_of_jobs;i++)
    {
        //job* job_object = job_list_object->job_list_head[i];

        job* job_object = walker->job_object;

        //if(job_object->state==READY || job_object->state==EXECUTING)
        //{
            
            for(j=0;j<job_object->resources_needed->number_of_resources;j++)
            {
                resource_usage* resource_usage_object = job_object->resources_needed->list_head[j];

                if(resource_usage_object->is_usage_over==0 &&resource_usage_object->is_currently_holding==0)
                {
                    //fprintf(output_fd,"Instances needed by Job: %d| Remaining Instances: %d\n",resource_usage_object->instances_needed,resource_usage_object->resource_name->remaining_instances);
                    if(resource_usage_object->resource_name->remaining_instances < resource_usage_object->instances_needed)
                    {
                        need_to_ceil[resource_usage_object->resource_name->resource_number]=1;
                        
                    }

                    max_job_priority_needing_resource[resource_usage_object->resource_name->resource_number] = min(max_job_priority_needing_resource[resource_usage_object->resource_name->resource_number], job_object->assigned_priority);
                }
                
            }
            
        //}
        walker=walker->next;
    }

    job* job_object;
    //the currently executing job
    if(kernel_object->currently_executing_job!=NULL)
    {
        job_object = kernel_object->currently_executing_job;
    }
    else
    {
        goto update_priority;
    }
    
    for(j=0;j<job_object->resources_needed->number_of_resources;j++)
    {
        resource_usage* resource_usage_object = job_object->resources_needed->list_head[j];

        if(resource_usage_object->is_usage_over==0 &&resource_usage_object->is_currently_holding==0)
        {
            //fprintf(output_fd,"Instances needed by Job: %d| Remaining Instances: %d\n",resource_usage_object->instances_needed,resource_usage_object->resource_name->remaining_instances);
            if(resource_usage_object->resource_name->remaining_instances < resource_usage_object->instances_needed)
            {
                need_to_ceil[resource_usage_object->resource_name->resource_number]=1;
                
            }

            max_job_priority_needing_resource[resource_usage_object->resource_name->resource_number] = min(max_job_priority_needing_resource[resource_usage_object->resource_name->resource_number], job_object->assigned_priority);
        }
        
    }

    update_priority:
    for(i=0;i<resource_list_object->number_of_resources;i++)
    {
        if(need_to_ceil[i]==1)
        {
            resource_list_object->resource_list_head[i]->priority_ceil_of_resource = max_job_priority_needing_resource[i];
        }
        else
        {
            resource_list_object->resource_list_head[i]->priority_ceil_of_resource=__INT16_MAX__;
        }
        

        kernel_object->system_priority_ceiling = min(kernel_object->system_priority_ceiling,resource_list_object->resource_list_head[i]->priority_ceil_of_resource);
    }

}

void print_priority_ceilings(kernel* kernel_object,resource_list* resource_list_object)
{   
    int i;

    //printing the ceilings
    fprintf(output_fd,"Resource Wise Priority Ceilings\n");
    fflush(output_fd);

    for(i=0;i<resource_list_object->number_of_resources;i++)
    {
       fprintf(output_fd,"Resouce %d: Priotity Ceiling %d | Remaining Instances: %d\n",i,resource_list_object->resource_list_head[i]->priority_ceil_of_resource,resource_list_object->resource_list_head[i]->remaining_instances);
       fflush(output_fd);
    }
    fprintf(output_fd,"\n");

    fprintf(output_fd,"System Priority Ceiling: %d\n",kernel_object->system_priority_ceiling);
    fflush(output_fd);
}



/*
PreConditions
Inputs: {pointer to task set object, pointer to the resource list object, scheduling algorithm that needs to be used}

Purpose of the Function: Schedules the task set using the scheduling algorithm and deals with resources using priority inheritance protocol

Basic Outline of the Function:

1. Initialize Ready Queue and find hyperperiod of the task set. Initialize other needed variables also
2. Iterate for each decision point in the hyperperiod as follows
    a. If a job is executing, reduce its remaining execution time by the difference between current decison point time and the time of the previous decision point (time_lapse)
    b.Update the remaining time of any resources held by the job and check if the decision point is due to the currently executing job releasing a resource. If it is, do the necessary action.
    c. Check if the decison point is due to job departure, if it is then free the job
    d. Check if decision point due to job arrival, insert jobs in the ready queue if there time matches the arrival time of any job.
    e. Pick a new job from ready queue. If a newly arrived job has highest priority then preempt the currently executing job.
    f. Check if decision point is due to resource request. Deal with resource requests according to the priority inheritance protocol. 
    g. Calculate the next resource time, next resource release time, next job arrival time, next job departure time. Our next decision point time will be the min of all these times
3. Display statistics related to scheduling.

PostConditions
Scheduled task set 

*/
void scheduler_priority_ceiling(kernel* kernel_object,task_set *priority_inheritance_task_set, resource_list* resource_list_object, int scheduling_algo)
{
    //initialzing variables to calculate number of decision points and number of preemptions
    int number_of_decision_points=0;
    int number_of_preemptions=0;


    //initializing variables to calculate min, max, and average response times for each task
    int min_response_time[priority_inheritance_task_set->num_tasks];
    int max_response_time[priority_inheritance_task_set->num_tasks];
    float avg_response_time[priority_inheritance_task_set->num_tasks];
    int num_jobs_per_task[priority_inheritance_task_set->num_tasks];
    int i=0;

    for(i=0;i<priority_inheritance_task_set->num_tasks;i++)
    {
        min_response_time[i] = __INT16_MAX__;
        max_response_time[i] = 0;
        avg_response_time[i] = 0;
        num_jobs_per_task[i] = 0;
    }


    //calculating hyperpeiod of the task set
    int priority_inheritance_task_set_hyperperiod = hyperperiod(priority_inheritance_task_set);
    fprintf(output_fd,"Hyperperiod of the task set for priority inheritance is: %d\n",priority_inheritance_task_set_hyperperiod);
    fflush(output_fd);

    //creating a ready queue
    //red_black_tree* ready_queue = new_red_black_tree();

    //getting the list of jobs
    job_list* job_list_object = initialize_job_list(priority_inheritance_task_set, resource_list_object,priority_inheritance_task_set_hyperperiod);

    //print_job_list(job_list_object);

    //sort the list of jobs based on arrival times
    qsort((void*)(job_list_object->job_list_head), job_list_object->number_of_jobs, sizeof(job_list_object->job_list_head[0]), comparator_arrival_time_job);

    print_job_list(job_list_object);

    //job *currently_executing_job=NULL;
    //job *new_job=NULL;


    int decision_point_time=0;
    int new_job_arrival_time=job_list_object->job_list_head[0]->arrival_time;
    int current_job_departure_time=__INT_MAX__;
    int next_resource_request_time=0;
    int next_resource_release_time=0;
    int job_num=0;
    int time_lapse=0;

    decision_point_time = min(new_job_arrival_time,current_job_departure_time);


    fprintf(output_fd,"PRINTING THE SCHEDULE\n\n");
    fflush(output_fd);

    print_priority_ceilings(kernel_object,resource_list_object);

    while(decision_point_time<=priority_inheritance_task_set_hyperperiod)
    {
        fprintf(output_fd,"\n\nCurrent Time: %d\n",decision_point_time);
        fflush(output_fd);

        //every loop iteration is a decision point
        number_of_decision_points++;

        if(kernel_object->currently_executing_job!=NULL)
        {
            kernel_object->currently_executing_job->remaining_execution_time = max(0,kernel_object->currently_executing_job->remaining_execution_time-time_lapse);
        }
        

        //check if the decision point is due to a resource release

        //update the time remaining for the resources held by the currently executing process
        if(kernel_object->currently_executing_job!=NULL)
        {
            for(int i=0;i<kernel_object->currently_executing_job->resources_needed->number_of_resources;i++)
            {   
                //for the resources the job was holding since the prev decision point
                if(kernel_object->currently_executing_job->resources_needed->list_head[i]->is_currently_holding==1)
                {
                    kernel_object->currently_executing_job->resources_needed->list_head[i]->remaining_time_needed = max(0,kernel_object->currently_executing_job->resources_needed->list_head[i]->remaining_time_needed-time_lapse);

                    fprintf(output_fd,"Job J{%d,%d} | Resource R%d | Time Left %d\n",kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number,kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->resource_number,kernel_object->currently_executing_job->resources_needed->list_head[i]->remaining_time_needed);
                    fflush(output_fd);

                    if(kernel_object->currently_executing_job->resources_needed->list_head[i]->remaining_time_needed==0)
                    {
                        //the resource can be released
                        kernel_object->currently_executing_job->resources_needed->list_head[i]->is_currently_holding=0;
                        kernel_object->currently_executing_job->resources_needed->list_head[i]->is_usage_over=1;

                        //free the resource instances
                        kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->remaining_instances = kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->remaining_instances + kernel_object->currently_executing_job->resources_needed->list_head[i]->instances_needed;
                        
                        //the job now gets the priority it had before it acquired the resource
                        //kernel_object->currently_executing_job->current_priority=kernel_object->currently_executing_job->resources_needed->list_head[i]->priority_before_acquiring;
                        insert_priority_queue(kernel_object->ready_queue,kernel_object->currently_executing_job);
                        update_job_priority(kernel_object,resource_list_object,kernel_object->currently_executing_job,kernel_object->currently_executing_job->resources_needed->list_head[i]->priority_before_acquiring);
                        delete_job_priority_queue(kernel_object->ready_queue,kernel_object->currently_executing_job);

                        //remove the job from the priority queue of the resource
                        fprintf(output_fd,"REMOVING J{%d,%d} FROM RESOURCE %d\n",kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number, kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->resource_number);
                        fflush(output_fd);
                        print_priority_queue(kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->currently_used_by);
                       delete_job_priority_queue(kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->currently_used_by,kernel_object->currently_executing_job);
                         print_priority_queue(kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->currently_used_by);
                        
                        //update the priority ceilings after freeing this resource
                        update_priority_ceiling(kernel_object,job_list_object,resource_list_object);

                        print_priority_ceilings(kernel_object,resource_list_object);



                        fprintf(output_fd,"Job J{%d,%d} Done with | Resource R%d | Time Left %d\n",kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number,kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->resource_number,kernel_object->currently_executing_job->resources_needed->list_head[i]->remaining_time_needed);
                        fflush(output_fd);


                    }
                }
            }
        }

        
        


        //check if decision point is due to job departure
        if(kernel_object->currently_executing_job!=NULL)
        {

            if(kernel_object->currently_executing_job->remaining_execution_time==0)
            {
                fprintf(output_fd,"J{%d,%d} Finished Execution\n",kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number);
                fflush(output_fd);

                if(decision_point_time>kernel_object->currently_executing_job->absolute_deadline)
                {
                    fprintf(output_fd,"J{%d,%d} Missed its Deadline\n", kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number);
                    fflush(output_fd);
                }

                //calculating response times

                int response_time = decision_point_time-kernel_object->currently_executing_job->arrival_time;

                min_response_time[kernel_object->currently_executing_job->task_number] = min(min_response_time[kernel_object->currently_executing_job->task_number],response_time);
                max_response_time[kernel_object->currently_executing_job->task_number] = max(max_response_time[kernel_object->currently_executing_job->task_number],response_time);
                avg_response_time[kernel_object->currently_executing_job->task_number] = avg_response_time[kernel_object->currently_executing_job->task_number] + response_time;
                num_jobs_per_task[kernel_object->currently_executing_job->task_number]++;

                //set state to terminated
                kernel_object->currently_executing_job->state=NOT_IN_SYSTEM;

                //free(kernel_object->currently_executing_job);

                kernel_object->currently_executing_job=NULL;
            }
        }



        //check if decision point is due to job arrival
        while(job_num<job_list_object->number_of_jobs && decision_point_time==job_list_object->job_list_head[job_num]->arrival_time)
        {
            //decision point time, insert job into ready queue

            job_list_object->job_list_head[job_num]->absolute_deadline = decision_point_time + priority_inheritance_task_set->list_head[job_list_object->job_list_head[job_num]->task_number]->relative_deadline;

            if(scheduling_algo==RM)
            {
                job_list_object->job_list_head[job_num]->assigned_priority = priority_inheritance_task_set->list_head[job_list_object->job_list_head[job_num]->task_number]->period;

                job_list_object->job_list_head[job_num]->current_priority = job_list_object->job_list_head[job_num]->assigned_priority;

                
            }
            else if(scheduling_algo==EDF)
            {
                job_list_object->job_list_head[job_num]->assigned_priority = job_list_object->job_list_head[job_num]->absolute_deadline;

                job_list_object->job_list_head[job_num]->current_priority = job_list_object->job_list_head[job_num]->assigned_priority;
            }

            //set state to ready for job
            job_list_object->job_list_head[job_num]->state=READY;

            insert_priority_queue(kernel_object->ready_queue,job_list_object->job_list_head[job_num]);
            

            fprintf(output_fd,"Insert J{%d,%d} with priority %d in Ready Queue\n",job_list_object->job_list_head[job_num]->task_number,job_list_object->job_list_head[job_num]->job_number,job_list_object->job_list_head[job_num]->current_priority);
            fflush(output_fd);
            job_num++;
        }

        //find next arrival time
        new_job_arrival_time=__INT16_MAX__;

        if(job_num<job_list_object->number_of_jobs)
        {
            new_job_arrival_time=job_list_object->job_list_head[job_num]->arrival_time;
        }

         //write function to update priority ceilings of resources and the system
        update_priority_ceiling(kernel_object,job_list_object,resource_list_object);
        print_priority_ceilings(kernel_object,resource_list_object);

        //pick a job from ready queue
        if(is_priority_queue_empty(kernel_object->ready_queue)==0)
        {
            fprintf(output_fd,"HELLO\n");
            fflush(output_fd);

            if(kernel_object->currently_executing_job==NULL)
            {
                //extract job with lowest priority
                kernel_object->currently_executing_job = remove_max_priority_queue(kernel_object->ready_queue);
            }
            else
            {
                //find the minimum job
                job* new_job =  get_max_priority_queue(kernel_object->ready_queue);

                if(new_job->current_priority<kernel_object->currently_executing_job->current_priority)
                {
                    //insert the currently executing job back into the ready queue
                    insert_priority_queue(kernel_object->ready_queue,kernel_object->currently_executing_job);

                    //extract another job
                    kernel_object->currently_executing_job = remove_max_priority_queue(kernel_object->ready_queue);

                    //this is a preemption
                    number_of_preemptions++;
                }
            }
            
        }
        else
        {
            int next_decision_point_time = new_job_arrival_time;
            time_lapse = next_decision_point_time - decision_point_time;

            decision_point_time = next_decision_point_time;
            continue;
        }
        
        fprintf(output_fd,"Currently Executing J{%d,%d}\n",kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number);
        fflush(output_fd);

        fprintf(output_fd,"\nTime: %d | Currently Executing Job: J{%d,%d}\n\n",decision_point_time,kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number);
        fflush(output_fd);

        //check if decision point is due to resource request
        int resource_needed_index;

        

        //the currently executing process needs a resource

        for(resource_needed_index=0;resource_needed_index<kernel_object->currently_executing_job->resources_needed->number_of_resources;resource_needed_index++)
        {
            if(((kernel_object->currently_executing_job->execution_time-kernel_object->currently_executing_job->remaining_execution_time)==kernel_object->currently_executing_job->resources_needed->list_head[resource_needed_index]->time_acquired_at_since_execution) && kernel_object->currently_executing_job->resources_needed->list_head[resource_needed_index]->is_currently_holding==0 && kernel_object->currently_executing_job->resources_needed->list_head[resource_needed_index]->is_usage_over==0)
            {   
                

                resource_usage* req_resource_usage = kernel_object->currently_executing_job->resources_needed->list_head[resource_needed_index];

                // fprintf(output_fd,"Resource Needed: R%d\n",req_resource_usage->resource_name->resource_number);
                // fflush(output_fd);

                //check if the currently executing job holds the resource which is responsible for system priority ceiling
                int is_responsible=0;
                for(int i=0;i<kernel_object->currently_executing_job->resources_needed->number_of_resources;i++)
                {
                    if(kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->priority_ceil_of_resource == kernel_object->system_priority_ceiling && kernel_object->currently_executing_job->resources_needed->list_head[i]->is_currently_holding==1)
                    {
                        is_responsible=1;
                    }
                }

                resource * resource_responsible=NULL;

                //need to check which resources are responsible for the ceil of the system
                for(int i=0;i<resource_list_object->number_of_resources;i++)
                {
                    if(resource_list_object->resource_list_head[i]->priority_ceil_of_resource == kernel_object->system_priority_ceiling)
                    {
                        resource_responsible = resource_list_object->resource_list_head[i];
                        break;
                    }
                }

                //check if that resource can be given to this job
                if(kernel_object->currently_executing_job->current_priority<kernel_object->system_priority_ceiling || (kernel_object->currently_executing_job->current_priority == kernel_object->system_priority_ceiling && is_responsible==1))
                {
                    
                    //give the resource to this job
                    req_resource_usage->is_currently_holding=1;

                    req_resource_usage->resource_name->remaining_instances = req_resource_usage->resource_name->remaining_instances - req_resource_usage->instances_needed;

                    req_resource_usage->priority_before_acquiring=kernel_object->currently_executing_job->current_priority;


                    //add this job to the currently used by queue of the resource
                    fprintf(output_fd,"Inserting job into resource %d\n",req_resource_usage->resource_name->resource_number);
                    fflush(output_fd);

                    insert_priority_queue(req_resource_usage->resource_name->currently_used_by,kernel_object->currently_executing_job);

                    print_priority_queue(req_resource_usage->resource_name->currently_used_by);


                    //update the priority ceilings after allocating this resource
                    update_priority_ceiling(kernel_object,job_list_object,resource_list_object);

                    fprintf(output_fd,"Resource R%d given to J{%d,%d}\n",req_resource_usage->resource_name->resource_number,kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number);
                    fflush(output_fd);
                    
                    
                }
                else //resource cannot be given to this job
                {
    

                    fprintf(output_fd,"IN INHERTIANCE\n");
                    fflush(output_fd);

                    if(is_priority_queue_empty(resource_responsible->currently_used_by)==1)
                    {
                        fprintf(output_fd,"ERROR\n");
                        fflush(output_fd);
                    }
                    
                    //job of the current highest priority which owns this resource
                    //job* job_to_inherit_priority = RB_tree_get_min(req_resource_usage->resource_name->currently_used_by);

                    //job of the current highest priority that is responsible fot the ceiling
                    job* job_to_inherit_priority = get_max_priority_queue(resource_responsible->currently_used_by);

                    fprintf(output_fd,"J{%d,%d} with Priority %d blocked by J{%d,%d} with Priority %d\n",kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number,kernel_object->currently_executing_job->current_priority,job_to_inherit_priority->task_number,job_to_inherit_priority->job_number,job_to_inherit_priority->current_priority);
                    fflush(output_fd);

                    //inorder(req_resource_usage->resource_name->currently_used_by, req_resource_usage->resource_name->currently_used_by->root);

                    // make this job inherit the priority of the currently executing job
                   
                    update_job_priority(kernel_object,resource_list_object,job_to_inherit_priority,kernel_object->currently_executing_job->current_priority);

                    print_priority_ceilings(kernel_object,resource_list_object);


                    //inorder(req_resource_usage->resource_name->currently_used_by, req_resource_usage->resource_name->currently_used_by->root);

                    fprintf(output_fd,"After Priority Inheritance J{%d,%d} has Priority %d\n",job_to_inherit_priority->task_number,job_to_inherit_priority->job_number,job_to_inherit_priority->current_priority);
                    fflush(output_fd);


                    //currently executing job is blocked
                    job* new_currently_executing_job = remove_max_priority_queue(kernel_object->ready_queue);
                    insert_priority_queue(kernel_object->ready_queue,kernel_object->currently_executing_job);

                    kernel_object->currently_executing_job = new_currently_executing_job;

                    fprintf(output_fd,"After Priority Inheritance, currently executing job is J{%d,%d} with Priority %d\n",kernel_object->currently_executing_job->task_number,kernel_object->currently_executing_job->job_number,kernel_object->currently_executing_job->current_priority);
                    fflush(output_fd);

                    number_of_preemptions++;

                }
                
            }
        }
        


        

        //find next time for resource request
        next_resource_request_time=__INT16_MAX__;
        for(int i=0;i<kernel_object->currently_executing_job->resources_needed->number_of_resources;i++)
        {
            if(kernel_object->currently_executing_job->resources_needed->list_head[i]->is_currently_holding==0)
            {
                int time_when_requested =  decision_point_time + (kernel_object->currently_executing_job->resources_needed->list_head[i]->time_acquired_at_since_execution - (kernel_object->currently_executing_job->execution_time - kernel_object->currently_executing_job->remaining_execution_time));

                //this happens for resources that have already been used
                if(time_when_requested<=decision_point_time)
                {
                    continue;
                }

                fprintf(output_fd,"Next Resource Request is for R%d at Time %d\n",kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->resource_number,time_when_requested);
                fflush(output_fd);

                next_resource_request_time = min(next_resource_request_time,time_when_requested);

                
            }
        }

        //find the next time for resource release
        next_resource_release_time=__INT16_MAX__;

        for(int i=0;i<kernel_object->currently_executing_job->resources_needed->number_of_resources;i++)
        {
            if(kernel_object->currently_executing_job->resources_needed->list_head[i]->is_currently_holding==1)
            {
                int time_when_released =  decision_point_time + kernel_object->currently_executing_job->resources_needed->list_head[i]->remaining_time_needed;


                fprintf(output_fd,"Next Resource Release is for R%d at Time %d\n",kernel_object->currently_executing_job->resources_needed->list_head[i]->resource_name->resource_number,time_when_released);
                fflush(output_fd);
                
                next_resource_release_time = min(next_resource_request_time,time_when_released);

                
            }
        }




        current_job_departure_time = kernel_object->currently_executing_job->remaining_execution_time + decision_point_time;

        fprintf(output_fd,"Current Job  Departure Time %d\n",current_job_departure_time);
        fprintf(output_fd,"New Job Arrival Time %d\n",new_job_arrival_time);
        fflush(output_fd);

        int next_decision_point_time = min(min(current_job_departure_time,new_job_arrival_time),min(next_resource_release_time,next_resource_request_time));

        time_lapse = next_decision_point_time - decision_point_time;

        decision_point_time = next_decision_point_time;
    
    }


    //calculate average response time
    for(i=0;i<priority_inheritance_task_set->num_tasks;i++)
    {
        avg_response_time[i] = avg_response_time[i]/num_jobs_per_task[i];
    }

    //print the response times

    fprintf(output_fd,"\n\nMin Response Times for each Task\n");
    for(i=0;i<priority_inheritance_task_set->num_tasks;i++)
    {
        fprintf(output_fd,"Task #%d: %d\n",i,min_response_time[i]);
        fflush(output_fd);
    }

    fprintf(output_fd,"\n\nMax Response Times for each Task\n");
    for(i=0;i<priority_inheritance_task_set->num_tasks;i++)
    {
        fprintf(output_fd,"Task #%d: %d\n",i,max_response_time[i]);
        fflush(output_fd);
    }

    fprintf(output_fd,"\n\nAvg Response Times for each Task\n");
    for(i=0;i<priority_inheritance_task_set->num_tasks;i++)
    {
        fprintf(output_fd,"Task #%d: %f\n",i,avg_response_time[i]);
        fflush(output_fd);
    }

    fprintf(output_fd,"\nNumber of Decision Points: %d\n",number_of_decision_points);
    fprintf(output_fd,"\nNumber of Preemption Points: %d\n",number_of_preemptions);

    //ready queue is not empty
    if(is_priority_queue_empty(kernel_object->ready_queue)==0)
    {
        fprintf(output_fd,"ALL JOBS COULD NOT BE SCHEDULED\n");
        fflush(output_fd);
    }

}


/*auxillary function used to print the list of jobs and the resources they need*/
void print_job_list(job_list* job_list_object)
{
    fprintf(output_fd,"JOB LIST SPECIFICATIONS\n\n");
    fflush(output_fd);
    for(int i=0;i<job_list_object->number_of_jobs;i++)
    {
        fprintf(output_fd,"J{%d,%d} | Arrival Time: %d | Execution Time: %d\n",job_list_object->job_list_head[i]->task_number,job_list_object->job_list_head[i]->job_number,job_list_object->job_list_head[i]->arrival_time,job_list_object->job_list_head[i]->execution_time);

        fprintf(output_fd,"Resources Needed By Job\n");
        fflush(output_fd);

        req_resource_info* req_resource_info_object = job_list_object->job_list_head[i]->resources_needed;

        for(int k = 0; k<req_resource_info_object->number_of_resources;k++)
        {
            fprintf(output_fd,"Resource Name: %d | Instances Needed: %d | Start Time: %d | Time Neeeded For: %d\n",req_resource_info_object->list_head[k]->resource_name->resource_number,req_resource_info_object->list_head[k]->instances_needed,req_resource_info_object->list_head[k]->time_acquired_at_since_execution,req_resource_info_object->list_head[k]->time_held_for);
            fflush(output_fd);
        }

        fprintf(output_fd,"\n\n");
        fflush(output_fd);
    }
}