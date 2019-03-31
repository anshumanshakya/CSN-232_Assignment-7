//17114013
//Anshuman Shakya

/*
the pthread flag should be used with gcc to compile this code
~$ gcc -pthread ReaderWriter.c -o ReaderWriter
*/

#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>


/*number of readers */
int ReaderNo=5;  

/* define semaphore object */
sem_t obj;        
/* number of readers reading*/
int readCount = 0;     
int writeCount = 0;
int requestWrite = 0;
 
 
//reader function
void* reader(void*arg)          /* readers function to read */
{
    int i = *(int*)arg;
    int can_read;
    while(1){
        can_read = 1;
 
        sem_wait(&obj);
        if(writeCount == 0 && requestWrite == 0)  readCount++;
        else                               can_read = 0;
        sem_post(&obj);
 
        if(can_read) {
            
            printf("Reader %d is reading\n", i);
            sleep(2);
 
            sem_wait(&obj);
            readCount--;
            sem_post(&obj);
        }
 
        sched_yield();
    }
    return 0;
}
;

//writer function
void* writer(void*arg)      
{   
    // writer can now write
    int wrt; 
    while(1){
        wrt = 1;
 
        sem_wait (&obj);
        if(readCount == 0)  
            writeCount++;
        else       
        { 
            wrt = 0; 
            requestWrite = 1; 
        }
        sem_post(&obj);
 
        if(wrt) {
           
            printf("Writer is writing with %d readers reading \n",readCount);
 
            sleep(1);
 
            sem_wait(&obj);
            writeCount--;
            requestWrite = 0;
            sem_post(&obj);
        }
 
        sched_yield();
    }
    return 0;
}



int main()
{
    pthread_t readers[ReaderNo],writerThead;
    int i;
    int ids[ReaderNo]; 
    sem_init (&obj,0,1);
    for(i = 0; i < ReaderNo; i ++)  
    {
        ids[i]=i+1;
        /* create reader thread and check if error */
        if(pthread_create(&readers[i],0,reader,&ids[i])!=0)
        {
            perror("error while creating reader thread");                         
        }
    }
     /* create writer thread and check if error */
    if(pthread_create(&writerThead,0,writer,0)!=0){
        perror("error while creating writer thread");    
    }
 
    pthread_join(writerThead,0);   
    sem_destroy (&obj);
    return 0;
}