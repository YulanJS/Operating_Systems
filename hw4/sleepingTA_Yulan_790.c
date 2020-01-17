#define MAX_SLEEP_TIME 3
#define NUM_OF_STUDENTS 4
#define NUM_OF_HELPS 2
#define NUM_OF_SEATS 2
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h> //srand time
#include <unistd.h> //sleep
#include <stdio.h>
pthread_mutex_t mutex_lock; //protects waiting_students
sem_t students_sem;
sem_t ta_sem;
int waiting_students;
void* doWork(void* arg);
void* doHelp(void* arg);
int main(int agrc, char*argv[])
{
	printf("CS149 SleepingTA from Yulan Jin\n");
	sem_init(&students_sem, 0, 0);
	sem_init(&ta_sem, 0, 0);
	//?initialized ta to be 0, because first student will wait for ta to finish his help,
	//then continue
	//???How to init students_sem...
	pthread_mutex_init(&mutex_lock, NULL);//create a mutex
	pthread_attr_t attr;
	pthread_t students[NUM_OF_STUDENTS];
	pthread_t ta;
	int threadIndex[NUM_OF_STUDENTS];
	//if I declared int j = i in the loop, gets a strange number in thread printf %d i
	//needs an address for each threadNo. void* type as an argument, needs a fixed address for each
	//int j inside loop, local variable, every new iteration, it may disappear?
	for(int i = 0; i < NUM_OF_STUDENTS; i++)
	{
		threadIndex[i] = i;
		pthread_attr_init(&attr);
		pthread_create(&students[i], &attr, doWork, &threadIndex[i]);
	}
	pthread_attr_init(&attr);
	pthread_create(&ta, &attr, doHelp, NULL);
	for(int i = 0; i < NUM_OF_STUDENTS; i++)
	{
		pthread_join(students[i], NULL);
	}
	//ta doesn't keep track of helping status, cannot terminate itself
	//???need to set up canceling point?
	pthread_cancel(ta);
	//destroy mutex and semaphore
	pthread_mutex_destroy(&mutex_lock);
	sem_destroy(&students_sem);
	sem_destroy(&ta_sem);
}

void* doWork(void* i)
{
	//time_t t;
	//initializes random number generator
	//check if the sequence is the same for all threads
	unsigned int seed;
	seed = (unsigned)(time(NULL) * pthread_self());
	//time * thread pid; maybe time is too close, same sequence
	srand(seed);
	int numOfHelpsReceived = 0;
	int tLength;
	while(numOfHelpsReceived < NUM_OF_HELPS)
	{
		tLength = rand_r(&seed) % MAX_SLEEP_TIME + 1; //between 1-3 seconds
		printf("\tStudent %d programming for %d seconds\n", *(int*)i, tLength);
		//should sleep to simulate programming
		sleep((unsigned)tLength);
		//students seek for help
		pthread_mutex_lock(&mutex_lock);
		//even if I am the first to arrive, and ta is napping, waiting_students = 1
		if(waiting_students < NUM_OF_SEATS)
		{
			waiting_students++;
			printf("\t\tStudent %d takes a seat, # of waiting students = %d\n", *(int*)i, waiting_students);
			pthread_mutex_unlock(&mutex_lock);
			sem_post(&students_sem);//signal ta that I am here
			sem_wait(&ta_sem);
			printf("Student %d receiving help\n",*(int *)i);
			numOfHelpsReceived++;
		}
		else
		{
			//if seats full, go to the next iteration
			//read before if, if goes to else, need to unlock
			pthread_mutex_unlock(&mutex_lock);
			printf("\t\t\tStudent %d will try later\n", *(int*)i);
		}
	}
	pthread_exit(0);//exit, then join
}

void* doHelp(void* arg)
{
	//???how to simulate ta napping
	//ta helping = sleep()
	//time_t t;
	unsigned int seed = (unsigned int)(time(NULL) * pthread_self());
	srand(seed);
	int tLength;
	//printf("I am the ta\n");
	while(1)
	{
		sem_wait(&students_sem);
		//printf("Students appeared\n");
		do
		{
			tLength = rand_r(&seed) % MAX_SLEEP_TIME + 1; //calculate helping time between 1-3 seconds
			pthread_mutex_lock(&mutex_lock);
			printf("Helping a student for %d seconds, # of waiting students = %d\n",tLength, --waiting_students);
			pthread_mutex_unlock(&mutex_lock);
			sem_post(&ta_sem); //tell students ta will be working
			sleep((unsigned)tLength); //helping students here
			pthread_mutex_lock(&mutex_lock);
			if(waiting_students == 0) break;
			pthread_mutex_unlock(&mutex_lock);
		}while(1);
		pthread_mutex_unlock(&mutex_lock);
		pthread_testcancel();
	}
}


