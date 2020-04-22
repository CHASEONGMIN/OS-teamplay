/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32153682 32154579
*	    Student name : 이창민(LEE Changmin) 차성민
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */

//functions
/*================================================================================*/
/*QUEUE============================================================*/
void initQueue(queue *q){
        q->front = NULL;
        q->rear = NULL;
}

int isEmpty(queue *q){
        if(q->front == NULL){
                return 1;
        }
        else
                return 0;
}

void enQueue(queue *q, char data){
        node *newnode = (node*)malloc(sizeof(node));
        newnode->next = NULL;
        newnode->data = data;
//	newnode->time = t;

        if(isEmpty(q)){
                q->front = newnode;
                q->rear = newnode;
        }
	else{	
		q->rear->next = newnode;
                q->rear = newnode;
	}
}


char deQueue(queue *q){
     
	if(isEmpty(q)){
		return 'x';
	}
	else{
		node* delete_node;
		char return_data;
        	delete_node = q->front;
        	return_data = delete_node->data;
        	q->front = q->front->next;
		free(delete_node);
        	return return_data;
	}
}

/*=================================================================*/

/*make workload====================================================*/
void put_works1(joblist *job){
        //A
        job[0].name = 'A';
        job[0].arriveTime = 0;
        job[0].runTime = 3;
	job[0].count = 0;
	job[0].ticket=100;
	job[0].pass_value = 0;

        //B
        job[1].name = 'B';
        job[1].arriveTime = 2;
        job[1].runTime = 6;
	job[1].count = 0;
	job[1].ticket=50;
        job[1].pass_value = 0;

        //C
        job[2].name = 'C';
        job[2].arriveTime = 4;
        job[2].runTime = 4;
	job[2].count = 0;
	job[2].ticket=250;
        job[2].pass_value = 0;

        //D
        job[3].name = 'D';
        job[3].arriveTime = 5;
        job[3].runTime = 3;
	job[3].count = 0;
	job[3].ticket=-1;
	
        //E
        job[4].name = 'E';
        job[4].arriveTime = 2;
        job[4].runTime = 2;
	job[4].count = 0;
	job[4].ticket=-1;
}


void put_works2(joblist *job){
        srand(time(NULL));

        for(int i=0;i<5;i++){
                job[i].name = 'A' + i;
                job[i].arriveTime = rand()%10;
              // printf("%d   ",job[i].arriveTime);
                job[i].runTime = rand()%5+1;
		job[i].count = 0;
		job[i].ticket = rand()%5+1;
        }
}

/*====================================================================*/
/*showWork=============================================================*/
void showWork(joblist* job){
	int i;
	for(i=0;i<5;i++){
		printf("job[%d].name : %c\n", i, job[i].name);
		printf("job[%d].arriveTime : %d\n", i, job[i].arriveTime);
		printf("job[%d].runTime : %d\n", i, job[i].runTime);
	}
}
void showTicket(joblist* job){
	int i;
	for(i=0;i<5;i++){
		printf("job[%d].name : %c\n", i, job[i].name);
		printf("job[%d].ticket : %d\n", i, job[i].ticket);
		//printf("job[%d].runTime : %d\n", i, job[i].runTime);
	}
}	

/*====================================================================*/
/*sorting=============================================================*/
void sorting(joblist *job, int n){
        int i, j;
        int temp;

        for(i = 0; i<n-1; i++){
                for(j=0; j<n-1-i ; j++){
                        if(job[j].arriveTime>job[j+1].arriveTime){
                                temp = job[j].arriveTime;
                                job[j].arriveTime = job[j+1].arriveTime;
                                job[j+1].arriveTime = temp;
                        }
                }
        }
}
/*=======================================================================*/

/*print Queue===========================================================*/
void printQ(queue *q,int totalTime){
        while(totalTime){
                printf("%c",deQueue(q));
                totalTime--;
        }
        printf("\n");
}
/*======================================================================*/

