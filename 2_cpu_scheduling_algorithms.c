#include <stdio.h>

/* Select one program from the manual: FCFS, SJF, ROUND_ROBIN, or PRIORITY. */
#define FCFS

#if defined(FCFS)

int a[10], b[10], no[10], wt[10], ta[10];

void main()
{
    int i, j, sb = 0, n, l, temp;
    float avgw, tw = 0, tt = 0, avgt;

    printf("Enter no of processes\n");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        no[i] = i;
        printf("Enter the Arrival time and Burst time for process %d\n", i);
        scanf("%d%d", &a[i], &b[i]);
    }

    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                temp = b[j];
                b[j] = b[j + 1];
                b[j + 1] = temp;
                temp = no[j];
                no[j] = no[j + 1];
                no[j + 1] = temp;
            }

    l = a[0];
    for (i = 0; i < n; i++)
    {
        if (i == 0)
            sb = l;
        else
            sb = sb + b[i - 1];
        wt[i] = sb - a[i];
        ta[i] = wt[i] + b[i];
    }

    for (i = 0; i < n; i++)
    {
        tw = tw + wt[i];
        tt = tt + ta[i];
    }
    avgw = tw / n;
    avgt = tt / n;

    printf("\tProcess\t\tAT\t\tBT\t\tWT\t\tTAT\n");
    for (i = 0; i < n; i++)
        printf("\tP%d\t\t%d\t\t%d\t\t%d\t\t%d\n", no[i], a[i], b[i], wt[i], ta[i]);
    printf("\nAverage WT is %f\n", avgw);
    printf("\nAverage TAT is %f\n", avgt);
}

#elif defined(SJF)

int main()
{
    int time, burst_time[10], at[10], sum_burst_time = 0, smallest, n, i;
    int sumt = 0, sumw = 0;

    printf("enter the no of processes : ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        printf("the arrival time for process P%d : ", i + 1);
        scanf("%d", &at[i]);
        printf("the burst time for process P%d : ", i + 1);
        scanf("%d", &burst_time[i]);
        sum_burst_time += burst_time[i];
    }
    burst_time[9] = 9999;

    for (time = 0; time < sum_burst_time;)
    {
        smallest = 9;
        for (i = 0; i < n; i++)
        {
            if (at[i] <= time && burst_time[i] > 0 && burst_time[i] < burst_time[smallest])
                smallest = i;
        }
        printf("P[%d]\t|\t%d\t|\t%d\n", smallest + 1,
               time + burst_time[smallest] - at[smallest], time - at[smallest]);
        sumt += time + burst_time[smallest] - at[smallest];
        sumw += time - at[smallest];
        time += burst_time[smallest];
        burst_time[smallest] = 0;
    }

    printf("\n\n average waiting time = %f", sumw * 1.0 / n);
    printf("\n\n average turnaround time = %f", sumt * 1.0 / n);
    return 0;
}

#elif defined(ROUND_ROBIN)

struct process
{
    char name;
    int at, bt, wt, tt, rt;
    int completed;
    float ntt;
} p[10];

int n;
int q[10];
int front = -1, rear = -1;

void enqueue(int i)
{
    if (rear == 10)
        printf("overflow");
    rear++;
    q[rear] = i;
    if (front == -1)
        front = 0;
}

int dequeue()
{
    if (front == -1)
        printf("underflow");
    int temp = q[front];
    if (front == rear)
        front = rear = -1;
    else
        front++;
    return temp;
}

int isInQueue(int i)
{
    int k;
    for (k = front; k <= rear; k++)
        if (q[k] == i)
            return 1;
    return 0;
}

