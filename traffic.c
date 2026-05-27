#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX 100

typedef struct {
    int id;
    int remainingTime;
} Vehicle;

typedef struct {
    Vehicle data[MAX];
    int front, rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

int isEmpty(Queue *q) {
    return (q->front == -1);
}

void enqueue(Queue *q, Vehicle v) {
    if (q->rear == MAX - 1) return;

    if (q->front == -1)
        q->front = 0;

    q->rear++;
    q->data[q->rear] = v;
}

Vehicle dequeue(Queue *q) {
    Vehicle temp;
    temp.id = -1;

    if (isEmpty(q))
        return temp;

    temp = q->data[q->front];

    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front++;

    return temp;
}

void signalCycle() {
    printf("\n RED - STOP (5 sec)\n");
    Sleep(5000);

    printf(" YELLOW - PREPARE (5 sec)\n");
    Sleep(5000);

    printf(" GREEN - GO (45 sec)\n");
}

// ================= FCFS =================
void fcfs(Queue *q) {

    while (!isEmpty(q)) {

        signalCycle();

        DWORD startTime = GetTickCount();

        while ((GetTickCount() - startTime < 45000) && !isEmpty(q)) {

            Vehicle v = dequeue(q);

            printf(" Vehicle %d crossing...\n", v.id);

            Sleep(v.remainingTime * 1000);

            printf(" Vehicle %d crossed\n", v.id);
        }

        printf(" Green ended. Remaining vehicles wait.\n");
    }

    printf("\n All vehicles completed (FCFS).\n");
}

// ================= ROUND ROBIN =================
void roundRobin(Queue *q, int timeQuantum) {

    while (!isEmpty(q)) {

        signalCycle();

        DWORD startTime = GetTickCount();

        while ((GetTickCount() - startTime < 45000) && !isEmpty(q)) {

            Vehicle v = dequeue(q);

            if (v.remainingTime > timeQuantum) {

                printf(" Vehicle %d running for %d sec\n",
                       v.id, timeQuantum);

                Sleep(timeQuantum * 1000);

                v.remainingTime -= timeQuantum;

                enqueue(q, v);
            }
            else {

                printf(" Vehicle %d finishing...\n", v.id);

                Sleep(v.remainingTime * 1000);

                printf(" Vehicle %d completed\n", v.id);
            }
        }

        printf(" Green ended. Remaining vehicles wait.\n");
    }

    printf("\n All vehicles completed (Round Robin).\n");
}

int main() {

    Queue q;
    initQueue(&q);

    int n, choice;

    printf("Enter number of vehicles: ");
    scanf("%d", &n);

    printf("\nChoose Method:\n");
    printf("1. FCFS\n");
    printf("2. Round Robin\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    for (int i = 0; i < n; i++) {

        Vehicle v;

        printf("Enter Vehicle ID: ");
        scanf("%d", &v.id);

        if (choice == 1) {
            printf("Enter crossing time (seconds): ");
            scanf("%d", &v.remainingTime);
        }
        else {
            // For Round Robin → fixed time
            v.remainingTime = 2;
        }

        enqueue(&q, v);
    }

    if (choice == 1)
        fcfs(&q);
    else if (choice == 2)
        roundRobin(&q, 12);
    else
        printf("Invalid Choice!\n");

    return 0;
}