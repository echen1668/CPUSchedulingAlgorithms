#include <string.h>
#include <stdlib.h>
#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include <stdio.h>
#include "list.h"
#include "schedulers.h"
void insertnode(struct node **head, Task *task); //adds node with task to the tail/end of the list

struct node *start = NULL; //points to head of the list
struct node *curr = NULL; //points to current task

// adds new task to the end/tail of the linked list
void insertnode(struct node **head, Task *newTask) {
    //create new node
    struct node *newnode = malloc(sizeof(struct node));
    newnode->task = newTask;
    newnode->next = NULL;
    if(*head == NULL){
        *head = newnode; //if list is empty, make new node the new head
    }else{
        struct node *current = *head;
        while(current->next != NULL){
            current = current->next; //traverse to the end of the list and adds new node
        }
        current->next = newnode;
    }
}

// add task read from the file to the linked list from the head (front)
void add(char *name, int priority, int burst) {
    Task *new = malloc(sizeof(Task));
    new->name = name; //task's name
    new->priority = priority; //task's priority
    new->burst = burst; //task's burst
    insertnode(&start, new); //calls insert to add task into linked list
}

Task *pickNextTask() {
    struct node *next = curr;
    if(curr->next != NULL){ //takes current task, sets curr to the next task on the list
        curr = curr->next;
    }else{
        curr = start; //if curr is at the end of the list, reset curr to the beginning of the list
    }
    return next->task;
}

// invoke the scheduler
void schedule() {
    printf("This is the Round-robin algorithim.\n");
    printf("It runs each task on the list for a time quantum.\n");
    printf("It will run one tasks for that time quantum and then process to the next task to run that.\n");
    printf("Each time a task is run, it CPU burst is subtracted by the time quantum.\n");
    printf("If a task's CPU burst is less than the time quantum, then it runs with its remainder CPU burst and then deletes from linked list.\n");
    printf("Step 1: Read the file.\n");
    printf("Step 2: Add each task from the file to a linked list, adding from the tail (back) of the list.\n");
    printf("Each task is add in the order in which they were presented on the file top to bottom.\n");
    printf("Step 3: Call pickNextTask() which will traverse to the end of the linked list and returns the task there.\n");
    printf("It has a variable pointer, 'curr' which will point to the next task on the linked list.\n");
    printf("Step 4: That task will run and have its CPU burst subtracted by the time quantum (or all of its CPU burst if it is less than quantum.\n");
    printf("Step 5: After each task is run, the program will go back to the beginning of the list the run them again.\n");
    printf("It will keep doing so until the task has no CPU burst left in which then the task is deleted.\n");
    printf("Step 6: Steps 3, 4, and 5 repeats until linked list is empty.\n");
    curr = start; //curr points at head of the list
    while (start != NULL) { //until list is empty
        Task *task = pickNextTask(); //obtains task
        int slice = 0;
        if(task->burst > QUANTUM){ //time slice is the time quantum if CPU burst is greater than that.
            slice = QUANTUM;
        }else{
            slice = task->burst; //else time slice is remainder of the CPU burst
        }
        run(task, slice); //runs task by time slice
        task->burst -= slice; //subtract CPU burst from either quantum or remainder of the CPU burst
        if(task->burst <= 0) {
            delete(&start, task); //deleted task from the list
        }
    }
}
