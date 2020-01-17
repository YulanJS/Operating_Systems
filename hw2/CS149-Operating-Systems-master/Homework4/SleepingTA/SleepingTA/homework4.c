/* the maximum time (in seconds) to sleep */
#define MAX_SLEEP_TIME 3
/* number of potential students */
#define NUM_OF_STUDENTS 4
#define NUM_OF_HELPS 2
/* number of available seats */
#define NUM_OF_SEATS 2

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

/* mutex declarations, a global variable */
pthread_mutex_t mutex_lock; /* protect the global variable waiting_student */
/* semaphore declarations, global variables */
sem_t students_sem; /* ta waits for a student to show up, student notifies ta his/her arrival*/
sem_t ta_sem;/* student waits for ta to help, ta notifies student he/she is ready to help*/
/* the number of waiting students, a global variable */
int waiting_students;


int main()
{
    printf("%s", "CS149 SleepingTA from Craig Huff ");
}