//17114013
//Anshuman Shakya

/*
the pthread flag should be used with gcc to compile this code
~$ gcc -pthread CigratteSmoker.c -o CigratteSmoker
*/


#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//number of smokers
int SMOKERS=3;

//three smokers semaphores one agent semaphore
sem_t sem_Tobacco, sem_Paper, sem_Match;
sem_t sem_Agent;

// Bools for items
int Tobacco = 0;
int Paper = 0;
int Match = 0;


void *smoker(void * arg){
  int id = *(int*)arg; // Convert argument to int

  while(1){
    
      if(id==0){ // Tobacco
        sem_wait(&sem_Tobacco);
        if(Paper > 0 && Match > 0){    //Check if Items are avalible
          Paper = 0;
          Match = 0;
          printf("Smoker with Tobacco smokes\n");
          sem_post(&sem_Agent);
        }
        else printf("Error: Paper or match is not avalible!\n");
      }
      else if(id==1){ // Paper
        sem_wait(&sem_Paper);
        if(Tobacco > 0 && Match > 0){  //Check if Items are avalible
          Tobacco = 0;
          Match = 0;
          printf("Smoker with Paper smokes\n");
          sem_post(&sem_Agent);
        }
        else printf("Error: Tobacco or match is not avalible!\n");
      }
      else if(id==2){ // Match
        sem_wait(&sem_Match);
        if(Tobacco > 0 && Paper > 0){  //Check if Items are avalible
          Tobacco = 0;
          Paper = 0;
          printf("Smoker with Match smokes\n");
          sem_post(&sem_Agent);
        }
        else printf("Error: Tobacco or paper is not avalible!\n");
      }
      
    }
}

/*
 * Agent sets a random Item
 */
void *agent(void * arg)
{
  while(1){
    sem_wait(&sem_Agent);  // Wait for smoker

    usleep(1000000);  // Delay to slow down process

    int id = rand() % 3;  // Generate a random ID for a random item

    
      if(id==0){ // Tobacco
        printf("Agent provides paper and match\n");
        Paper++;
        Match++;
        sem_post(&sem_Tobacco);
      }
      else if(id==1){ // Paper
        printf("Agent provides tobacco and match\n");
        Tobacco++;
        Match++;
        sem_post(&sem_Paper);
      }
      else if(id==2){ // Match
        printf("Agent gives tobacco and paper\n");
        Tobacco++;
        Paper++;
        sem_post(&sem_Match);
      }
  }
}

int main()
{
  // Initialize semaphores
  sem_init(&sem_Tobacco, 0, 0);
  sem_init(&sem_Paper, 0, 0);
  sem_init(&sem_Match, 0, 0);

  sem_init(&sem_Agent, 0, 1);

  // Define threads
  pthread_t tSmokers[SMOKERS], tAgent;

  int smokersID[SMOKERS];

  // Create smokers 
  for(int i = 0; i < SMOKERS; i++)
  {
    smokersID[i] = i;
    pthread_create(&tSmokers[i], NULL, smoker, &smokersID[i]);
  }
  
  // Craete agent 
  pthread_create(&tAgent, NULL, agent, NULL);

  // Wait for smokers
  for (int i = 0; i < SMOKERS; i++)
    pthread_join(tSmokers[i], NULL);

  // Wait for agent
  pthread_join(tAgent, NULL);

  exit(0);
}