/*get total work time===================================================*/
int totalTime(joblist *job){
        int totalTime=0;

        for(int i=0;i<5;i++){
                totalTime = totalTime + job[i].runTime;
        }
        return totalTime;
}
/*======================================================================*/


/*scheduling==============================================================*/
/*1.FCFS------------------------------*/
//아이디어..
        //1. 도착한 job 먼저 순차적으로 실행
        //*도착하는 시간대로 정렬*
void FCFS(joblist *job){
        int i=0; int j, k, l;
        int totalTime=0;

        for(i=0;i<5;i++){
                if(i!=0){
                        endTime = endTime + job[i-1].runTime;
                }
                //앞 빈공간
                printf("job %c :",job[i].name);

                for(j = 0;j<endTime;j++){
                        printf(" ");

                }

                //실행시간
                for(k = 0;k<job[i].runTime;k++){
                        printf("%c",job[i].name);
                }

                printf("\n");
        }
        printf("====================================================\n");
}
/*------------------------------------*/

/*2.RR--------------------------------*/
//아이디어..  ================================>!연결리스트 큐 구현에 문제가 있는듯 하다...., 일단 배열을 이용한 큐를 사용함
        //1. timeQuantum동안만 실행
        //2. 다음 job 도착때까지
        //3. 다음 job이 도착하지 않으면,
        //timeQuantum이 끝난 후 이전에 끝나지 않은 순차적으로  작업 수행
        //*큐 이용해서 job 도착때마다 timeQuantum만큼 큐에 넣음*
        //job A, B, C, D, E를 줄마다 표현하기위해 각각의 큐를 만듬
        //total큐에서 deQueue
        //자신의 job이 아니면 공백을 enQueue, 자신의 큐면 해당 알파벳을 enQueue
