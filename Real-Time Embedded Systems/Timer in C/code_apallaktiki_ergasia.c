/*
 *  File    : pc.c
 *
 *  Title   : Demo Producer/Consumer.
 *
 *  Short   : A solution to the producer consumer problem using
 *      pthreads.
 *
 *  Long    :
 *
 *  Author  : Andrae Muys
 *
 *  Date    : 18 September 1997
 *
 *  Revised :
 */
 
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
 
#define QUEUESIZE 5
#define LOOP 5000
 
#define PRODTHREADS 1
#define CONSTHREADS 10
 
//the type of elements that queue will be filled
typedef struct {
  void * (*work)(void *);
  void * arg;
  unsigned long long time;//time that item added in the queue
 
}workFunction;
 
typedef struct {
  workFunction buf[QUEUESIZE]; //queue of workfunction items
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;
 
// struct of timer
typedef struct {
  int Period;
  int TasksToExecute;
  int StartDelay;
  int inStartAt;
  unsigned long long epochTimeInUsec;
  void * (*StartFcn)(void *);
  void * (*StopFcn)(void);
  void * (*TimerFcn)(void *);
  void * (*ErrorFcn)(void);
  void * UserData;
  queue * fifo;
  pthread_t thread; // &(t->thread)
 
 
}timer;
 

 
//calculate cos(pi/arg) 
void *work2(void *arg){
 
 
int i = (int)arg + 1;
double pi = 3.1415926535;
double ang = (pi/(double)i);
double res = cos(ang);
 
 
printf("cos(pi/%d) = %lf\n",i,res);
 
 
}
 

 
 
void *producer (void *args);
void *consumer (void *args);
 
 
 
 
//initializations of timer's struct variables
 
 
void start(timer * t){
  //t->StartFcn
  pthread_create (&(t->thread), NULL, producer, t);
}

void startat(timer * t, int y, int m, int d, int h, int min, int sec){
    
  struct tm t1;
  time_t t_of_day;
 
//fill struct's tm variables with desirable arguments
  t1.tm_year = y; 
  t1.tm_mon = m;          
  t1.tm_mday = d;          
  t1.tm_hour = h;
  t1.tm_min = min;
  t1.tm_sec = sec;
  t1.tm_isdst = -1;        
  t_of_day = mktime(&t1);//convert date to seconds from epoch time
  t->epochTimeInUsec = (t_of_day);
  t->inStartAt = 1;
 
  pthread_create (&(t->thread), NULL, producer, t);
}
 
void *errorFcn(void){
  printf("The queue is Full!\n");
}
 
void *stopFcn(void){
 
  printf("Done!\n");
}



void *startFcn(void * arg){
  timer *t;
  t= arg;
  t->UserData = 15;

}
 
 // Helping function for the iteration of timer object
timer * timerInit(int period, int tasks, int startDelay, queue *q,void*(*func)(void *) ){
  timer * t;
  t = (timer *)malloc (sizeof (timer));
  if (t == NULL) return (NULL);
 
  t->Period = period;//Period in ms
  t->StartDelay = startDelay;
  t->TasksToExecute = tasks;
  
  t->fifo = q;
  t->ErrorFcn = errorFcn;
  t->StopFcn = stopFcn;
  t->StartFcn = startFcn;
  t->StartFcn(t);
  t->TimerFcn = func;
  t->inStartAt = 0;
  t->epochTimeInUsec = 0;
  return(t);
 
} 
 
 
queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, workFunction in);
void queueDel (queue *q, workFunction *out); 
 
 
int counter = 0;//counter of items
double sum = 0;//variable for summing of elapsed times
FILE *fptr1;

 
int main ()
{
  fptr1 = fopen("Consumers.txt","w");
  queue *fifo;
  timer *t;
  timer *t1;
  timer *t2;
 
 //array of threads for consumers
  
  pthread_t cons_threads[CONSTHREADS];
 
  fifo = queueInit ();
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
 
  int i;

 t = timerInit(1000,3600,0,fifo,work2);
 t1 = timerInit(100,36000,0,fifo,work2);
 t2 = timerInit(10,360000,0,fifo,work2);

 
// creation of threads calling pthread_create()
 
 
  for(i=0;i<CONSTHREADS;i++){
     pthread_create (&cons_threads[i], NULL, consumer, fifo);
  }
 
/*
Call startat with following arguments: year= desirable year - 1900
                                       month =desirable number of month -1
                                       hour = desirable hour - 2
                                       minutes = desirable minutes
                                       seconds = derirable seconds
*/

//startat(t,2020-1900,9,2,21,11,0);



start(t2);
start(t1);
start(t);
//wait for the threads of producer
 
 
 pthread_join (t->thread, NULL);
 pthread_join (t1->thread, NULL);
 pthread_join (t2->thread, NULL);
  
 
//wait for the threads of consumer
 printf("I'm closing the file!!!!!!!!!!!!!!!!\n");
 fclose(fptr1);
 for(i=0;i<CONSTHREADS;i++){
     pthread_join (cons_threads[i], NULL);
   }
 
  queueDelete (fifo);
  free(t);
  free(t1);
  free(t2);
 
  
  

  return 0;
}
 
