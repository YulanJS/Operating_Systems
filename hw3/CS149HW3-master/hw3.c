//To compile: gcc -o h3 hw3.c -pthread
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <errno.h>
/*the maxmimum time(in seconds) to sleep*/
#define MAX_SLEEP_TIME 3
/*the number of potential student*/
#define NUM_OF_STUDENTS 4
/*number of avaiable seats*/
#define NUM_OF_SEATS 2
/*function declarations*/
void programming();
void helping_student();
void* ta_thread(void* args);
void* student_thread(void* args);

/*semaphore declarations*/
sem_t students_sem;//TA waits for a student to show up, student notidies TA
sem_t ta_sem;//student waits for TA to help,TA notifies student TA is ready to help
/*mutex_lock*/
pthread_mutex_t mutex_lock;
/*the number of waiting students (shared variable)*/
int waiting_students;
/*random number*/
int seed = MAX_SLEEP_TIME;
void* ta_thread(void* args)
{
	while(1)
	{
		//waitting for student
		sem_wait(&students_sem);
		//printf("==after waiting for students_sem\n");

		//helps student who wake up TA
		sem_post(&ta_sem);

		//printf("==after waiting for ta_sem \n");
		//Now helping
		helping_student(); 

		//Complete helping, so check if waitting student
		while(waiting_students > 0)
		{
			sem_post(&ta_sem);
			helping_student();
		}
		printf("ta is now sleeping\n");

	}//while
}//ta_thread
void* student_thread(void* args)
{
	//pthread_t th_id = pthread_self(); 
	//printf("th_id is:" + th_id);
	int num_helps = 0;
	int try_wait = 0;
	while(num_helps <= 2)
	{
		printf("programming\n");
		programming();


		//printf("============signal:students_sem===========\n");
		sem_post(&students_sem);
		//printf("============wait:ta_sem===========\n");
		try_wait = sem_trywait(&ta_sem);
		if (try_wait == EAGAIN)
		{
			if (waiting_students <= 2)
			{
				pthread_mutex_lock(&mutex_lock);
				waiting_students++;
				pthread_mutex_unlock(&mutex_lock);

				sem_wait(&ta_sem);
				/*suceeded to get help, so declement waiting_student*/
				pthread_mutex_lock(&mutex_lock);
				waiting_students--;
				pthread_mutex_unlock(&mutex_lock);
				num_helps++;
			}
			else
			{
				//there is no empty seat
				//go back to programming
			}
		}
		else
		{
			//get help soon.
			num_helps++;
		}
	}
}
void helping_student()
{	

	printf("TA is helping a student\n");
	sleep(rand_r(&seed)%3+1);
}
void programming()
{
	printf("programming \n");
	sleep(rand_r(&seed)%3+1);
}
int main(int argc, char const *argv[])
{
	printf("Kaya Ota from CS149\n");
  	pthread_t th_s1, th_s2,th_s3,th_s4;
  	pthread_t th_ta;
	

	waiting_students = 0; 
	/*generate seed of random number*/
	srand(3);


	
	pthread_mutex_init(&mutex_lock, NULL);
	sem_init(&students_sem,0,0);
	sem_init(&ta_sem, 0, 0);

	pthread_create(&th_ta, NULL, ta_thread, NULL);

	pthread_create(&th_s1, NULL, student_thread, NULL);
	pthread_create(&th_s2, NULL, student_thread, NULL);
	pthread_create(&th_s3, NULL, student_thread, NULL);
	pthread_create(&th_s4, NULL, student_thread, NULL);

	pthread_join(th_s1, NULL);//wait for ta_thread complete 
	pthread_join(th_s2, NULL);
	pthread_join(th_s3, NULL);
	pthread_join(th_s4, NULL);
	pthread_cancel(th_ta);





	

 	return 0;
}