void RR(joblist *job){
        int i,j,k,l;
        //queue total, A, B, C, D, E;
        //큐 초기화
        //initQueue(&total);
        //initQueue(&A);initQueue(&B);initQueue(&C);initQueue(&D);initQueue(&E);

        printf("timeQuantum is %d.\n",timeQuantum);

        int totalT = totalTime(job);
	char* q = malloc(sizeof(totalT)/sizeof(int));
	char* A = malloc(sizeof(totalT)/sizeof(int));
	char* B = malloc(sizeof(totalT)/sizeof(int));
	char* C = malloc(sizeof(totalT)/sizeof(int));
	char* D = malloc(sizeof(totalT)/sizeof(int));
	char* E = malloc(sizeof(totalT)/sizeof(int));

	int a = 0;
        for(i=0;i<totalT;i++){
                //job도착 체크 위한 j for문
                for(j=0;j<5;j++){
                        //job이 도착=>새로운 job enQ
                        if( i == job[j].arriveTime){
                                //timeQuantum만큼
                                for(l=0;l<timeQuantum;l++){
                                        if(job[j].runTime > 0){
						q[a] = job[j].name;				
                                               // enQueue(&total, job[j].name);
                                                job[j].runTime--;
						a++;
                                        }
                                }
				
                        }
                        //job 도착하지 않음 => 하던거 계속 수행
                        else{
                                //도착한 job까지만 timeQuantum만큼 enQ반복
                                for(k=0;k<=j;k++){
                                        for(l=0;l<timeQuantum;l++){
                                                if(job[k].runTime > 0){
							q[a] = job[k].name;
                                                        //enQueue(&total, job[k].name);
                                                        job[k].runTime--;
							a++;
                                                }
                                        }
                                }


                        }
                }

        }
	a = 0;
	printf("job A :");
	while(a<totalT){
		if(q[a] == 'A'){
			printf("%c",q[a]);
		}
		else
			printf(" ");
		a++;
	}
	printf("\n");
	printf("job B :");
	a = 0;
        while(a<totalT){
                if(q[a] == 'B'){
                        printf("%c",q[a]);
                }
                else
                        printf(" ");
                a++;
        }
	printf("\n");

	printf("job C :");
	a = 0;
        while(a<totalT){
                if(q[a] == 'C'){
                        printf("%c",q[a]);
                }
                else
                        printf(" ");
                a++;
        }
	printf("\n");

	printf("job D :");
	a = 0;
        while(a<totalT){
                if(q[a] == 'D'){
                        printf("%c",q[a]);
                }
                else
                        printf(" ");
                a++;
        }
	printf("\n");
	a = 0;
	printf("job E :");
        while(a<totalT){
                if(q[a] == 'E'){
                        printf("%c",q[a]);
                }
                else
                        printf(" ");
                a++;
        }
/*
	while(a<totalT){
		char check = q[a];
		switch(check){
			case 'A':
				A[a] = check; B[a] =' '; C[a]=' '; D[a]=' '; E[a]=' ';break;
			case 'B':	
                                A[a] = ' '; B[a] =check; C[a]=' '; D[a]=' '; E[a]=' ';break;
			case 'C':
                                A[a] = ' '; B[a] =' '; C[a]=check; D[a]=' '; E[a]=' ';break;
			case 'D':	
                                A[a] = ' '; B[a] =' '; C[a]=' '; D[a]=check; E[a]=' ';break;
			case 'E':
                                A[a] = ' '; B[a] =' '; C[a]=' '; D[a]=' '; E[a]=check;break;
		}
		a++;
	}
	printArr(A, totalT);
	printArr(B, totalT);
	printArr(C, totalT);
	printArr(D, totalT);
	printArr(E, totalT);
*/
	
/*
	printf("\n");
	printf("%c", deQueue(&total));
	while(totalT){
		printf("%c",deQueue(&total));
		totalT--;
	}*/
/*
        int temp = totalT;
        //deQ했을때, 해당되지않는 job일 경우 공백으로 채움
        while(temp){

                char check = deQueue(&total);
                printf("%c",check);
                switch(check){
                        case 'A':
                                enQueue(&A, check);enQueue(&B,' ');enQueue(&C,' ');enQueue(&D,' ');enQueue(&E,' ');
			 break;
                        case 'B':
                                enQueue(&B, check);enQueue(&A,' ');enQueue(&C,' ');enQueue(&D,' ');enQueue(&E,' ');
                                break;
                        case 'C':
                                enQueue(&C, check);enQueue(&B,' ');enQueue(&A,' ');enQueue(&D,' ');enQueue(&E,' ');
                                break;
                        case 'D':
                                enQueue(&D, check);enQueue(&B,' ');enQueue(&C,' ');enQueue(&A,' ');enQueue(&E,' ');
                                break;
                        case 'E':
                                enQueue(&E, check);enQueue(&B,' ');enQueue(&C,' ');enQueue(&D,' ');enQueue(&A,' ');
                                break;
                }
                temp--;
        }
        printf("\n");
        //각 큐 프린트
        printQ(&A, totalT);
        printQ(&B, totalT);
        printQ(&C, totalT);
        printQ(&D, totalT);
        printQ(&E, totalT);
*/
        printf("\n====================================================\n");

}
/*-------------------------------------*/

/*MLFQ--------------------------------------------*/
//아이디어...
	//1. 큐여러개 
	//2. 큐마다 레벨(우선순위)
	//3. timeQuantum이 다되면 낮은 레벨로
	//4. 새 작업이 들어오면 가장 높은 레벨로 들어가 timeQuantum이 다되면 레벨 낮춤
	//시간할당량 끝? 낮은큐로, 
	//새작업이 들어왔다! => 가장높은 큐로 
	//timeQuantum다쓴후 낮은 레벨로
	//위 작업 반복 언제? totalTime이 끝날때 까지
	//큐는 연결리스트나 배열로
	//RR구현시 연결리스트 큐가 문제 있었음
	//일단 배열로 구현 후, 확장성을 위해 연결리스트로 교체
	//큐 갯수 ? => runTime이 가장 긴것을 timeQuantum으로 나눔
	//but, 실제로는 들어올 작업의 실행시간을 모르므로 => 각 job이 timeQuantum만큼 실행된는
	//횟수를 측정함=> struct joblist에 int count 추가