void *producer (void *t)
{
   


  timer *tim;
  tim = (timer*)t;
  usleep(tim->StartDelay*1000000);//start Delay in secs
  long long timeToSleep;
  timeToSleep = 1000*tim->Period;
  unsigned long long avgDrift, sumOfDrifts,runTimeProducer, driftAbs,startAddTime, endAddTime,avgAddTime,sumAddTime;
  long long AddTime;
  long long previousCall = 0;
  long long timestmps;
  sumAddTime = 0;
  long long drift;
  unsigned long long maxAddTime, minAddTime;
  unsigned long long maxDrift, minDrift;
  minDrift = 10000;
  minAddTime = 10000;
  //check if startAt function has been called.
  if(tim->inStartAt == 1){
  
    unsigned long long timeInUsec,sleepTime;
    struct timeval timein;//variable that stores time with gettimeofday()
   
    gettimeofday(&timein, NULL);
    timeInUsec = (unsigned long long)timein.tv_sec*1000000 + (unsigned long long)timein.tv_usec;//time in usecs
    sleepTime = (tim->epochTimeInUsec*1000000) - timeInUsec;//calculate what is the time difference between now and chosen date and time 
    printf("I'm going to sleep for %lld us \n", sleepTime);
    printf("future time in us %lld us \n", (tim->epochTimeInUsec*1000000));
    printf("time now in us %lld us \n",timeInUsec);
    usleep(sleepTime);

  }


  int i;
 
  workFunction w;// item that wil be added in q
  
  struct timeval timstmp,StrAddTimstmp,EndAddTimstmp;//variables that stores time with gettimeofday()
  int n;
  n=tim->TasksToExecute;
  
  
  int TasksCounter = 0;
  unsigned long long  sumElapsedTime = 0;//sum of real elapsed time - find the real time
  unsigned long long elapsedTime;
  // iteration of element w
   w.work = tim->TimerFcn;
   w.arg = tim->UserData;//einai sosto???
   
   FILE *fptr, *fptr2;
//open the true file,according to which timer-producer thread is executed now
   if(tim->Period == 1000){
    fptr = fopen("Timer1sec.txt","w");  
    fptr2 = fopen("Timer1secDrift.txt","w");
    }
   if(tim->Period == 100){
    fptr = fopen("Timer0.1sec.txt","w");  
    fptr2 = fopen("Timer0.1secDrift.txt","w");
    }
   if(tim->Period == 10){
    fptr = fopen("Timer0.01sec.txt","w");
    fptr2 = fopen("Timer0.01secDrift.txt","w");  
    }

 
  for (i = 0; i < tim->TasksToExecute; i++) {
  
   usleep(timeToSleep);//sleeep
    
   printf("added %d funcs\n",i);
   
    gettimeofday(&StrAddTimstmp, NULL);//save start time of adding
    
    pthread_mutex_lock (tim->fifo->mut);
    while (tim->fifo->full) {
      printf ("producer: queue FULL.\n");
      tim->ErrorFcn();
      pthread_cond_wait (tim->fifo->notFull, tim->fifo->mut);
    }
    
    
 
    
    gettimeofday(&timstmp, NULL);
    
    queueAdd(tim->fifo, w);//add item in queue
    
    gettimeofday(&EndAddTimstmp, NULL);//save end time after queueAdd has returned
    
    
   

    pthread_mutex_unlock (tim->fifo->mut);
    pthread_cond_signal (tim->fifo->notEmpty);
    
    endAddTime = (unsigned long long)EndAddTimstmp.tv_sec*1000000 + (unsigned long long)EndAddTimstmp.tv_usec;
    startAddTime = (unsigned long long)StrAddTimstmp.tv_sec*1000000 + (unsigned long long)StrAddTimstmp.tv_usec;
    AddTime = endAddTime - startAddTime; // calculate the time to add one func in queue by the producer
    
    if(tim->Period == 10)
      fprintf(fptr,"%lld\n",AddTime);
    else if(tim->Period == 100)
      fprintf(fptr,"%lld\n",AddTime);
    else if(tim->Period == 1000)
      fprintf(fptr,"%lld\n",AddTime);

    sumAddTime = AddTime + sumAddTime;
    avgAddTime = sumAddTime/(i+1);// average add time
   
    timestmps = (long long)timstmp.tv_sec*1000000 + (long long)timstmp.tv_usec;
 
    if(i>0){// time between two adds can be calculated only after the first iteration
     
    
     elapsedTime = timestmps - previousCall; //time between two adds in queue

     runTimeProducer = elapsedTime + runTimeProducer;//run time of producer
     driftAbs = abs(elapsedTime - (tim->Period*1000));//absolute value of difference of period and actual elapsed time between two adds 
     sumOfDrifts = driftAbs + sumOfDrifts;// sum of drifts
     drift = elapsedTime - (tim->Period*1000);//*1000 because period is in ms


    // write drift to the true file for each timer
     if(tim->Period == 10)
      fprintf(fptr2,"%lld\n",drift);
     else if(tim->Period == 100)
      fprintf(fptr2,"%lld\n",drift);
     else if(tim->Period == 1000)
      fprintf(fptr2,"%lld\n",drift);

     






     if(maxDrift<driftAbs){
      maxDrift = driftAbs;
     }
     if(minDrift>driftAbs){
       minDrift = driftAbs;
     }


    if(maxAddTime<AddTime){
      maxAddTime = AddTime;
     }
     if(minAddTime>AddTime){
       minAddTime = AddTime;
     }


     avgDrift = sumOfDrifts/i;//average drift
    
     timeToSleep = timeToSleep - drift; 
     
     if(timeToSleep<0){
       timeToSleep = 0;
       printf("time to sleep is 0!!!!\n");
     }
     

    
 
   }
  
  previousCall = timestmps;
  
  }
//print for each timer in a txt file

  if(tim->Period == 10){//10ms timer

     
    fprintf(fptr,"Stats for 0.01sec timer\n");

    fprintf(fptr,"Average drift: %lld\n", avgDrift); 
    fprintf(fptr,"max drift %lld\n",maxDrift);
    fprintf(fptr,"min drift %lld\n",minDrift);
    fprintf(fptr,"max add time %lld\n",maxAddTime);
    fprintf(fptr,"min add time %lld\n",minAddTime);
   

  }
  else if(tim->Period == 100){//100ms timer

    
    fprintf(fptr,"Stats for 0.1sec timer\n");

    fprintf(fptr,"Average drift: %lld\n", avgDrift); 
    fprintf(fptr,"max drift %lld\n",maxDrift);
    fprintf(fptr,"min drift %lld\n",minDrift);
    fprintf(fptr,"max add time %lld\n",maxAddTime);
    fprintf(fptr,"min add time %lld\n",minAddTime);

  }
  else if(tim->Period == 1000){//1000ms timer

     
    fprintf(fptr,"Stats for 1sec timer\n");

    fprintf(fptr,"Average drift: %lld\n", avgDrift); 
    fprintf(fptr,"max drift %lld\n",maxDrift);
    fprintf(fptr,"min drift %lld\n",minDrift);
    fprintf(fptr,"max add time %lld\n",maxAddTime);
    fprintf(fptr,"min add time %lld\n",minAddTime);

  } 
 
  tim->StopFcn();
  fclose(fptr);
  fclose(fptr2);

  return (NULL);
}
 