void sortByArrival()
{
    struct process temp;
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (p[i].at > p[j].at)
            {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
}

void main()
{
    int i, j, time = 0, sum_bt = 0, tq;
    char c;
    float avgwt = 0, avgtt = 0;

    printf("Enter no of processes:");
    scanf("%d", &n);
    for (i = 0, c = 'A'; i < n; i++, c++)
    {
        p[i].name = c;
        printf("\nEnter the arrival time and burst time of process %c: ", p[i].name);
        scanf("%d%d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
        p[i].completed = 0;
        sum_bt += p[i].bt;
    }

    printf("\nEnter the time quantum:");
    scanf("%d", &tq);
    sortByArrival();
    enqueue(0);
    printf("Process execution order: ");
    for (time = p[0].at; time < sum_bt;)
    {
        i = dequeue();
        if (p[i].rt <= tq)
        {
            time += p[i].rt;
            p[i].rt = 0;
            p[i].completed = 1;
            printf(" %c ", p[i].name);
            p[i].wt = time - p[i].at - p[i].bt;
            p[i].tt = time - p[i].at;
            p[i].ntt = ((float)p[i].tt / p[i].bt);
            for (j = 0; j < n; j++)
                if (p[j].at <= time && p[j].completed != 1 && isInQueue(j) != 1)
                    enqueue(j);
        }
        else
        {
            time += tq;
            p[i].rt -= tq;
            printf(" %c ", p[i].name);
            for (j = 0; j < n; j++)
                if (p[j].at <= time && p[j].completed != 1 && i != j && isInQueue(j) != 1)
                    enqueue(j);
            enqueue(i);
        }
    }

    printf("\nName\tArrival Time\tBurst Time\tWaiting Time\tTurnAround Time\t ");
    for (i = 0; i < n; i++)
    {
        avgwt += p[i].wt;
        avgtt += p[i].tt;
        printf("\n%c\t\t%d\t\t%d\t\t%d\t\t%d\t", p[i].name, p[i].at, p[i].bt, p[i].wt, p[i].tt);
    }
    printf("\nAverage waiting time:%f\n", avgwt / n);
    printf("\nAverage turnaround time:%f\n", avgtt / n);
}

#elif defined(PRIORITY)

struct process
{
    char process_name;
    int arr_time, bst_time, ct, waiting_time, turn_around_time, priority;
    int status;
} process_queue[10];

int limit;

void Arr_time_Sorting()
{
    struct process temp;
    int i, j;
    for (i = 0; i < limit - 1; i++)
        for (j = i + 1; j < limit; j++)
            if (process_queue[i].arr_time > process_queue[j].arr_time)
            {
                temp = process_queue[i];
                process_queue[i] = process_queue[j];
                process_queue[j] = temp;
            }
}

void main()
{
    int i, time = 0, bst_time = 0, largest;
    char c;
    float wait_time = 0, turn_around_time = 0, average_waiting_time, average_turn_around_time;

    printf("\nEnter Total Number of Processes:\t");
    scanf("%d", &limit);
    for (i = 0, c = 'A'; i < limit; i++, c++)
    {
        process_queue[i].process_name = c;
        printf("\nEnter Details For Process[%C]:\n", process_queue[i].process_name);
        printf("Enter Arrival Time:\t");
        scanf("%d", &process_queue[i].arr_time);
        printf("Enter Burst Time:\t");
        scanf("%d", &process_queue[i].bst_time);
        printf("Enter Priority:\t");
        scanf("%d", &process_queue[i].priority);
        process_queue[i].status = 0;
        bst_time = bst_time + process_queue[i].bst_time;
    }
    Arr_time_Sorting();
    process_queue[9].priority = 9999;
    printf("\nProcess Name\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnAround Time");
    for (time = process_queue[0].arr_time; time < bst_time;)
    {
        largest = 9;
        for (i = 0; i < limit; i++)
            if (process_queue[i].arr_time <= time && process_queue[i].status != 1 &&
                process_queue[i].priority < process_queue[largest].priority)
                largest = i;

        time = time + process_queue[largest].bst_time;
        process_queue[largest].ct = time;
        process_queue[largest].waiting_time = process_queue[largest].ct -
                                               process_queue[largest].arr_time - process_queue[largest].bst_time;
        process_queue[largest].turn_around_time = process_queue[largest].ct - process_queue[largest].arr_time;
        process_queue[largest].status = 1;
        wait_time = wait_time + process_queue[largest].waiting_time;
        turn_around_time = turn_around_time + process_queue[largest].turn_around_time;
        printf("\n%c\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d", process_queue[largest].process_name,
               process_queue[largest].arr_time, process_queue[largest].bst_time,
               process_queue[largest].priority, process_queue[largest].waiting_time,
               process_queue[largest].turn_around_time);
    }
    average_waiting_time = wait_time / limit;
    average_turn_around_time = turn_around_time / limit;
    printf("\n\nAverage waiting time:\t%f\n", average_waiting_time);
    printf("Average Turnaround Time:\t%f\n", average_turn_around_time);
}

#endif
