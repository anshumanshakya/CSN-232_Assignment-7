//17114013
//Anshuman Shakya

/*
the pthread flag should be used with gcc to compile this code
~$ gcc -pthread SleepingBarber.c -o SleepingBarber
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//maximum number of clients
int MAX=10;

sem_t chairs_mutex;
sem_t sem_client;
sem_t sem_barber;
int num_chairs;
int clientWait;

void *barber(void *arg) {
   int worktime;
   while(1){
      //Wait for a client to become available (sem_client).
      sem_wait(&sem_client);
      //Wait for mutex to access chair count (chair_mutex).
    sem_wait(&chairs_mutex);
      //Increment number of chairs available.
    num_chairs++;
    printf("Number of chairs available=%d\n",num_chairs);
      //Signal to client that barber is ready to cut their hair (sem_barber).
    sem_post(&sem_barber);
      //Unlock chair.
    sem_post(&chairs_mutex);
      //Generate random duration of a haricut, from 1-5 seconds.
    worktime=(rand() % 5) + 1;
    sleep(worktime);
    } 
}

void *client(void *arg) {
   int waittime;

   while(1) {
      //Wait for mutex to access chair count (chair_mutex).
    sem_wait(&chairs_mutex);

    if(num_chairs <= 0){
           //Free mutex lock on chair count.
       printf("Client %u leaving without haircut\n", (unsigned int)pthread_self());
       sem_post(&chairs_mutex);
    }
        else{
                   num_chairs -= 1;
       printf("Client %u is waiting.\n Number of chairs left= %d\n",(unsigned int)pthread_self(),num_chairs);
           //Signal that a customer is ready (sem_client).
       sem_post(&sem_client);
           //Free mutex lock on chair count.
       sem_post(&chairs_mutex);
           //Waiting for barber (sem_barber).
       sem_wait(&sem_barber);
           //Getting haircut.
       printf("Client %u is getting a haircut\n",(unsigned int)pthread_self());
    }
   /* generate random number, waittime,  Max value from command line. */
    waittime=(rand() % clientWait) + 1;
      /* sleep for waittime seconds */
    printf("Client %u is waiting\n",(unsigned int)pthread_self());
    sleep(waittime);
     }
}

int main() {
  pthread_t barberid;
  pthread_t clientids[MAX];
  

   int runTime, clients, i;
   
   runTime=20; //runtime of program
   clients=5;  // no. of clients
   num_chairs=4; // no. of chairs
   clientWait=3; // client wait time

   //Initializing semaphores.
   sem_init(&chairs_mutex,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);

   //Creating barber thread.
   pthread_create(&barberid, NULL, barber, NULL);
   
   //Creating client threads.
   for (i=0; i < clients; i++){
     pthread_create(&clientids[i], NULL, client, NULL);
   }
   
   //Sleep till job is done and then exit.
   sleep(runTime);
   exit(0);
}