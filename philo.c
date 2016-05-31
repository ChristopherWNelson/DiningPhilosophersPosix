/*
 * Christopher Nelson
 * CS360
 * Assignment 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "random_r.h"

/* chopstick variables and mutex */
  int chopsticks[5];
  pthread_mutex_t chopsticks_mutex; 
  
/* when printing, philosophers is incremented by 1 so it prints as 1,2,3... 
   instead of 0,1,2... */
void philo(int *philosopher){
  int thinking;
  int eating;
  int thinking_total = 0;
  int eating_total = 0;
  unsigned int state = (unsigned int)pthread_self(); // ID of calling thread
  
  /* use randomGaussian to generate random amounts of time thinking */
      if ((thinking = randomGaussian_r(11,7, &state)) < 0) // if negative
	thinking = 0; // it's 0
      thinking_total += thinking; // increment total
      printf("Philosopher %d thinking for %d seconds (total = %d)\n", *philosopher+1, thinking, thinking_total);
      sleep(thinking); // simulate thinking time

  /* cycle of eating and thinking until all philosophers have eaten for at least 100 seconds */
  while(eating_total < 30){
    pthread_mutex_lock(&chopsticks_mutex); // lock mutex
    
    /* if both adjacent chopsticks available */
    if(chopsticks[*philosopher] == 1 && chopsticks[(*philosopher +1) % 5] == 1){

      chopsticks[*philosopher] = chopsticks[(*philosopher+1) % 5] = 0; // chopsticks are no longer available
      pthread_mutex_unlock(&chopsticks_mutex); // unlock mutex

      /* use randomGaussian to generate random amounts of time eating */
      if ((eating = randomGaussian_r(9,3, &state)) < 0) // if negative
	eating = 0; // it's 0
      eating_total += eating; // increment total
      printf("Philosopher %d eating for %d seconds (total = %d)\n", *philosopher+1, eating, eating_total);
      sleep(eating); // simulate eating time

      chopsticks[*philosopher] = chopsticks[(*philosopher+1) % 5] = 1; // chopsticks are made available.

      /* use randomGaussian to generate random amounts of time thinking */
      if ((thinking = randomGaussian_r(11,7, &state)) < 0) // if negative
	thinking = 0; // it's 0
      thinking_total += thinking; // increment total
      printf("Philosopher %d thinking for %d seconds (total = %d)\n", *philosopher+1, thinking, thinking_total);
      sleep(thinking); // simulate thinking time
    }

    else{
      pthread_mutex_unlock(&chopsticks_mutex); // unlock mutex
      sleep(1);
    }
  }
  /* Philosopher leaves the table once they eat for at least 100 seconds */
  printf("Philosopher %d done with meal and leaves the table\n", *philosopher+1);
}

int main(int argc, char *argv[]){
  /* philosopher variables */
  pthread_t philosophers[5];
  int *philosopher;
  
  /* initialize mutex */
  pthread_mutex_init(&chopsticks_mutex, NULL);
  
  /* all chopsticks are initially available. */
  for(int i=0; i<5; i++){
    chopsticks[i] = 1;
  }

  /* create a new thread for each philosopher */
  for(int i=0; i<5; i++){
    philosopher = (int*) malloc(sizeof(int));
    *philosopher = i;
    pthread_create(&philosophers[i], NULL, (void*)philo, (void*)philosopher);
  }
  
  /* make each calling thread wait for the speciified thread to terminate */
  for(int i=0; i<5; i++){
    pthread_join(philosophers[i], NULL); // like wait() for processes
  }
  return 0;
}
