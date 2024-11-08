
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20

typedef struct process{

       int BT, AT, TAT, WT, PNO, PID;

       char name[10];

} process;

typedef struct RQ{

       process pr[MAX];

       int f, r;

} RQ;

void get_PCB(process[], int *);

void sort_AT(process[], int);

void sort_BT(RQ[]);

void disp_table(process[], int);

void SJF_P(process[], int);

void RR(process p[], int n, int);

float cal_avgwt(process[], int);

float cal_avgtat(process[], int);

void menu(){

       printf("\n\t\t****MENU*****");

       printf("\n\t\t1. RR");

       printf("\n\t\t2. SJF P");

       printf("\n\t\t3. EXIT");

       printf("\n\t\tEnter Choice: \t");
}

void main(){

       int ch, TQ, n;

       process P[MAX];

       float avg_WT, avg_TAT;

       printf("\nEnter Time Quantum for Round Robin : ");

       scanf("%d", &TQ);

       get_PCB(P, &n);

       do{

              menu();

              scanf("%d", &ch);

              switch (ch){

              case 1:

              {

                     RR(P, n, TQ);

                     disp_table(P, n);

                     avg_WT = cal_avgwt(P, n);

                     avg_TAT = cal_avgtat(P, n);

                     printf("\nAVERAGE WT  : %f", avg_WT);

                     printf("\nAVERAGE TAT : %f", avg_TAT);

                     break;
              }

              case 2:

              {

                     SJF_P(P, n);

                     disp_table(P, n);

                     avg_WT = cal_avgwt(P, n);

                     avg_TAT = cal_avgtat(P, n);

                     printf("\nAVERAGE WT  : %f", avg_WT);

                     printf("\nAVERAGE TAT : %f", avg_TAT);

                     break;
              }

              case 3:

                     break;
              }

       } while (ch != 3);
}

float cal_avgwt(process p[], int n){

       float avg = 0;

       int i;

       for (i = 0; i < n; i++){

              avg += p[i].WT;
       }

       avg = avg / n;

       return avg;
}

float cal_avgtat(process p[], int n){

       float avg = 0;

       int i;

       for (i = 0; i < n; i++){

              avg += p[i].TAT;
       }

       avg = avg / n;

       return avg;
}

int get_first_process(process p[], int n){

       int min, j, in;

       min = p[0].AT;

       for (j = 0; j < n; j++){

              if (p[j].AT < min){

                     in = j;
              }
       }

       return in;
}

void check_arrival(RQ *r, process p[], int time, int n){

       int i, j, flag = 0;

       for (i = 0; i < n; i++){

              for (j = 0; j <= r->r; j++){

                     if (strcmp(p[i].name, r->pr[j].name) == 0)

                            flag = 1;

                     else

                            break;
              }

              if (p[i].AT == time && flag == 0){

                     r->r = r->r + 1;

                     r->pr[r->r] = p[i];
              }

              flag = 0;
       }
}

void RR(process p[], int n, int tq){

       int count = 0, i, start, time = 0;

       RQ r;

       r.f = r.r = -1;

       start = get_first_process(p, n);

       r.pr[0] = p[start];

       r.f = r.r = 0;

       check_arrival(&r, p, time, n);

       while (r.f != -1){

              for (count = 0; count < tq; count++){

                     r.pr[r.f].BT--;

                     time++;

                     if (r.pr[r.f].BT == 0)

                            break;

                     check_arrival(&r, p, time, n);
              }

              if (r.pr[r.f].BT != 0){

                     r.pr[r.r + 1] = r.pr[r.f];

                     r.r++;
              }

              else{

                     p[r.pr[r.f].PID].TAT = time - r.pr[r.f].AT;

                     p[r.pr[r.f].PID].WT = p[r.pr[r.f].PID].TAT - p[r.pr[r.f].PID].BT;
              }

              if (r.f == r.r)

                     r.f = r.r = -1;

              else

                     r.f++;
       }
}

void sort_BT(RQ *r){

       int i, j;

       process temp;

       for (i = r->f; i <= r->r; i++){

              for (j = i + 1; j <= r->r; j++){

                     if (r->pr[j].BT < r->pr[i].BT){

                            temp = r->pr[j];

                            r->pr[j] = r->pr[i];

                            r->pr[i] = temp;
                     }
              }
       }
}

int get_total_time(process p[], int n){

       int i, sum = 0;

       for (i = 0; i < n; i++){

              sum += p[i].BT;
       }

       return sum;
}

