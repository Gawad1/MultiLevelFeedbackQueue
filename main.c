#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int pid;
    int burst_time;
};
struct Node
{
    struct Process data;
    struct Node* next;
};
struct Queue
{
    struct Node* front;
    struct Node* rear;
    int maxSize;
    int currentSize;
};
void initialize_queue(struct Queue* queue, int maxSize)
{
    queue->front=NULL;
    queue->rear=NULL;
    queue->maxSize=maxSize;
    queue->currentSize=0;
}

int isQueueFull(const struct Queue* queue)
{
    return (queue->currentSize ==queue->maxSize);
};

int isQueueEmpty(const struct Queue* queue)
{
    return (queue->front ==NULL);
};
void enqueue(struct Queue* queue, struct Process process)
{
    if (isQueueFull(queue))
    {
        printf("full queue");
        return;
    }

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data =process;
    newNode->next =NULL;
    if (isQueueEmpty(queue))
    {
        queue->front =newNode;
        queue->rear =newNode;
    }
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    queue->currentSize++;
};
struct Process dequeue(struct Queue* queue)
{
    struct Process emptyProcess = {0, 0};

    if (isQueueEmpty(queue))
    {
        printf("already emty");
        return emptyProcess;
    }
    struct Process process =queue->front->data;
    queue->front = queue->front->next;
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
    queue->currentSize--;
    return process;
}
struct Process seek(struct Queue* queue)
{
    struct Process emptyProcess = {0, 0};

    if (isQueueEmpty(queue))
    {
        printf("no seek its empty");
        return emptyProcess;
    }
    struct Process process =queue->front->data;
    return process;
}
void display_queue(const struct Queue* queue)
{
    printf("\nContent :");
    struct Node* current =queue->front;
    while (current !=NULL)
    {
        printf("(PID: %d, Burst Time: %d) ", current->data.pid, current->data.burst_time);
        current = current->next;
    }
    printf("\n");
}
void display_all_queues(const struct Queue* queue0,const struct Queue* queue1,const struct Queue* queue2,const struct Queue* finished)
{
    printf("\n");
    printf("Queue_0:");
    display_queue(queue0);
    printf("Queue_1:");
    display_queue(queue1);
    printf("Queue_2:");
    display_queue(queue2);
    printf("Finished_Queue:");
    display_queue(finished);
    printf("\n\n\n");
}
struct Process process_generator(int pid)
{
    struct Process process;
    process.pid =pid;
    process.burst_time = 1+rand()%100;
    return process;
}
void decreaseBurstTime(struct Process* process, int time)
{
    if (process != NULL)
    {
        process->burst_time -= time;
        if (process->burst_time < 0)
        {
            process->burst_time = 0;
        }
    }
}
int main()
{
    const int size_0=10;
    const int size_1=20;
    const int size_2=30;
    const int size_longterm=100;
    const int size_finished=100;
    const int rr_0=8;
    const int rr_1=16;
    struct Queue Queue_0;
    struct Queue Queue_1;
    struct Queue Queue_2;
    struct Queue finished;
    struct Queue longTermScheduler;
    initialize_queue(&Queue_0,size_0);
    initialize_queue(&Queue_1,size_1);
    initialize_queue(&Queue_2,size_2);
    initialize_queue(&finished,size_finished);
    initialize_queue(&longTermScheduler,size_longterm);
    for (int i = 0; i < 100; i++)
    {
        struct Process newProcess =process_generator(i + 1);
        enqueue(&longTermScheduler, newProcess);
    }
    printf("\nLong term Scheduler:");
    display_queue(&longTermScheduler);
    printf("\n\n");
    display_all_queues(&Queue_0,&Queue_1,&Queue_2,&finished);
    while(!isQueueFull(&finished))
    {
        printf("New CPU cycle:");

        while((!isQueueFull(&Queue_0))&&!isQueueEmpty(&longTermScheduler))
        {
            enqueue(&Queue_0,dequeue(&longTermScheduler));

        }
        display_all_queues(&Queue_0,&Queue_1,&Queue_2,&finished);
        for(int i=0; i<5; i++)
        {
            if(!isQueueEmpty(&Queue_0))
            {
                struct Process p=seek(&Queue_0);
                if(p.burst_time>rr_0&&!(isQueueFull(&Queue_1)))
                {
                    struct Process dequeuedProcess = dequeue(&Queue_0);
                    decreaseBurstTime(&dequeuedProcess,rr_0);
                    enqueue(&Queue_1, dequeuedProcess);
                }
                else if(p.burst_time<=rr_0)
                {
                    enqueue(&finished,dequeue(&Queue_0));

                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }

        }
        printf("After working on queue 0:");
        display_all_queues(&Queue_0,&Queue_1,&Queue_2,&finished);
        for(int i=0; i<3; i++)
        {
            if(!isQueueEmpty(&Queue_1))
            {
                struct Process p=seek(&Queue_1);
                int x=rand() % 2;

                if(p.burst_time>rr_1)
                {
                    if((!isQueueFull(&Queue_2))&&x==0)
                    {
                        struct Process dequeuedProcess = dequeue(&Queue_1);
                        decreaseBurstTime(&dequeuedProcess,rr_1);
                        enqueue(&Queue_2, dequeuedProcess);
                    }
                    if((!isQueueFull(&Queue_0))&&x==1)
                    {
                        struct Process dequeuedProcess = dequeue(&Queue_1);
                        decreaseBurstTime(&dequeuedProcess,rr_1);
                        enqueue(&Queue_0, dequeuedProcess);
                    }

                }
                else if(p.burst_time<=rr_1)
                {
                    enqueue(&finished,dequeue(&Queue_1));


                }
            }
            else
            {
                break;
            }

        }
        printf("After working on queue 1:");
        display_all_queues(&Queue_0,&Queue_1,&Queue_2,&finished);
        for(int i=0; i<2; i++)
        {
            if(!isQueueEmpty(&Queue_2))
            {
                struct Process p=dequeue(&Queue_2);
                enqueue(&finished, p);
            }
            else
            {
                break;
            }
        }
        printf("After working on queue 2:");
        display_all_queues(&Queue_0,&Queue_1,&Queue_2,&finished);

    }

    return 0;
}
