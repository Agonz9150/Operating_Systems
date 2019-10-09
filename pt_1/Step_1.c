#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread(void *);
void SimpleThread(int);

int counter = 0;
int thread_count = 0;
pthread_mutex_t lock;
pthread_barrier_t barr;
int SharedVariable = 0;

//Function for threads to latch on to

void *thread(void* number){
  int num = (long)number;
  SimpleThread(num);
  pthread_exit(NULL);
}

void SimpleThread(int which){
  int num,val;
  for(num = 0; num < 20; num++){
    #ifdef PTHREAD_SYNC // Mutex control
    
      while(pthread_mutex_lock(&lock) != 0){
        usleep(1);
      }      
    #endif
    val = SharedVariable;
    printf("*** thread %d sees value %d\n",which,val);
    SharedVariable = val + 1;
    pthread_mutex_unlock(&lock);
    if(random() > RAND_MAX/2){
      usleep(10);
    }
  }
  counter++;
  // waits for all threads to complete
  #ifdef PTHREAD_SYNC
    int err = pthread_barrier_wait(&barr);
    if(err && err != PTHREAD_BARRIER_SERIAL_THREAD){
      fprintf(stderr,"Error -> wait for barrier failed");
      exit(1); 
    }
  #endif
  val = SharedVariable;
  printf("Thread %d sees final value %d\n",which,val);
}

int main(int argc, const char* argv[]){
  //check for legal arguments
  if(argc != 2){
    fprintf(stderr,"Error: improper number of imputs on command line\n"
            "This program takes 1 input for the number of threads\n");
    return 1;
  }
  thread_count = atoi(argv[1]);
  if(thread_count <= 0){
    fprintf(stderr,"Error: not a valid number for thread count.\n Must be"
	    " a positive interger\n");
    return 1;
  }
  //Safety for mutex fali
  if(pthread_mutex_init(&lock,NULL)){
    fprintf(stderr,"Error: mutex lock has failed");
    return 1;
  }
  //barrier init
  #ifdef PTHREAD_SYNC
  if(pthread_barrier_init(&barr,NULL,thread_count)){
      fprintf(stderr,"Error -> barrier failed");
      return 1;
    }
  #endif
  int loop;
   
  long arr[thread_count];
  pthread_t threads [thread_count];
  
  //create threads
  for(loop = 1;loop <= thread_count; loop++){
    arr[loop - 1] = loop;
    int test = pthread_create(&threads[loop-1],NULL,thread, (void *) arr[loop - 1]);
    // Safety for failed threads
    if(test){
      fprintf(stderr,"Error: return code is %d/n",test);
      return 1;
    }
  }
  pthread_exit(NULL);
  pthread_mutex_destroy(&lock);
  return 0;
}



