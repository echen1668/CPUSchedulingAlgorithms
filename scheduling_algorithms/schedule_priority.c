#include <string.h>
#include <stdlib.h>
#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include <stdio.h>
#include "list.h"
#include "schedulers.h"

struct node *head = NULL;

// add task read from the file to the linked list from the head (front)
void add(char *name, int priority, int burst) {
    Task *new = malloc(sizeof(Task));
    new->name = name; //task's name
    new->priority = priority; //task's priority
    new->burst = burst; //task's burst
    insert(&head, new); //calls insert to add task into linked list
}

Task *pickNextTask() {
    Task *highest = head->task;
    struct node *next = head;
    while(next != NULL){ //traverse through list, take task with the highest priority
        if(next->task->priority >= highest->priority){
            highest = next->task;
        }
        next = next->next;
    }
    return highest;
}

// invoke the scheduler
void schedule() {
    printf("This is the Priority algorithim.\n");
    printf("It runs each task on the list in an order starting from the highest priority to the lowest priority.\n");
    printf("Step 1: Read the file.\n");
    printf("Step 2: Add each task from the file to a linked list, adding from the head (front) of the list.\n");
    printf("Each task is add in the order in which they were presented on the file top to bottom.\n");
    printf("Step 3: Call pickNextTask() which will traverse across the entire linked list and find the task with the highest priority.\n");
    printf("Step 4: That task will run and then deleted from the linked list.\n");
    printf("Step 5: Steps 3 and 4 repeats until linked list is empty.\n");
    while (head != NULL) { //until list is empty
        Task *task = pickNextTask(); //obtains task
        run(task, task->burst); //runs task
        delete(&head, task); //deleted task from the list
    }
}
