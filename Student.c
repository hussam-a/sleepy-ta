
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

extern pthread_mutex_t printf_lock;
extern pthread_mutex_t setup_lock;
extern pthread_mutex_t chairs_lock;
extern sem_t TA_ready;
extern sem_t TA_done;
extern sem_t Student_register;
extern int setupNotDone;
extern int globalSeed;
extern int occupied_chairs;
extern int MAX_CHAIRS;
extern int chairs[];
extern int front;
extern int rear;
int generateSomeTime(int, unsigned int*);

void* Student(void* param)
{
    unsigned int seed = globalSeed;	//Get unique seed
    globalSeed++;
    int SID = *((int*) param);	//Setup the student's SID
    printf("SID:%d summoned.\n", SID);
    pthread_mutex_unlock(&setup_lock);	//Lock related to setuping
    
    while(1)
    {
        int time = generateSomeTime(1, &seed);	//Generate random time period
        printf("SID:%d will go program for %d seconds.\n", SID, time);
        sleep(time);	//Sleep here indicates programming time the student uses
        printf("SID:%d inteds to go to TA\n", SID);	//Student done with programming, inteds to go to TA
        
        //Lock access to chairs
        pthread_mutex_lock(&chairs_lock);
        if (occupied_chairs<MAX_CHAIRS)	//Check is atleast one chair is free
        {
            occupied_chairs++;	//If so update chairs and sit there
            rear++;
            rear = rear % MAX_CHAIRS;
            chairs[rear] = SID;	//Insert SID into queue
            printf("SID:%d waiting for TA\n", SID);
			sem_post(&Student_register);	//Inform TA student is waiting
            pthread_mutex_unlock(&chairs_lock);	//Release lock on chairs            
            sem_wait(&TA_ready);	//Wait till TA is done with previous student
			sem_wait(&TA_done);	//Wait till TA is done with current student            
        }
        else
        {
            pthread_mutex_unlock(&chairs_lock);	//If not chair found , return to programming (in next loop)
            printf("SID:%d did not find chairs.\n", SID);
        }
    }
    
    pthread_exit(0);
}
