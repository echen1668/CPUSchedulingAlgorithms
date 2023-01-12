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
    new->name = name;//task's name
    new->priority = priority; //task's priority
    new->burst = burst; //task's burst
    insert(&head, new); //calls insert to add task into linked list
}

Task *pickNextTask() {
    struct node *next = head;
    while(next->next != NULL) { //traverse to the end of the list and takes the task at the end of the list
        next = next->next;
    }
    return next->task;
}

// invoke the scheduler
void schedule() {
    printf("This is the First-come, first-served algorithim.\n");
    printf("It takes the first task it read and runs it.\n");
    printf("The order in which the tasks are run are in the order they were presented from the file top to bottom.\n");
    printf("Step 1: Read the file.\n");
    printf("Step 2: Add each task from the file to a linked list, adding to the head (front) of the list.\n");
    printf("Each task is add in the order in which they were presented on the file top to bottom.\n");
    printf("Step 3: Call pickNextTask() which will traverse to the end of the linked list and returns the task there.\n");
    printf("Step 4: That task will run and then deleted from the linked list.\n");
    printf("Step 5: Steps 3 and 4 repeats until linked list is empty.\n");
    while(head != NULL) { //until list is empty
        Task *task = pickNextTask(); //obtains task
        run(task, task->burst); //runs task
        delete(&head, task); //deleted task from the list
    }
}
