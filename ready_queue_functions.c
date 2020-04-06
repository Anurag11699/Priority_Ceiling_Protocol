#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include "functions.h"

tree_node* new_tree_node(job* job_object) 
{
    tree_node* n = malloc(sizeof(tree_node));
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    n->job_object = job_object;
    n->color = Red;

    return n;

}

red_black_tree* new_red_black_tree() 
{
    red_black_tree *t = malloc(sizeof(red_black_tree));
    tree_node *nil_node = malloc(sizeof(tree_node));
    nil_node->left = NULL;
    nil_node->right = NULL;
    nil_node->parent = NULL;
    nil_node->color = Black;
    nil_node->job_object = NULL;
    t->NIL = nil_node;
    t->root = t->NIL;

    return t;
}

void left_rotate(red_black_tree *t, tree_node *x) 
{
    tree_node *y = x->right;
    x->right = y->left;
    if(y->left != t->NIL) 
    {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if(x->parent == t->NIL) 
    { //x is root
        t->root = y;
    }

    else if(x == x->parent->left) 
    { //x is left child
        x->parent->left = y;
    }
    else 
    { //x is right child
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void right_rotate(red_black_tree *t, tree_node *x) 
{
    tree_node *y = x->left;
    x->left = y->right;

    if(y->right != t->NIL) 
    {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if(x->parent == t->NIL) 
    { //x is root
        t->root = y;
    }
    else if(x == x->parent->right) 
    { //x is left child
        x->parent->right = y;
    }
    else 
    { //x is right child
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

void insertion_fixup(red_black_tree *t, tree_node *z) 
{
    while(z->parent->color == Red) 
    {
        if(z->parent == z->parent->parent->left) 
        { //z.parent is the left child

            tree_node *y = z->parent->parent->right; //uncle of z

            if(y->color == Red) 
            { //case 1
                z->parent->color = Black;
                y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            }
            else 
            { //case2 or case3
                if(z == z->parent->right) 
                { //case2
                    z = z->parent; //marked z.parent as new z
                    left_rotate(t, z);
                }
                //case3
                z->parent->color = Black; //made parent black
                z->parent->parent->color = Red; //made parent red
                right_rotate(t, z->parent->parent);
            }
        }
        else 
        { //z.parent is the right child
            tree_node *y = z->parent->parent->left; //uncle of z

            if(y->color == Red) 
            {
                z->parent->color = Black;
                y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            }
            else 
            {
                if(z == z->parent->left) 
                {
                    z = z->parent; //marked z.parent as new z
                    right_rotate(t, z);
                }
                z->parent->color = Black; //made parent black
                z->parent->parent->color = Red; //made parent red
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = Black;
}

void insert(red_black_tree *t, job* job_object) 
{

    tree_node* z = new_tree_node(job_object);
    tree_node* y = t->NIL; //variable for the parent of the added node
    tree_node* temp = t->root;

    while(temp != t->NIL) 
    {
        y = temp;
        if(z->job_object->current_priority < temp->job_object->current_priority)
            temp = temp->left;
        else
            temp = temp->right;
    }
    z->parent = y;

    if(y == t->NIL) 
    { //newly added node is root
        t->root = z;
    }
    else if(z->job_object->current_priority < y->job_object->current_priority) //data of child is less than its parent, left child
        y->left = z;
    else
        y->right = z;

    z->right = t->NIL;
    z->left = t->NIL;

    insertion_fixup(t, z);
}

void rb_transplant(red_black_tree *t, tree_node *u, tree_node *v) 
{
    if(u->parent == t->NIL)
        t->root = v;
    else if(u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

tree_node* minimum(red_black_tree *t, tree_node *x) 
{
    while(x->left != t->NIL)
        x = x->left;
    return x;
}

void rb_delete_fixup(red_black_tree *t, tree_node *x) 
{
    while(x != t->root && x->color == Black) 
    {
        if(x == x->parent->left) 
        {
            tree_node *w = x->parent->right;
            if(w->color == Red) 
            {
                w->color = Black;
                x->parent->color = Red;
                left_rotate(t, x->parent);
                w = x->parent->right;
            }
            if(w->left->color == Black && w->right->color == Black) 
            {
                w->color = Red;
                x = x->parent;
            }
            else 
            {
                if(w->right->color == Black) 
                {
                    w->left->color = Black;
                    w->color = Red;
                    right_rotate(t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = Black;
                w->right->color = Black;
                left_rotate(t, x->parent);
                x = t->root;
            }
        }
        else 
        {
            tree_node *w = x->parent->left;
            if(w->color == Red) 
            {
                w->color = Black;
                x->parent->color = Red;
                right_rotate(t, x->parent);
                w = x->parent->left;
            }
            if(w->right->color == Black && w->left->color == Black) 
            {
                w->color = Red;
                x = x->parent;
            }
            else 
            {
                if(w->left->color == Black) 
                {
                    w->right->color = Black;
                    w->color = Red;
                    left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = Black;
                w->left->color = Black;
                right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = Black;
}

void rb_delete(red_black_tree *t, tree_node *z) 
{
    tree_node *y = z;
    tree_node *x;
    enum COLOR y_orignal_color = y->color;

    if(z->left == t->NIL) 
    {
        x = z->right;
        rb_transplant(t, z, z->right);
    }
    else if(z->right == t->NIL) 
    {
        x = z->left;
        rb_transplant(t, z, z->left);
    }
    else 
    {
        y = minimum(t, z->right);
        y_orignal_color = y->color;
        x = y->right;
        if(y->parent == z) 
        {
            x->parent = z;
        }
        else 
        {
            rb_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rb_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(y_orignal_color == Black)
    {
        rb_delete_fixup(t, x);
    }
        
}

void inorder(red_black_tree *t, tree_node *n) 
{
    if(n != t->NIL) 
    {
        inorder(t, n->left);
        fprintf(output_fd,"%d\t", n->job_object->current_priority);
        fflush(output_fd);
        inorder(t, n->right);
    }
    else
    {
        fprintf(output_fd,"\n");
    }
    
}


//This function is used to remove the job with the highest priority from the Red Black Tree
job* RB_tree_remove_min(red_black_tree * rb_tree_object)
{
    
    tree_node *min = minimum(rb_tree_object,rb_tree_object->root);
    

    rb_delete(rb_tree_object,min);
    return min->job_object;
}

//This function is used to get the job with the highest priority from the Red Black Tree
job* RB_tree_get_min(red_black_tree * rb_tree_object)
{
    
    tree_node *min = minimum(rb_tree_object,rb_tree_object->root);
    
    return min->job_object;
}

//This function is used to check if the ready queue is empty or no
int is_ready_queue_empty(red_black_tree * rb_tree_object)
{
    if(rb_tree_object->root==rb_tree_object->NIL)
    {
        return 1;
    }
    return 0;
}

/*
PreConditions
Inputs: {pointer to red black tree, pointer to job to search}

Purpose of the Function: To search for a job in the red black tree
PostConditions
Returns pointer to node if found, else returns pointer to NIL node
*/
tree_node* rb_tree_search(red_black_tree* rb_tree_object,job* job_object)
{
    tree_node *x;

    x = rb_tree_object->root;
    while(x != rb_tree_object->NIL && x->job_object != job_object)
    {
        if(job_object->current_priority < x->job_object->current_priority)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    return x;
}

/*
PreConditions
Inputs: {pointer to ready queue, pointer to resource list object, new priority for job}

Purpose of the Function: This function is used to update the priority of the job given and update the Red Black trees of the resources that hold this job. This is called when a job is blocked by a lower priority job holding a resource that it requires. Hence the Red Black tree for both the ready queue and for all the resources that are holding this job must be updated

PostConditions
Updated priority of job and its respective priority queues
*/
void update_job_priority(red_black_tree* ready_queue,resource_list* resource_list_object,job* job_object,int new_priority)
{
    
    //searching for job in all Trees
    tree_node * needed_node_resource_queue[resource_list_object->number_of_resources];
    tree_node *needed_node_ready_queue = rb_tree_search(ready_queue,job_object);


   

    for(int i=0;i<resource_list_object->number_of_resources;i++)
    {
        needed_node_resource_queue[i] = rb_tree_search(resource_list_object->resource_list_head[i]->currently_used_by, job_object);
    }

    //deleting jobs from all trees if present 

    rb_delete(ready_queue,needed_node_ready_queue);

    for(int i=0;i<resource_list_object->number_of_resources;i++)
    {
        if(needed_node_resource_queue[i] != resource_list_object->resource_list_head[i]->currently_used_by->NIL)
        {
            rb_delete(resource_list_object->resource_list_head[i]->currently_used_by,needed_node_resource_queue[i]);
        }
    }

    

    //changing job priority
    needed_node_ready_queue->job_object->current_priority=new_priority;

    //inserting into the trees
    insert(ready_queue,job_object);
    for(int i=0;i<resource_list_object->number_of_resources;i++)
    {
        if(needed_node_resource_queue[i] != resource_list_object->resource_list_head[i]->currently_used_by->NIL)
        {
            insert(resource_list_object->resource_list_head[i]->currently_used_by,job_object);
        }
    }

}


void rb_delete_job(red_black_tree* rb_tree_object,job* job_object)
{
    tree_node *needed_node = rb_tree_search(rb_tree_object,job_object);

    rb_delete(rb_tree_object,needed_node);
}