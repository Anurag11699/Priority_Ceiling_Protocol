#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include "functions.h"
#include<assert.h>


/*
Purpose of the Function: Initialize a priority queue

PostConditions
pointer_queue_object!=NULL
*/
priority_queue* initialize_priority_queue()
{
    priority_queue* priority_queue_object = (priority_queue*)malloc(sizeof(priority_queue));
    priority_queue_object->priority_queue_head = (priority_queue_dummy_head *)malloc(sizeof(priority_queue_dummy_head));
    priority_queue_object->priority_queue_head->number_of_jobs=0;
    priority_queue_object->priority_queue_head->next=NULL;
    priority_queue_object->priority_queue_tail=NULL;

    //check PostConditions
    assert(priority_queue_object!=NULL && "Pointer to priority queue cannot be null");
    
    return priority_queue_object;
}


/*
PreConditions
Inputs: {pointer to priority queue object, pointer to job object}
priority_queue_object!=NULL
job_object!=NULL


Purpose of the Function: This function is used to insert a job in the priority queue. The job is inserted in ascending order of current priority.

PostConditions
*/
void insert_priority_queue(priority_queue* priority_queue_object,job* job_object)
{
    //check PreConditions
    assert(priority_queue_object!=NULL && "Pointer to priority queue cannot be null");
    assert(job_object!=NULL && "Pointer to job cannot be null");
    
    priority_queue_node *priority_queue_new_node = (priority_queue_node*)malloc(sizeof(priority_queue_node));
    priority_queue_new_node->job_object=job_object;

    if(priority_queue_object->priority_queue_head->next==NULL)
    {
        priority_queue_new_node->next=NULL;
        priority_queue_object->priority_queue_head->next=priority_queue_new_node;
        priority_queue_object->priority_queue_head->number_of_jobs++;
        return;
    }

    
    priority_queue_node* walker;
    priority_queue_node* walker_prev;
    walker_prev = priority_queue_object->priority_queue_head->next;
    walker=priority_queue_object->priority_queue_head->next->next;

    
    while(walker!=NULL && (walker->job_object->current_priority > job_object->current_priority && walker_prev->job_object->current_priority<= job_object->current_priority))
    {
        walker_prev=walker;
        walker=walker->next;
    }

   

    priority_queue_new_node->next=walker;
    walker_prev->next = priority_queue_new_node;
    priority_queue_object->priority_queue_head->number_of_jobs++;

}


/*
PreConditions
Inputs: {pointer to priority queue object}
priority_queue_object!=NULL

Purpose of the Function: This function is used to return the job with the highest priority (lowest priority number) from the priority queue. As the jobs are ordered in priority order, it returns the job in the first node of the priority queue.

PostConditions
*/
job* get_max_priority_queue(priority_queue* priority_queue_object)
{
    //check PreConditions
    assert(priority_queue_object!=NULL && "Pointer to priority queue cannot be null");

    if(is_priority_queue_empty(priority_queue_object)==1)
    {
        return NULL;
    }

    return priority_queue_object->priority_queue_head->next->job_object;
}

/*
PreConditions
Inputs: {pointer to priority queue object}
priority_queue_object!=NULL

Purpose of the Function: This function is used to return the job with the highest priority (lowest priority number) from the priority queue and remove that job from the priority queue. As the jobs are ordered in priority order, it returns the job in the first node of the priority queue.

PostConditions
Updated priority queue
*/
job* remove_max_priority_queue(priority_queue* priority_queue_object)
{
    //check PreConditions
    assert(priority_queue_object!=NULL && "Pointer to priority queue cannot be null");

    if(is_priority_queue_empty(priority_queue_object)==1)
    {
        return NULL;
    }
    priority_queue_node* temp=priority_queue_object->priority_queue_head->next;
    job* job_object = priority_queue_object->priority_queue_head->next->job_object;
    priority_queue_object->priority_queue_head->next = priority_queue_object->priority_queue_head->next->next;

    free(temp);
    priority_queue_object->priority_queue_head->number_of_jobs--;

    return job_object;

}