void MLFQ(joblist *job){
	//얼만큼 쓸지 모르니까일단 m까지 선언하자
	int i,j,k,l,m;
	int T = totalTime(job);
	int COUNT=0;
	

	queue total;
	initQueue(&total);
	//큐 레벨 10까지 있음
	//큐 레벨을 정해주지 않을시, 매번 동적할당이 필요. 따라서 원하는 레벨을 미리 설정해두면 동작시간이 더 줄어들것  같음
	//각큐에 넣고 빼야됨.
	//만약 새로운 job이 들어오지 않으면 출력
	//total에 출력할 순서대로 넣음
	queue Q[10]; 
	//큐 초기화
	for(i=0;i<10;i++){
		initQueue(&Q[i]);
	}
	queue results[5];
	for(i=0;i<5;i++){
                initQueue(&results[i]);
        }
/*
	for(i=0;i<5;i++){
		job[i].	
*/
//	int totalT = totalTime(job);

 /*       char* total = malloc(sizeof(totalT)/sizeof(int));
	char **q = malloc(10);
	for(i=0;i<10;i++){
		malloc
        q[0] = malloc(sizeof(totalT)/sizeof(int));
        q[1] = malloc(sizeof(totalT)/sizeof(int));
        q[2] = malloc(sizeof(totalT)/sizeof(int));
        q[3] = malloc(sizeof(totalT)/sizeof(int));
        q[4] = malloc(sizeof(totalT)/sizeof(int));
	*/

	int level = 0;

	for(i=0;i<T;i++){
		//새로운 작업 도착??
		for(j=0;j<5;j++){
			
			//job이 하나만 들어오고 그 다음에 job이 올때까지 그 job priority를 변경할 필요없이 그냥 수행하면됨
			//맨위에 큐 추가
			if(i == job[j].arriveTime){
				COUNT++;//job이 들어오면 count 증가
				level = job[j].count;
				for(k=0;k<timeQuantum;k++){
					if(job[j].runTime >0){
						enQueue(&Q[level], job[j].name);
						job[j].runTime--;
					}
						
				}
				//큐 레벨을 정하기 위해서 더 낮은 레벨이 필요한경우 체크
				job[j].count++;
			
				
			}
			//job이 처음 하나만 들어온 상태
			else if(COUNT == 1){
				for(k=0;k<timeQuantum;k++){
					if(job[j].runTime>0){
                                        	enQueue(&Q[0], job[j].name);
                                        	job[j].runTime--;
					}
				}

				while(1){
					char ch = deQueue(&Q[0]);
                                        if(ch != 'x'){
						//printf("0번:"); 
                                                printf("%c ", ch);
                                                enQueue(&total, ch);
					}
					else{
						break;
					}
                                }

                                
			}
			//새로운 작업이 아닐경우
			else{
				for(k=0;k<=COUNT;k++){
					//큐체크
					level = job[k].count;
					for(l=0;l<timeQuantum;l++){
						if(job[k].runTime>0){
							//q[level]
							enQueue(&Q[level], job[k].name);
							job[j].runTime--;
						}
					}
						//runTime이 남았으면, count++
					if(job[k].runTime>0){
						job[j].count++;
						/*
							//큐 레벨을 정하기 위해서 더 낮은 레벨이 필요한경우 체크
							if(COUNT < job[j].count){
                                       				 COUNT = job[j].count;
                                			}
						*/
					}
					//printf("total에 넣음\n");
						
					for(k=0;k<=j;k++){
						char ch = deQueue(&Q[k]);
						if(ch != 'x')
							printf("%c", ch);
							enQueue(&total, ch);
					}
					
					
				}
			}
		}
	}
/*
	//job에 넣기
	while(T){
		char type = deQueue(&total);
		//printf("%c", type);
		char blank = ' ';
	
		if(type == 'A'){
			enQueue(&results[0], type);
			enQueue(&results[1], blank);
			enQueue(&results[2], blank);
			enQueue(&results[3], blank);
			enQueue(&results[4], blank);	
		}
		else if(type == 'B'){
                        enQueue(&results[0], blank);
                        enQueue(&results[1], type);
                        enQueue(&results[2], blank);
                        enQueue(&results[3], blank);
                        enQueue(&results[4], blank);
                }
		else if(type == 'C'){
                        enQueue(&results[0], blank);
                        enQueue(&results[1], blank);
                        enQueue(&results[2], type);
                        enQueue(&results[3], blank);
                        enQueue(&results[4], blank);
                }
		else if(type == 'D'){
                        enQueue(&results[0], blank);
                        enQueue(&results[1], blank);
                        enQueue(&results[2], blank);
                        enQueue(&results[3], type);
                        enQueue(&results[4], blank);
                }
		else if(type == 'E'){
                        enQueue(&results[0], blank);
                        enQueue(&results[1], blank);
                        enQueue(&results[2], blank);
                        enQueue(&results[3], blank);
                        enQueue(&results[4], type);
                }
		T--;
	}
	//각 job 출력
	for(i=0;i<5;i++){
		printQ(&results[i],T);
	}
*/

}


	
	

