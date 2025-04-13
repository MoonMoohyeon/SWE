#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

typedef struct Queue {
    char* data[MAX_SIZE];
    int front;
    int rear;
} Queue;

int frontSensor, leftSensor, rightSensor, dustSensor; // 외부 인터페이스
int moveForward, turnLeft, turnRight, moveBackward, powerUp, powerDown; // 내부 인터페이스
int obstacleLocation[3];
Queue* q; // 결과 출력

void Controller(int* obstacle, int dust);
void DetermineObstacleLocation(int front, int left, int right, int rv[3]);
int DetermineDustLocation(int dust);
int tick(void);
int safeInput(void);
void initQueue(Queue* q);
int isEmpty(Queue* q);
int isFull(Queue* q);
int enqueue(Queue* q, char* value);
void printQueue(Queue* q);

int main(void) {
    q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) return 1;
    initQueue(q);

    while (1) {
        if(tick() == 1) break;
        DetermineObstacleLocation(frontSensor, leftSensor, rightSensor, obstacleLocation);
        Controller(obstacleLocation, DetermineDustLocation(dustSensor));
    }
    printQueue(q);
    free(q);

    return 0;
}

void Controller(int* obstacle, int dust) {
    if (dust && powerDown && !obstacle[0]) {
        powerUp = 1;
        powerDown = 0;
        enqueue(q, "파워업");
    } else {
        powerUp = 0;
        powerDown = 1;
        enqueue(q, "파워다운");
    }

    if (!obstacle[0]) {
        moveForward = 1;
        enqueue(q, "전진");
    } else if (!obstacle[1]) {
        moveForward = 0;
        turnLeft = 1;
        enqueue(q, "좌회전");
        // sleep(5);
        turnLeft = 0;
    } else if (!obstacle[2]) {
        moveForward = 0;
        turnRight = 1;
        enqueue(q, "우회전");
        // sleep(5);
        turnRight = 0;
    } else {
        moveForward = 0;
        moveBackward = 1;
        enqueue(q, "후진");
        // sleep(5);
        moveBackward = 0;

        turnLeft = 1;
        enqueue(q, "좌회전");
        // sleep(5);
        turnLeft = 0;
    }
}

void DetermineObstacleLocation(int front, int left, int right, int rv[3]) {
    rv[0] = front;
    rv[1] = left;
    rv[2] = right;
}

int DetermineDustLocation(int dust) {
    return dust;
}

int tick(void) {
    printf("front obstacle(1 or 0, 2 = 종료): ");
    frontSensor = safeInput();
    if (frontSensor == 2) return 1;
    printf("left obstacle(1 or 0): ");
    leftSensor = safeInput();
    printf("right obstacle(1 or 0): ");
    rightSensor = safeInput();
    printf("dust obstacle(1 or 0): ");
    dustSensor = safeInput();

    return 0;
}

int safeInput(void) {
    int value;
    char check;

    if (scanf("%d%c", &value, &check) != 2 || check != '\n') {
        while (getchar() != '\n');
        return 0;
    }

    return value;
}

void initQueue(Queue* q) {
    q->front = 0;
    q->rear = -1;
}

int isEmpty(Queue* q) {
    return q->front > q->rear;
}

int isFull(Queue* q) {
    return q->rear == MAX_SIZE - 1;
}

int enqueue(Queue* q, char* value) {
    if (isFull(q)) {
        printf("큐가 가득 찼습니다!\n");
        return 0;
    }
    printf("%s\n", value);
    q->data[++q->rear] = strdup(value);
    return 1;
}

void printQueue(Queue* q) {
    if (isEmpty(q)) {
        printf("큐가 비어 있습니다!\n");
        return;
    }
    printf("동작 내용: ");
    for (int i = q->front; i <= q->rear; i++) {
        printf("%s ", q->data[i]);
    }
    printf("\n");
}
