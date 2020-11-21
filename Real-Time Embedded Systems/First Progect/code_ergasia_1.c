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
 
#define QUEUESIZE 10
#define LOOP 5000
 
#define PRODTHREADS 1
#define CONSTHREADS 48
 
// work functions

//calculate tan(pi/arg) 
void *work1(void *arg){
 
int i = (int)arg;
double pi = 3.1415926535;
double ang = (pi/(double)i);
double res = tan(ang);
 
 
printf("tan(pi/%d) = %lf\n",i,res);
 
}
 
//calculate cos(pi/arg) 
void *work2(void *arg){
 
 
int i = (int)arg + 1;
double pi = 3.1415926535;
double ang = (pi/(double)i);
double res = cos(ang);
 
 
printf("cos(pi/%d) = %lf\n",i,res);
 
 
}
 
//calculate power of two of arg
void *work3(void *arg){
 
int i = (int)arg;
int j;
int res = i*i;
 
printf("the power of two of %d is %d\n",i,res);
 
}
 
//just print arg
void *work4(void *arg){
 
 
int i = (int)arg;
printf("I'm work 4 and I print number %d\n",i);
 
}
 
//just print arg
void *work5(void *arg){
 
int i = (int)arg;
printf("I'm work 5 and I print number %d\n",i);
 
 
}
 
 
void *producer (void *args);
void *consumer (void *args);

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
 
queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, workFunction in);
void queueDel (queue *q, workFunction *out); 
 
 
int counter = 0;//counter of items
double sum = 0;//variable for summing of elapsed times
 
int main ()
{
  queue *fifo;
 
 //arrays of threads for producers and consumers
  pthread_t prod_threads[PRODTHREADS];
  pthread_t cons_threads[CONSTHREADS];
 
  fifo = queueInit ();
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
 
  int i;
 
// creation of threads calling pthread_create()

  for(i=0;i<PRODTHREADS;i++){
     pthread_create (&prod_threads[i], NULL, producer, fifo);
  }
 
  for(i=0;i<CONSTHREADS;i++){
     pthread_create (&cons_threads[i], NULL, consumer, fifo);
  }
 
 
//wait for the threads of producer
 
 for(i=0;i<PRODTHREADS;i++){
     pthread_join (prod_threads[i], NULL);
  }

//wait for the threads of consumer
 for(i=0;i<CONSTHREADS;i++){
     pthread_join (cons_threads[i], NULL);
   }
 
  queueDelete (fifo);
 
  return 0;
}
 
void *producer (void *q)
{
  queue *fifo;
  int i;
 
  workFunction w;// item that wil be added in q
  
  typedef void *(*funcType)(void *);
  funcType arrayOfWorks[5];
 
  
  
 //filling of the array with pointers to works-functions
  arrayOfWorks[0] = work1;
  arrayOfWorks[1] = work2;
  arrayOfWorks[2] = work3;
  arrayOfWorks[3] = work4;
  arrayOfWorks[4] = work5;
 
 
  fifo = (queue *)q;
 
  for (i = 0; i < LOOP; i++) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->full) {
      printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
	
    int selectedWork;
  
  selectedWork = (((unsigned int)pthread_self() + counter) % 5);// each thread will add to queue that work that is placed at "selectedWork" cell of arrayOfWorks
 
    // iteration of element w
    w.work = arrayOfWorks[selectedWork];
    w.arg=((unsigned int)pthread_self() % 10000) + counter ;

    //add item in queue
    queueAdd(fifo, w);
    
    

    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
 
  }
 
  return (NULL);
}
 
void *consumer (void *q)
{
  queue *fifo;
 
  double avg;
 
  unsigned long long timeElapsed;
 
  fifo = (queue *)q;
 
  workFunction w;
 
  while(1) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
 
    struct timeval fin;//variable that stores time with gettimeofday()
  
    w = fifo->buf[fifo->head];//store the item in head of queue,this item will be deleted
 
    gettimeofday(&fin, NULL);
  
    queueDel (fifo, &w);//delete item
    
    unsigned long long startValueinuSec = w.time;//store the adding time in startValueInusec
    unsigned long long endValueInusec = (unsigned long long)fin.tv_sec*1000000 + (unsigned long long)fin.tv_usec;
   
    timeElapsed = endValueInusec - startValueinuSec;//elapsed time calculated as substraction between end and start value
   
    sum = sum + ((double)timeElapsed)/1000;//add time elapsed in sum variable and convert into milliseconds

    counter++;//increase counter of items

    avg = ((sum)/((double)counter));//calculation of average

    printf("item was received in %llu us\n",endValueInusec);

    printf("the average is %f ms\n",avg);

    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
   
    w.work(w.arg);//execution of work-function
 
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

  printf("item was added in %llu us\n",in.time);
  
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
