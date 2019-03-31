//17114013
//Anshuman Shakya

/*
the pthread flag should be used with gcc to compile this code
~$ gcc -pthread ProducerConsumer.c -o ProducerConsumer
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>





//three semaphores
sem_t mutex;
sem_t empty;
sem_t full;


int *buffer; //buffer pointer
int buffer_pos=-1; 
int prod_no=5; //number of producers
int con_no=5;  //number of consumers
int buffer_len=10; //buffer length



pthread_t *producer_thread;
pthread_t *consumer_thread;

//producer function to produce an item
int produce(pthread_t current){
	int i = 0;
	int p = 1 + rand()%40;
	while(!pthread_equal(*(producer_thread+i),current) && i < prod_no){
		i++;
	}
	printf("\nProducer %d produced item %d \n",i+1,p);
	printf("Buffer:");
	for(i=0;i<=buffer_pos;++i)
		printf("%d ",*(buffer+i));

	return p;
}

//consumer function to consume an item
void consume(int p,pthread_t current){
	int i = 0;
	while(!pthread_equal(*(consumer_thread+i),current) && i < con_no){
		i++;
	}

	printf("\nConsumer %d consumed item %d \nNow buffer len: %d\n",i+1,p,buffer_pos);
	
}

//producers synchronization
void* producer(void *args){

	while(1){
		int p = produce(pthread_self());
		sem_wait(&empty);
		sem_wait(&mutex);
		++buffer_pos;
		*(buffer + buffer_pos) = p; 
		sem_post(&mutex);
		sem_post(&full);
		sleep(1 + rand()%3);
	}
	
	return NULL;
}

//consumers synchronization
void* consumer(void *args){
	int c;
	while(1){
		sem_wait(&full);
		sem_wait(&mutex);
		c = *(buffer+buffer_pos);
		consume(c,pthread_self());
		--buffer_pos;
		sem_post(&mutex);
		sem_post(&empty);
		sleep(1+rand()%5);
	}

	return NULL;
}

int main(){
	
	int i,thread;

	srand(time(NULL));

	sem_init(&mutex,0,1); //mutex initialized to 1
	sem_init(&full,0,0);  //full initialized to 0

	producer_thread = (pthread_t*) malloc(prod_no*sizeof(pthread_t));

	consumer_thread = (pthread_t*) malloc(con_no*sizeof(pthread_t));

	buffer = (int*) malloc(buffer_len*sizeof(int));

	sem_init(&empty,0,buffer_len); //empty initialized to buffer length

	for(i=0;i<prod_no;i++){
		thread = pthread_create(producer_thread+i,NULL,&producer,NULL);
		
	}

	for(i=0;i<con_no;i++){
		thread = pthread_create(consumer_thread+i,NULL,&consumer,NULL);
	}

	for(i=0;i<prod_no;i++){
		pthread_join(*(producer_thread+i),NULL);
	}
	for(i=0;i<con_no;i++){
		pthread_join(*(consumer_thread+i),NULL);
	}


	return 0;
}
