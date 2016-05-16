
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t setup_lock;
pthread_mutex_t printf_lock;
pthread_mutex_t rand_lock;
pthread_mutex_t chairs_lock;
sem_t TA_done;
sem_t Student_waiting;
int occupied_chairs;
int MAX_CHAIRS;
int MIN_PROGRAMMING_TIME;
int MAX_PROGRAMMING_TIME;
int MIN_HELPING_TIME;
int MAX_HELPING_TIME;
int globalSeed;
int chairs[3];
int front;
int rear;

void* Student(void* param);
void* TA(void* param);

int main(int argc, char** argv) 
{
	//Command line arguments checking
    int number_of_students;
    if (argc > 1)
        if (argc > 2)
        {    
            printf("Failed. Please check command line arguments.");
            return(EXIT_FAILURE);
        }
        else
        {
            number_of_students = atoi(argv[1]);
            if (number_of_students <1)
            {
                printf("Failed. Please check command line arguments.");
                return(EXIT_FAILURE);                
            }
        }
    else
    {
        number_of_students = 6;
    }
    
    //Setting global variables and initializing mutexes and semaphores
    MAX_CHAIRS = 3;
    occupied_chairs = 0;
    MIN_PROGRAMMING_TIME = 20;
    MAX_PROGRAMMING_TIME = 30;
    MIN_HELPING_TIME = 10;
    MAX_HELPING_TIME = 20;
    globalSeed= time(NULL);
    front = 0;
    rear = -1;
    pthread_mutex_init(&setup_lock, NULL);
    pthread_mutex_init(&printf_lock, NULL);
    pthread_mutex_init(&rand_lock, NULL);
    pthread_mutex_init(&chairs_lock, NULL);
    sem_init(&TA_done, 0,0);
    sem_init(&Student_waiting,0,0);
    
    //Start creating TA then students one by one, using the setup_lock mutex
    pthread_t temp;
    pthread_mutex_lock(&setup_lock);
    pthread_create(&temp,NULL,TA,NULL);
    pthread_mutex_lock(&setup_lock);
    int i;
    for (i = 1 ; i <= number_of_students; i++)
    {
        pthread_create(&temp,NULL,Student, &i);
        pthread_mutex_lock(&setup_lock);
    }
    pthread_mutex_unlock(&setup_lock);
    
    //Wait indefinitely
    pthread_join(temp,NULL);
    
    return (EXIT_SUCCESS);
}