/*
PreConditions
Inputs: {pointer to priority queue object, pointer to job object to be deleted}
priority_queue_object!=NULL
job_object!=NULL

Purpose of the Function: This function is used to delete a specific job from the priority queue.

PostConditions
Updated priority queue without the particular job
Return Value: 1 if the job was present and deleted.
Return Value: 0 if the job was not present and hence not deleted.
*/
int delete_job_priority_queue(priority_queue* priority_queue_object,job* job_object)
{
    //check PreConditions
    assert(priority_queue_object!=NULL && "Pointer to priority queue cannot be null");
    assert(job_object!=NULL && "Pointer to job cannot be null");

    if(is_priority_queue_empty(priority_queue_object)==1)
    {
        return 0;
    }

    if(priority_queue_object->priority_queue_head->next->job_object==job_object)
    {
        priority_queue_node* temp=priority_queue_object->priority_queue_head->next;
        priority_queue_object->priority_queue_head->next=priority_queue_object->priority_queue_head->next->next;
        free(temp);
        return 1;
    }

    priority_queue_node* walker;
    priority_queue_node* walker_prev;
    walker_prev = priority_queue_object->priority_queue_head->next;
    walker=priority_queue_object->priority_queue_head->next->next;

    while(walker!=NULL && walker->job_object!=job_object)
    {
        walker_prev=walker;
        walker=walker->next;
    }

    if(walker==NULL)
    {
        return 0;
    }

    if(walker->job_object==job_object)
    {
        // fprintf(output_fd,"REMOVING JOB J{%d,%d}\n",walker->job_object->task_number,walker->job_object->job_number);
        // fflush(output_fd);

        walker_prev->next=walker->next;
        free(walker);
        priority_queue_object->priority_queue_head->number_of_jobs--;
        return 1;
    }

    return 0;
    
}

/*
PreConditions
Inputs: {pointer to priority queue object}
priority_queue_object!=NULL


Purpose of the Function: This function is used to check if the priority queue is empty or no.

PostConditions
Updated priority queue without the particular job
Return Value: 1, priority queue is empty
Return Value: 0, priority queue is not empty
*/
int is_priority_queue_empty(priority_queue* priority_queue_object)
{
    //check PreConditions
    assert(priority_queue_object!=NULL && "Pointer to priority queue cannot be null");

    if(priority_queue_object->priority_queue_head->next==NULL || priority_queue_object->priority_queue_head->number_of_jobs==0)
    {
        return 1;
    }

    return 0;
}

/*
PreConditions
Inputs: {pointer to priority queue, pointer to resource list object,pointer to job to inherit new priorty, new priority for job}
kernel_object!=NULL
resource_list_object!=NULL
job_object!=NULL
new_priority>=0

Purpose of the Function: This function is used to update the priority of the job given and update the priority queues of the resources that hold this job. This is called when a job is blocked by a lower priority job holding a resource that it requires. Hence the priority queues for both the ready queue and for all the resources that are holding this job must be updated

PostConditions
Updated priority of job and its respective priority queues
*/
void update_job_priority(kernel* kernel_object,resource_list* resource_list_object,job* job_object,int new_priority)
{
    //check PreConditions
    assert(kernel_object!=NULL && "Pointer to kernel object cannot be null");
    assert(resource_list_object!=NULL && "Pointer to resource list cannot be null");
    assert(job_object!=NULL && "Pointer to job cannot be null");

    assert(new_priority>=0 && "Priority cannot be less than 0");

    // fprintf(output_fd,"Job to Inherit-> J{%d,%d}\n",job_object->task_number,job_object->job_number);
    // fflush(output_fd);
    
    job_object->current_priority=new_priority;

    //remove from ready queue
    delete_job_priority_queue(kernel_object->ready_queue,job_object);
    insert_priority_queue(kernel_object->ready_queue,job_object);


    for(int i=0;i<resource_list_object->number_of_resources;i++)
    {
        if(delete_job_priority_queue(resource_list_object->resource_list_head[i]->currently_used_by,job_object)==1)
        {
            insert_priority_queue(resource_list_object->resource_list_head[i]->currently_used_by,job_object);
        }
    }

}

void print_priority_queue(priority_queue* priority_queue_object)
{
    if(is_priority_queue_empty(priority_queue_object)==1)
    {
        return;
    }

    priority_queue_node* walker;
    //priority_queue_node* walker_prev;
    walker=priority_queue_object->priority_queue_head->next;

    while(walker!=NULL)
    {
        fprintf(output_fd,"J{%d,%d}->",walker->job_object->task_number,walker->job_object->job_number);
        fflush(output_fd);
        walker=walker->next;
    }
    fprintf(output_fd,"\n");
    fflush(output_fd);
}