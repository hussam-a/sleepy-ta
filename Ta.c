
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//External vars
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

void* TA(void* param)
{
    unsigned int seed = globalSeed;	//Get unique seed
    globalSeed++;
    printf("TA summoned.\n");
    pthread_mutex_unlock(&setup_lock); //Lock related to setuping
    
    while(1)
    {
        pthread_mutex_lock(&chairs_lock);	//Lock chairs
		if (occupied_chairs == 0)
	        printf("No students waiting, TA will sleep:)\n");
        pthread_mutex_unlock(&chairs_lock);	//Release chairs
        sem_wait(&Student_register);	//Wait for student
        pthread_mutex_lock(&chairs_lock);	//Lock chairs
        occupied_chairs--;	//Update them
        int SID = chairs[front];	//Extract student SID
        front++;
        front = front%MAX_CHAIRS;
        int time = generateSomeTime(0, &seed);	//Generate period to help student
        printf("TA will help SID: %d for %d seconds.\n", SID , time);
        pthread_mutex_unlock(&chairs_lock);	//Release access to chairs
        sleep(time);	//Sleep acts as simulation of helping
	    printf("SID:%d done with TA\n", SID);
        sem_post(&TA_done);	//Notify current student that TA is done
        sem_post(&TA_ready);	//Notify next student that TA is ready
    }
    
    pthread_exit(0);
}
