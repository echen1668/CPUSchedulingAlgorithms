#include <string.h>
#include <stdlib.h>
#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include <stdio.h>
#include "list.h"
#include "schedulers.h"

void insertnode(struct node **head, Task *task); //adds task to the tail, back of the list
Task* copy(char *name, int priority, int burst); //copies task to a new node
Task *pickNextTask2(); //get task
int checkpriority(int priority); //check if list has a task with priority passed in this function

struct node *start = NULL; //start of inital list
struct node *curr = NULL; //points to current task
struct node *newstart = NULL; //start of new list, ordered by priority


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
    Task *highest = start->task;
    struct node *task = start;
    while(task != NULL){
        if(task->task->priority > highest->priority){ //picks task with highest priority
            highest = task->task;
        }
        task = task->next;
    }
    return highest;
}

Task *pickNextTask2() {
    struct node *next = curr;
    if(curr->next != NULL){
        curr = curr->next; //takes current task, sets curr to the next task on the list
    }else{
        curr = newstart; //if curr is at the end of the list, reset curr to the beginning of the list
    }
    return next->task;
}

Task* copy(char *name, int priority, int burst) {
    Task *new = malloc(sizeof(Task)); //copies tasks to a new allocated task
    new->name = name; //task's name
    // priority and burst
    new->priority = priority;
    new->burst = burst;
    // insert into task list
    return new;
}

int checkpriority(int priority){
    int exist = 0;
    struct node *current = newstart;
    while(current != NULL){ //traverses through list and find of there is task with the desired priority
        if(current->task->priority == priority){
            exist = 1; //exist if 1 if task with the desired priority is found, else 0
        }
        current = current->next;
    }
    return exist;
}

// invoke the scheduler
void schedule() {
    printf("This is the Priority Round-robin algorithim.\n");
    printf("It runs each task on the list for a time quantum.\n");
    printf("It will run one task for that time quantum and then process to the next task to run that.\n");
    printf("Each time a task is run, it CPU burst is subtracted by the time quantum.\n");
    printf("If a task's CPU burst is less than the time quantum, then it runs with its remainder CPU burst and then deletes from linked list.\n");
    printf("It will keep running each task repeatly in the same order until all of them have no CPU burst left thus deleted.\n");
    printf("Step 1: Read the file.\n");
    printf("Step 2: Add each task from the file to a linked list, adding from the tail (back) of the list.\n");
    printf("Each task is add in the order in which they were presented on the file top to bottom.\n");
    printf("Step 3: The linked list is reordered from highest priority to the lowest priority front to back.\n");
    printf("Step 4: Will traverse from highest priority to the 0 by a for loop.\n");
    printf("Step 5: Call pickNextTask() which will traverse to the end of the linked list and returns the task there.\n");
    printf("It has a variable pointer, 'curr' which will point to the next task on the linked list.\n");
    printf("Step 6: If the task is equal to the priority, then it will run that task and subtract is CPU burst by the time quantum( or all of its CPU burst if it is less than quantum).\n");
    printf("Step 7: After each task of that priority is run, the program will go back to the beginning of the list the run them again.\n");
    printf("It will keep doing so until the tasks with the priority has no CPU burst left in which then the task is deleted.\n");
    printf("Step 8: The for loop will move to the next priority and repeats steps 5-7.\n");
    int highestpriority = pickNextTask()->priority;
    while (start != NULL) {
        Task *newtask = pickNextTask(); //obtains task with highest priority
        insertnode(&newstart, copy(newtask->name, newtask->priority, newtask->burst)); //copies task to new linked list
        delete(&start, newtask); //deleted task from the list //deletes task from old linked list
    }
    curr = newstart; //curr points to head of new linked list which ordered by priority
    for(int i = highestpriority; i >= 0; i--){
        int priorityexists = checkpriority(i);
        while (priorityexists == 1) {
            Task *task = pickNextTask2();
            if(task->priority == i){ //runs task with current priority if exists
                int slice = 0;
                if (task->burst > QUANTUM) { //time slice is the time quantum if CPU burst is greater than that.
                    slice = QUANTUM;
                } else {
                    slice = task->burst; //else time slice is remainder of the CPU burst
                }
                run(task, slice); //runs task by time slice
                task->burst -= slice; //subtract CPU burst from either quantum or remainder of the CPU burst
                if (task->burst <= 0) {
                    delete(&newstart, task); //deleted task from the list
                }
            }
            priorityexists = checkpriority(i);
        }
        curr = newstart;
        
    }
}
