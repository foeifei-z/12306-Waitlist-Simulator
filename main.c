#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义候补订单结构体
typedef struct Order {
    char order_id[50];       // 订单号
    char passenger_name[50]; // 旅客姓名
    char id_card[30];        // 身份证号
    struct Order* next;      // 指向下一个节点的指针
} Order;

// 定义队列结构体，包含队头和队尾指针
typedef struct {
    Order* front; // 队头，用于出队
    Order* rear;  // 队尾，用于入队
} Queue;

// 初始化队列
void initQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

// 判断队列是否为空
int isEmpty(Queue* q) {
    return q->front == NULL;
}

// 1. 新增候补订单（入队）
void enqueue(Queue* q, char* id, char* name, char* card) {
    // 创建新节点
    Order* newOrder = (Order*)malloc(sizeof(Order));
    if (newOrder == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    strcpy(newOrder->order_id, id);
    strcpy(newOrder->passenger_name, name);
    strcpy(newOrder->id_card, card);
    newOrder->next = NULL;

    // 如果队列为空，新节点既是队头也是队尾
    if (isEmpty(q)) {
        q->front = newOrder;
        q->rear = newOrder;
    } else {
        // 否则将新节点追加到队尾
        q->rear->next = newOrder;
        q->rear = newOrder;
    }
    printf("【系统提示】订单 %s (%s) 已成功加入候补队列，等待余票。\n", id, name);
}

// 2. 余票发放（核心逻辑）
void releaseTickets(Queue* q, int k) {
    if (isEmpty(q)) {
        printf("【系统提示】当前候补队列为空，无需处理。\n");
        return;
    }

    printf("\n--- 开始发放余票，本次释放余票数: %d ---\n", k);
    int processed = 0;

    // 只要还有余票，且队列不为空，就继续处理
    while (k > 0 && !isEmpty(q)) {
        // 从队头取出一个订单
        Order* current = q->front;
        printf("候补成功 -> 订单号: %s | 旅客姓名: %s | 身份证号: %s\n", 
               current->order_id, current->passenger_name, current->id_card);

        // 出队操作
        q->front = q->front->next;
        
        // 如果出队后队列为空，需要将队尾也置空
        if (q->front == NULL) {
            q->rear = NULL;
        }

        // 释放该节点的内存
        free(current);
        
        // 扣减余票（基础版默认每个订单消耗1张票）
        k--;
        processed++;
    }

    printf("--- 余票发放结束，本次共处理了 %d 个候补订单 ---\n", processed);

    // 3. 余票为0时，判断候补队列是否为空
    if (k == 0 && !isEmpty(q)) {
        printf("【系统提示】余票已用完，当前队列中仍有候补订单在等待。\n");
    } else if (k == 0 && isEmpty(q)) {
        printf("【系统提示】余票已用完，且当前候补队列已清空。\n");
    } else if (k > 0) {
        printf("【系统提示】余票还有剩余 (%d 张)，但候补队列已清空。\n", k);
    }
}

// 打印当前队列（便于演示和测试）
void printQueue(Queue* q) {
    if (isEmpty(q)) {
        printf("当前候补队列为空。\n");
        return;
    }
    printf("当前候补队列情况（队头 -> 队尾）:\n");
    Order* temp = q->front;
    while (temp != NULL) {
        printf("  [%s] %s (%s)\n", temp->order_id, temp->passenger_name, temp->id_card);
        temp = temp->next;
    }
}

// 释放整个队列的内存（程序退出时调用）
void freeQueue(Queue* q) {
    Order* temp;
    while (q->front != NULL) {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
}

int main() {
    Queue waitlist;
    initQueue(&waitlist);

    int choice;
    while (1) {
        printf("\n========== 12306 候补系统（基础版） ==========\n");
        printf("1. 新增候补订单 (入队)\n");
        printf("2. 余票发放 (处理订单)\n");
        printf("3. 查看当前候补队列\n");
        printf("4. 退出程序\n");
        printf("==============================================\n");
        printf("请输入您的操作(1-4): ");
        
        if (scanf("%d", &choice) != 1) {
            // 清理输入缓冲区防止死循环
            while(getchar() != '\n');
            printf("输入无效，请重新输入！\n");
            continue;
        }

        if (choice == 1) {
            char id[50], name[50], card[30];
            printf("请输入订单号: ");
            scanf("%s", id);
            printf("请输入旅客姓名: ");
            scanf("%s", name);
            printf("请输入身份证号: ");
            scanf("%s", card);
            enqueue(&waitlist, id, name, card);
        } 
        else if (choice == 2) {
            int k;
            printf("请输入本次释放的余票数量: ");
            scanf("%d", &k);
            if (k <= 0) {
                printf("余票数量必须大于0！\n");
            } else {
                releaseTickets(&waitlist, k);
            }
        } 
        else if (choice == 3) {
            printQueue(&waitlist);
        } 
        else if (choice == 4) {
            printf("正在退出系统...\n");
            freeQueue(&waitlist); // 释放内存
            printf("感谢使用，再见！\n");
            break;
        } 
        else {
            printf("无效的操作选项，请重新输入！\n");
        }
    }

    return 0;
}