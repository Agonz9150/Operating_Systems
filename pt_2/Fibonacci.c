#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>

int* arr = NULL;
void *runner(void *param);

int main(int argc, char* argv[]){
  pthread_t tid;
  pthread_attr_t attr;
  int val;
  int i; 
  if(argc != 2){
    fprintf(stderr,"usage: fibonacci <interger value>\n");
    return -1;
  }
  if((val = atoi(argv[1])) < 0){
    fprintf(stderr,"%d must be >= 0\n", val);
    return -1;
  }
  arr = (int*)malloc(sizeof(int) * (val + 1));
  if(arr == NULL){
    fprintf(stderr,"Error: memory was not allocated");
    return -1;
  }
  
  pthread_attr_init(&attr);

  if(pthread_create(&tid,&attr,runner,argv[1]) != 0){
    fprintf(stderr,"Error: thread creation failed");
    free(arr);
    return -1;
  }

  pthread_join(tid,NULL);

  
  for(i = 0; i < (val + 1); i++){
    printf("Value %d = %d\n",i,*(arr + i));
  }
  free(arr);
  return 0;
}

void *runner(void *param){
  int i, upper = atoi(param);
  *(arr) = 0;
  if(upper >= 1){
    *(arr + 1) = 1;
  }
  for(i=2;i <= upper; i++){
    *(arr + i) = *(arr + (i - 1)) + *(arr +(i - 2));
  }
}
