
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

extern pthread_mutex_t printf_lock;
extern pthread_mutex_t setup_lock;
extern pthread_mutex_t chairs_lock;
extern sem_t TA_done;
extern sem_t Student_waiting;
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
        pthread_mutex_lock(&printf_lock);
        printf("SID:%d will go program for %d seconds.\n", SID, time);
        pthread_mutex_unlock(&printf_lock);
        sleep(time);	//Sleep here indicates programming time the student uses
        pthread_mutex_lock(&printf_lock);
        printf("SID:%d will go to TA\n", SID);	//Student done with programming, inteds to go to TA
        pthread_mutex_unlock(&printf_lock);
        
        //Lock access to chairs
        pthread_mutex_lock(&chairs_lock);
        if (occupied_chairs<MAX_CHAIRS)	//Check is atleast one chair is free
        {
            occupied_chairs++;	//If so update chairs and sit there
            rear++;
            rear = rear % MAX_CHAIRS;
            chairs[rear] = SID;	//Insert SID into queue
            pthread_mutex_unlock(&chairs_lock);	//Release lock on chairs
            pthread_mutex_lock(&printf_lock);
            printf("SID:%d waiting for TA\n", SID);
            pthread_mutex_unlock(&printf_lock);            
            sem_post(&Student_waiting);	//Inform TA student is waiting
            sem_wait(&TA_done);	//Wait till TA is done with student
            pthread_mutex_lock(&printf_lock);
            printf("SID:%d done with TA\n", SID);
            pthread_mutex_unlock(&printf_lock);            
        }
        else
        {
            pthread_mutex_unlock(&chairs_lock);	//If not chair found , return to programming (in next loop)
            pthread_mutex_lock(&printf_lock);
            printf("SID:%d did not find chairs.\n", SID);
            pthread_mutex_unlock(&printf_lock);
        }
    }
    
    pthread_exit(0);
}
