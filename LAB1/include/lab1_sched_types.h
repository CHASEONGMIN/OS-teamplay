/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */


int endTime;
int timeQuantum;

//job struct
typedef struct joblist{
               char name;
                int arriveTime;
                int runTime;
		int count;
		int ticket;
		int pass_value;
		int stride;
		int count1;
}joblist;

//queue
typedef struct node{
        char data;
	//int time;
        struct node *next;
}node;
typedef struct queue{
        node *front;
        node *rear;
	//int length;
}queue;

void initQueue(queue *queue);
int isEmpty(queue *queue);
void enQueue(queue *queue, char data);
char deQueue(queue *queue);



//functions
void put_works1(joblist *job);//workload1
void put_works2(joblist *job);//workload2
int totalTime(joblist *job);
void showWork(joblist* job);
void showTicket(joblist* job);
void sorting(joblist *job, int n);//sorting
void printQ(queue *q,int totalTime);

//1
void FCFS(joblist *job);
//2
void RR(joblist *job);
//3
void stride(joblist *job, int in);
#endif /* LAB1_HEADER_H*/



