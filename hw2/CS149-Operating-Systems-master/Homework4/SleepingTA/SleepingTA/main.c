//  main.c
//  SleepingTA
//
//  Created by Craig on 10/26/17.
//  Copyright © 2017 Craig. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/* the maximum time (in seconds) to sleep */
#define MAX_SLEEP_TIME 3
/* number of potential students */
#define NUM_OF_STUDENTS 4
#define NUM_OF_HELPS 2
/* number of available seats */
#define NUM_OF_SEATS 2

/* mutex declarations, a global variable */
pthread_mutex_t mutex_lock; /* protect the global variable waiting_student */

/* semaphore declarations, global variables */
sem_t students_sem; /* ta waits for a student to show up, student notifies ta his/her arrival*/
sem_t ta_sem;/* student waits for ta to help, ta notifies student he/she is ready to help*/

/* the number of waiting students, a global variable */
int waiting_students = 0;
int seats[NUM_OF_SEATS];
int next_seat = 0;
int next_help = 0;
int timer;

void rand_sleep(void);
void* student_programming(void *id);
void* ta_working(void);

int main(int argc, char **argv)
{
    printf("%s", "CS149 SleepingTA from Craig Huff\n");
    
    //threads
    pthread_t *students;
    pthread_t ta;
    
    //initialization
    int* student_ids;
    int i;
    
    students = (pthread_t*)malloc(sizeof(pthread_t) * NUM_OF_STUDENTS);
    student_ids = (int*)malloc(sizeof(int) * NUM_OF_STUDENTS);
    
    sem_init(&students_sem, 0, 0);
    sem_init(&ta_sem, 0, 1);
    
    //set random
    srand(time(NULL));
    
    pthread_mutex_init(&mutex_lock, NULL);
    
    //create TA thread
    pthread_create(&ta, NULL, ta_working, NULL);
    
    //create student threads
    for (i = 0; i < NUM_OF_STUDENTS; i++) {
        student_ids[i] = i;
        pthread_create(&students[i], NULL, student_programming, (void*) &student_ids[i]);
    }
    
    //when threads are done running, join them
    pthread_join(ta, NULL);
    
    for (i = 0; i < NUM_OF_STUDENTS; i++)
    {
        pthread_join(students[i], NULL);
    }
    
    return 0;
}

void* student_programming(void* student_id)
{
    int id = *(int*)student_id;
    printf("Student %d is programming for %d seconds\n", id, timer);
    int times_helped = 0;
    
    while (1)
    {
        rand_sleep();
        
        //acquire the mutex lock so the global variables can't be chganged by other threads
        pthread_mutex_lock(&mutex_lock);
        if (waiting_students < NUM_OF_SEATS)
        {
            seats[next_seat] = id;
            waiting_students++;
            printf("Student %d takes a seat. Number of students waiting = %d\n", id, waiting_students );
            next_seat = (next_seat+1) % NUM_OF_SEATS;
            times_helped++;
            
            //let go of the lock once they have been added to the seats queue
            pthread_mutex_unlock(&mutex_lock);
            sem_post(&students_sem);
            sem_wait(&ta_sem);
        }
        else
        {
            //if the seats are all full, release the lock and try again later
            pthread_mutex_unlock(&mutex_lock);
            printf("\t\t\tStudent %d will try later\n\tStudent %d is programming for %d seconds \n",id,id, timer);
        }
        
        //the thread will end its process if it's been helped two times
        if (times_helped == NUM_OF_HELPS)
            pthread_cancel(pthread_self());
    }
}

void* ta_working()
{
    //used to make sure all students have been helped 2 times
    int counter = 0;
    
    while (1) {
        sem_wait(&students_sem);
        
        //acuqire the mutex lock to be able to removed values from the array
        pthread_mutex_lock(&mutex_lock);
        if (waiting_students == 0)
        {
            rand_sleep(); //this is put there too prevent the while loop from completing before "students" have the chance to get help from the TA
            counter++;
        }
        else
        {
            printf("\tHelping Student %d for %d seconds\n", seats[next_help], timer);
            seats[next_help] = 0;
            waiting_students--;
            
            //            printf("waiting students : [1] %d [2] %d\n",seats[0],seats[1]);
            next_help = (next_help + 1) % NUM_OF_SEATS;
            
            counter--;
        }
        
        //release when done editing variables
        pthread_mutex_unlock(&mutex_lock);
        
        //let the resource become availible again
        sem_post(&ta_sem);
        
        //when all studentes have been helped twice, the TA will go to sleep and end its thread.
        if (counter == 5) {
            printf("Number of students waiting = %d\n", waiting_students);
            printf("The TA is sleeping for the rest of the day. All students have been helped.\n");
            pthread_cancel(pthread_self());
        }
    }
}

void rand_sleep(void){
    timer = rand() % MAX_SLEEP_TIME + 1;
    sleep(timer);
}

