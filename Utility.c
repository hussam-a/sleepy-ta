
#include <stdlib.h>
#include <pthread.h>

//External variables used
extern pthread_mutex_t rand_lock;
extern int MIN_PROGRAMMING_TIME;
extern int MAX_PROGRAMMING_TIME;
extern int MIN_HELPING_TIME;
extern int MAX_HELPING_TIME;

//Function to generate a random number from a seed pointer
//Also takes the type of caller as argument, whether TA or student.
//TA seed together with the HELPING_TIME constants determines the amount of time he will sit with the student.
//Student seed together with the PROGRAMMING_TIME constants determines the amount of time he will program
int generateSomeTime(int caller_type, unsigned int* seed)
{
    if (caller_type)
    { 
        //Lock access to random generator before using it
        pthread_mutex_lock(&rand_lock);
        int period = rand_r(seed);	//Get random number
        pthread_mutex_unlock(&rand_lock);
        //Release lock to random generator before using it
        //Return the shifted appropriate number representing the period        
        return (MIN_PROGRAMMING_TIME+period%(MAX_PROGRAMMING_TIME-MIN_PROGRAMMING_TIME));
    }
    else
    {
        //Lock access to random generator before using it    
        pthread_mutex_lock(&rand_lock);
        int period = rand_r(seed); //Get random number
        pthread_mutex_unlock(&rand_lock);
        //Release lock to random generator before using it        
        //Return the shifted appropriate number representing the period
        return (MIN_HELPING_TIME+period%(MAX_HELPING_TIME-MIN_HELPING_TIME));
    }
}