void *consumer (void *q)
{
  queue *fifo;
 
  double avg;
 
  unsigned long long timeElapsed;
  
  
  
    
  

  fifo = (queue *)q;
 
  workFunction w;
  struct timeval fin;//variable that stores time with gettimeofday()
 
  while(1) {
    
    
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
 
    
  
    w = fifo->buf[fifo->head];//store the item in head of queue,this item will be deleted
 
   
  
    queueDel (fifo, &w);//delete item
    gettimeofday(&fin, NULL);//hold the time that queueDel() has returned
 
   
   
    
    
    unsigned long long startValueinuSec = w.time;//store the adding time in startValueInusec
    unsigned long long endValueInusec = (unsigned long long)fin.tv_sec*1000000 + (unsigned long long)fin.tv_usec;
   
    timeElapsed = endValueInusec - startValueinuSec;//elapsed time calculated as substraction between end and start value
   
    
 
    counter++;//increase counter of items

 
    
 
 
    
 
 
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
   
    w.work(w.arg);//execution of work-function

    
    
    fprintf(fptr1,"%lld %d\n",timeElapsed,counter);

    
  }
 
  return (NULL);
}
 
 
queue *queueInit (void)
{
  queue *q;
 
  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);
 
 
 
  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);
 
  return (q);
}
 
void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
 
}
 
void queueAdd (queue *q, workFunction in)
{
 
  struct timeval timein;//variable that stores time with gettimeofday()
   
  gettimeofday(&timein, NULL);
 
  in.time = (unsigned long long)timein.tv_sec*1000000 + (unsigned long long)timein.tv_usec;//time in usecs
 
 
  
  q->buf[q->tail] = in;//add item in tail of queue
 
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;
 
  return;
}
 
void queueDel (queue *q, workFunction *out)
{
 
  *out = q->buf[q->head];
 
 
  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;
 
  return;
}
