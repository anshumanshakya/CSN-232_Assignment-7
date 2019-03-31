//17114013
//Anshuman Shakya

/*
the pthread flag should be used with gcc to compile this code
~$ gcc -pthread DiningPhilosopher.c -o DiningPhilosopher
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef DININGPHILOSOPHERS  
#define DININGPHILOSOPHERS

enum State {
    EATING,
    THINKING,
    HUNGRY
};

#endif

int numofphil = 0;
int *states;
sem_t *forks;

char getStateName(int state){
    switch(state){
        case EATING: return 'E'; break;
        case THINKING: return 'T'; break;
        case HUNGRY: return 'H'; break;
    }
}

void* philosopher (void* arg)
{
    
    int n =*((int *)arg);
    int left = n;
    int right = (n+1)%numofphil;
    while(1)
    {       
            
        /*get the forks */ 
        sleep(rand()%10+1);
              
        states[n] = HUNGRY;     
        if (n==0)
        {
            sem_wait(&forks[left]);
            sem_wait(&forks[right]);
            states[n] = EATING;
        }else
        {
            sem_wait(&forks[right]);
            sem_wait(&forks[left]);
            states[n] = EATING;
        }

        sleep(rand()%10+1);

        /*drop the forks */
        sem_post(&forks[left]);
        sem_post(&forks[right]);
        states[n] = THINKING;
        
    }   
}


int main() {

    numofphil=5;

    int i;
    states = malloc(sizeof(int)*numofphil);   
    
    for (i=0; i<numofphil; i++){
        states[i] = THINKING;
    }
    
    forks = malloc(sizeof(sem_t)*numofphil);
    
    pthread_t philosophers[numofphil];
    
    for(i=0; i<numofphil;i++){       
        //initialize semaphores for the forks all in 1
            sem_init(&forks[i], 0, 1);
            int *arg= malloc (sizeof(int));
            *arg = i;       
            //creates a thread for each philosopher
            pthread_create(&philosophers[i], NULL, philosopher, (void *)arg);
    }

    char state;
    int j;
    while(1){
        sleep(1);
        for (j=0; j<numofphil; j++){
            state = getStateName(states[j]);
            if(state=='E') printf("Eating - ");
            else if(state=='T') printf("Thinking - ");
            if(state=='H') printf("Hungry - ");
        }
        printf("\n");
    }
    pthread_exit(0); 
}