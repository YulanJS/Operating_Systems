/*to compile:  gcc -o th_c thread_counter.c -pthread

  to run: ./th_c
*/

#include <stdio.h>
#include <unistd.h>//sleep()
#include <pthread.h>

//thread 
void* thread(void* args)
{
  int counter = 0;
  while(1)
  {
    printf("thread %d\n", counter);
    sleep(1);//stop thread for 1 sec
    counter++;
  }
  return NULL;
}

//main thread 
int main(int argc, char const *argv[])
{
  pthread_t th;
  pthread_create(&th, NULL, thread, (void *)NULL);
  //wait for typed key
  getchar();
  return 0;
}
