#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include "functions.h"


int main()
{
    output_fd = fopen(output_file,"w");
    srand(time(0));

    FILE *resource_fd = fopen(resource_input_file,"r");

    //creating resources 

    int number_of_resources,i;
    fscanf(resource_fd,"%d",&number_of_resources);

    resource_list* resource_list_object = (resource_list *)malloc(sizeof(resource_list));
    resource_list_object->number_of_resources=number_of_resources;
    resource_list_object->resource_list_head = (resource **)malloc(resource_list_object->number_of_resources * sizeof(resource*));

    for(i=0;i<number_of_resources;i++)
    {
        resource *R = (resource *)malloc(sizeof(resource));
        fscanf(resource_fd,"%d",&(R->total_instances));
        R->remaining_instances=R->total_instances;
        R->resource_number=i;
        R->currently_used_by = new_red_black_tree();
        resource_list_object->resource_list_head[i]=R;

        //fprintf(stderr,"%d\n",R->total_instances);
    }

    fclose(resource_fd);



    //creating the task set
    FILE* tasks_fd = fopen(task_set_input_file,"r");

    int number_of_tasks;
    fscanf(tasks_fd,"%d",&number_of_tasks);
    task_set *prioty_inheritance_task_set;
    prioty_inheritance_task_set = (task_set *)malloc(sizeof(task_set));
    prioty_inheritance_task_set->num_tasks=number_of_tasks;
    prioty_inheritance_task_set->list_head = (task **)malloc(prioty_inheritance_task_set->num_tasks * (sizeof(task *)));

    for(i=0;i<number_of_tasks;i++)
    {
        task *task_object = (task *)malloc(sizeof(task));
        fscanf(tasks_fd,"%d",&(task_object->phase));
        fscanf(tasks_fd,"%d",&(task_object->period));
        fscanf(tasks_fd,"%d",&(task_object->execution_time));
        fscanf(tasks_fd,"%d",&(task_object->relative_deadline));
        prioty_inheritance_task_set->list_head[i]=task_object;
    }

    int scheduling_algo=-1;
    fscanf(tasks_fd,"%d",&scheduling_algo);
    fclose(tasks_fd);

    //call the scheduler implementing priority inheritance protcol

    if(scheduling_algo==0)
    {
        scheduler_priority_ceiling(prioty_inheritance_task_set,resource_list_object,RM);
    }
    else if(scheduling_algo==1)
    {
        scheduler_priority_ceiling(prioty_inheritance_task_set,resource_list_object,EDF);
    }
    
    

    fclose(output_fd);
   
    
}