void SJF_P(process p[], int n){

       int time = 0, start, sum, i;

       RQ r;

       r.f = r.r = -1;

       start = get_first_process(p, n);

       r.pr[0] = p[start];

       r.f = r.r = 0;

       check_arrival(&r, p, time, n);

       sum = get_total_time(p, n);

       for (i = 0; i < sum; i++){

              r.pr[r.f].BT--;

              time++;

              if (r.pr[r.f].BT == 0){

                     p[r.pr[r.f].PID].TAT = time - r.pr[r.f].AT;

                     p[r.pr[r.f].PID].WT = p[r.pr[r.f].PID].TAT - p[r.pr[r.f].PID].BT;

                     r.f++;
              }

              check_arrival(&r, p, time, n);

              sort_BT(&r);
       }
}

void sort_AT(process p[], int n){

       int i, j;

       process temp;

       for (i = 0; i < n; i++){

              for (j = i + 1; j < n; j++){

                     if (p[j].AT < p[i].AT){

                            temp = p[j];

                            p[j] = p[i];

                            p[i] = temp;
                     }
              }
       }
}

void disp_table(process p[], int n){

       int i;

       printf("\n\n P_NAME \t AT \t BT \t WT \t TAT \t");

       for (i = 0; i < n; i++){

              printf("\n %-10s \t %d \t %d \t %d \t %d \t", p[i].name, p[i].AT, p[i].BT, p[i].WT, p[i].TAT);
       }
}

void get_PCB(process p[], int *n){

       int i;

       printf("\nEnter total no of processes : ");

       scanf("%d", n);

       for (i = 0; i < *n; i++){

              printf("\n Enter Following details for Process\n%d", i + 1);

              printf("\nName :\t");

              scanf("%s", p[i].name);

              printf("\nArrival Time :\t");

              scanf("%d", &p[i].AT);

              printf("\nBurst Time :\t");

              scanf("%d", &p[i].BT);

              p[i].PID = i;
       }
}




dityagawas@adityagawas-VirtualBox:~$ gcc robin1.c -0 robin1
gcc: error: unrecognized command-line option ‘-0’
adityagawas@adityagawas-VirtualBox:~$ gcc robin1.c -o robin1
cc1: fatal error: robin1.c: No such file or directory
compilation terminated.
adityagawas@adityagawas-VirtualBox:~$ gcc robins.c -o robins
cc1: fatal error: robins.c: No such file or directory
compilation terminated.
adityagawas@adityagawas-VirtualBox:~$ gcc sjf.c -o sjf
cc1: fatal error: sjf.c: No such file or directory
compilation terminated.
adityagawas@adityagawas-VirtualBox:~$ gcc robin.c -o robin
adityagawas@adityagawas-VirtualBox:~$ ./robin

Enter Time Quantum for Round Robin : 2

Enter total no of processes : 5

 Enter Following details for Process
1
Name :	p1

Arrival Time :	5

Burst Time :	4

 Enter Following details for Process
2
Name :	p2

Arrival Time :	6

Burst Time :	3

 Enter Following details for Process
3
Name :	p3

Arrival Time :	4

Burst Time :	2

 Enter Following details for Process
4
Name :	p4

Arrival Time :	7

Burst Time :	1

 Enter Following details for Process
5
Name :	p5

Arrival Time :	3

Burst Time :	4

		****MENU*****
		1. RR
		2. SJF P
		3. EXIT
		Enter Choice: 	1


 P_NAME 	 AT 	 BT 	 WT 	 TAT 	
 p1         	 5 	 4 	 32723 	 1940511714 	
 p2         	 6 	 3 	 -785340496 	 32723 	
 p3         	 4 	 2 	 0 	 1 	
 p4         	 7 	 1 	 1943001776 	 32723 	
 p5         	 3 	 4 	 -3 	 1 	
AVERAGE WT  : 231538784.000000
AVERAGE TAT : 388115456.000000
		****MENU*****
		1. RR
		2. SJF P
		3. EXIT
		Enter Choice: 	2


 P_NAME 	 AT 	 BT 	 WT 	 TAT 	
 p1         	 5 	 4 	 5 	 9 	
 p2         	 6 	 3 	 1 	 4 	
 p3         	 4 	 2 	 0 	 2 	
 p4         	 7 	 1 	 0 	 1 	
 p5         	 3 	 4 	 -3 	 1 	
AVERAGE WT  : 0.600000
AVERAGE TAT : 3.400000
		****MENU*****
		1. RR
		2. SJF P
		3. EXIT
		Enter Choice: 	    