/*Stride-----------------------------------------------------*/

void stride(joblist *job, int in){
	//job[0]=A ticket=100
	//job[1]=B ticket=50
	//job[2]=c ticket=250
	int i;
	
	//pass_value 초기화, count
	for(i=0;i<in;i++){
		job[i].pass_value = 0;
		job[i].count1 = 0;
	}
	
	//result = A stride * B stride * C stride
	int result=1;
	for(i=0;i<in;i++){
		result = result*job[i].ticket;
	}

	/* 수업시간에는 티켓 수들의  최소공배수를구하고, 최소공배수를 각각의 티켓 수로나눈것이 Stride라하였으나,
	Stride의핵심인 각 job들의 티켓비와 최종적인 각 job들의 실행 횟수의 비는 동일하다는 사실은 최소공배수가
	 아닌 각 티켓의 곱을 각각의 티켓 수로 나눈값을St,ride로하여도 변치않으므로 최소공배수를 구하기 위한 함수를
	만들고 사용하기보다 속도적인 측면을 고려하여 티켓들의 곱을 각각의 티켓으로 나누어 Stride값을 구하였다. */
	for(i=0;i<in;i++){
		job[i].stride = result/job[i].ticket;
	}
	
	int min, min_index, count;
	int same;
	int k;
	//printf("A	B	C\n");
	for(k=0;k<in;k++){
		printf("%c	",job[k].name);
	}
	printf("\n");
	for(k=0;k<in;k++){
		printf("%d	",job[k].pass_value);//, job[k].stride);
	}
	printf("\n");
	min = 0;
	int test = 0;
	while(1){
		//가장 작은 pass_value찾음
		i=in;
		min = job[0].pass_value;
		while(i>0){
			if(min>=job[i-1].pass_value){
				min = job[i-1].pass_value;
				min_index = i-1;
			}
			i--;
		}

		//printf("min index = %d\n",min_index);
		//min의 passvalue에 stride값을 더함
		job[min_index].pass_value += job[min_index].stride;
		job[min_index].count1++;	
			
		for(k=0;k<in;k++){
			printf("%d	",job[k].pass_value);
		}
		
		printf("\n");

		same =0;

		count = in;
		while(count-1){
			if(job[count-1].pass_value == job[count-2].pass_value){
				//same++;
				count--;
			}
			else{
				same = -1;
				break;
			}
		}
		
		//pass_value가 같으면 끝
		if(same == 0){
			for(k=0;k<in;k++){
				printf("%c: %d	",job[k].name,job[k].count1);
			}
			break;
		}
		printf("\n");
		
	}
	printf("\n==========================================================================");
}
 

		
	
	

	

	

	



	

