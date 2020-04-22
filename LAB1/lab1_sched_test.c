/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32153682 32154579
*	    Student name : 이창민(LEE Changmin) 차성민
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
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
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
	int i = 0;
        int type;

        //사용법
        if(argc != 2){
                printf("---------------------------------------------\n");
                printf("|USEAGE: %s type_number            |\n",argv[0]);
                printf("---------------------------------------------\n");
                printf("|type_number|\n");
                printf("---------------------------------------------\n");
                printf("|1:FCFS |2:RR |3:MLFQ |4. STRIDE|5. Show All|\n");
                printf("---------------------------------------------\n");
                exit(-1);
        }
        type = atoi(argv[1]);

        //workload 두개 생성
        joblist job1[5];
        joblist job2[5];

        //workload 값 입력
        put_works1(job1);
        //T1 = totalTime(job1);
        put_works2(job2);
        //T2 = totlaTime(job2);
	printf("====================================================\n");
	printf("[job1]\n");
	showWork(job1);
	printf("====================================================\n");
	printf("[job2]\n");
	showWork(job2);

        //도착시간대로 정렬
        sorting(job1, 5);
        sorting(job2, 5);

	timeQuantum = 1;

        switch(type){

                case 1:
                        printf("====================================================\n");
                        printf("/*FCFS                                            */\n");
                        printf("====================================================\n");


                        printf("[job1]\n");
                        endTime = 0;
                        FCFS(job1);

                        printf("[job2]\n");
                        endTime = 0;
                        FCFS(job2);
                        break;
                case 2:
                        printf("====================================================\n");
                        printf("/*RR                                              */\n");
                        printf("====================================================\n");

                        endTime = 0;
                        printf("[job1]\n");
                        RR(job1);

                        endTime = 0;
                        printf("[job2]\n");
			RR(job2);
                        break;
                case 3:
			printf("====================================================\n");
                        printf("/*MLFQ                                             */\n");
                        printf("====================================================\n");

                        //endTime = 0;
                        printf("[job1]\n");
                        MLFQ(job1);

                        //endTime = 0;
                        printf("[job2]\n");
                        MLFQ(job2);

                        break;
                case 5:
			printf("====================================================\n");
                        printf("/*FCFS                                            */\n");
                        printf("====================================================\n");


                        printf("[job1]\n");
                        endTime = 0;
                        FCFS(job1);

                        printf("[job2]\n");
                        endTime = 0;
                        FCFS(job2);
			printf("====================================================\n");
                        printf("/*RR                                              */\n");
                        printf("====================================================\n");

                        endTime = 0;
                        printf("[job1]\n");
                        RR(job1);

                        endTime = 0;
                        printf("[job2]\n");
                        RR(job2);
			printf("====================================================\n");
                        printf("/*MLFQ                                             */\n");
                        printf("====================================================\n");

                        //endTime = 0;
                        printf("[job1]\n");
                        MLFQ(job1);

                        //endTime = 0;
                        printf("[job2]\n");
                        MLFQ(job2);

			printf("====================================================\n");
                        printf("/*STRIDE                                           */\n");
                        printf("====================================================\n");
			printf("[job1]\n");
			showTicket(job1);
			printf("\n");
			printf("[job2]\n");
			showTicket(job2);
			printf("\n");
			printf("[job1]\n");
			stride(job1, 3);
			printf("[job2]\n");
			stride(job2, 4);
                        break;
                case 4:
			printf("====================================================\n");
                        printf("/*STRIDE                                           */\n");
                        printf("====================================================\n");
			printf("[job1]\n");
			showTicket(job1);
			printf("\n");
			printf("[job2]\n");
			showTicket(job2);
			printf("\n");
			printf("[job1]\n");
			stride(job1, 3);
			printf("[job2]\n");
			stride(job2, 4);
			//printf("[job2]\n");
                        break;


        }
}


