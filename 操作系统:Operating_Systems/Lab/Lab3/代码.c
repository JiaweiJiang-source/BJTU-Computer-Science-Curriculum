#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct PCB{
    char name[20];
//  运行时间
    int running_time;
//   到达时间
    int enter_time;
//    优先级
    int priority;
//   完成时间
    int done_time;    //用于时间片轮转
    int copyRunning_time;  //用于时间片轮转
//  进程开始运行的时间
    int start_time;

    struct PCB* next;
} PCB;

typedef struct PCBQueue{
    PCB* firstProg;
    PCB* LastProg;
    int size;
} PCBQueue;

void Queueinit(PCBQueue* queue){
    if(queue==NULL){
        return;
    }
    queue->size = 0;
    queue->LastProg = (PCB*)malloc(sizeof(PCB));
    queue->firstProg = queue->LastProg;
}



void EnterQueue(PCBQueue* queue,PCB* pro){   //加入进程队列
    queue->LastProg->next = (PCB*)malloc(sizeof(PCB));
    queue->LastProg = queue->LastProg->next;
    queue->LastProg->enter_time = pro->enter_time;
//    将name复制给 LastProg
    memcpy(queue->LastProg->name,pro->name,sizeof(pro->name));
    queue->LastProg->priority = pro->priority;
    queue->LastProg->running_time = pro->running_time;
    queue->LastProg->copyRunning_time = pro->copyRunning_time;
    queue->LastProg->start_time = pro->start_time;
    queue->size++;
}
PCB* poll(PCBQueue* queue){
    PCB* temp = queue->firstProg->next;
    if(temp == queue->LastProg){
        queue->LastProg=queue->firstProg;
        queue->size--;
        return temp;
    }
    queue->firstProg->next = queue->firstProg->next->next;
    queue->size--;
    return temp;
}

void inputPCB(PCB pro[],int num){
    for(int i=0;i<num;i++){
        PCB prog ;
        printf("请输入第%d个进程的名字，到达时间 ，服务时间，优先级\n",i+1);
        scanf("%s",prog.name);
        scanf("%d",&prog.enter_time) ;
        scanf("%d",&prog.running_time);
        prog.copyRunning_time = prog.running_time;
        scanf("%d",&prog.priority);
        pro[i]=prog;
    }
}

//冒泡排序算法（每次找到最大的放在末尾）
void sortWithEnterTime(PCB pro[],int num){
    for(int i=1;i<num;i++){
        for(int j= 0;j<num-i;j++){
            if(pro[j].enter_time>pro[j+1].enter_time){
                PCB temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}

void FCFS(PCB pro[],int num){
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);    //按照进入顺序排序
    PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));
    Queueinit(queue);
    EnterQueue(queue,&pro[0]);
    int time = pro[0].enter_time;
    int pronum=1;    //记录当前的进程
    //平均周转时间
    float sum_T_time = 0;
//    带权平均周转时间
    float sum_QT_time = 0 ;

    while(queue->size>0){
        PCB* curpro = poll(queue);   //从进程队列中取出进程
        if(time < curpro->enter_time)
            time =  curpro->enter_time;
        //完成时间
        int done_time = time+curpro->running_time;
        //周转时间（作业完成的时间-作业到达的时间）
        int T_time = done_time - curpro->enter_time;
        sum_T_time += T_time;
        // 带权周转时间（（作业完成的时间-作业到达的时间）/ 作业运行时间）
        float QT_time = T_time / (curpro->running_time+0.0) ;
        sum_QT_time += QT_time;
        for(int tt = time;tt<=done_time && pronum<num;tt++){    //模拟进程的执行过程
            if(tt >= pro[pronum].enter_time){
                EnterQueue(queue,&pro[pronum]);
                pronum++;
            }
        }
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",curpro->name,curpro->enter_time,curpro->running_time,time,done_time
               ,T_time,QT_time);
        time += curpro->running_time;
        if(queue->size==0 && pronum < num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(num+0.0),sum_QT_time/(num+0.0));
}

//按照运行时间排序
void sortWithLongth(PCB pro[],int start,int end){
    int len = end - start;
    if(len == 1)
        return;
    for(int i=1;i<len;i++){
        for(int j= start;j<end-i;j++){
            if(pro[j].running_time>pro[j+1].running_time){
                PCB temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}
void SJF(PCB pro[],int num) {
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);
    PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));;
    Queueinit(queue);
    EnterQueue(queue,&pro[0]);
    int time = pro[0].enter_time;
    int pronum=1;    //记录当前的进程
    float sum_T_time = 0,sum_QT_time = 0;
    while(queue->size>0){
        PCB* curpro = poll(queue);   //从进程队列中取出进程
        if(time <  curpro->enter_time)
            time =  curpro->enter_time;
        int done_time = time+curpro->running_time;
        int T_time = done_time - curpro->enter_time;
        float QT_time = T_time / (curpro->running_time+0.0) ;
        sum_T_time += T_time;
        sum_QT_time += QT_time;
        int pre = pronum;
        for(int tt = time;tt<=done_time&&pronum<num;tt++){    //模拟进程的执行过程
            if(tt>=pro[pronum].enter_time){ // 统计从此任务开始到结束之间有几个进程到达
                pronum++;
            }
        }
        sortWithLongth(pro,pre,pronum);//将到达的进程按照服务时间排序
        for(int i=pre;i<pronum;i++){    //将进程链入队列
            EnterQueue(queue,&pro[i]);
        }
        pre = pronum;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",curpro->name,curpro->enter_time,curpro->running_time,time,done_time
               ,T_time,QT_time);
        time +=  curpro->running_time;
        if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(num+0.0),sum_QT_time/num);
}
//按照响应比排序（倒序）
void sortWithResponse(PCB pro[],int start,int end){
    int len = end - start;
    if(len == 1)
        return;
    for(int i=1;i<len;i++){
        for(int j= start;j<end-i;j++){
            //计算响应比
            float m = (pro[j].start_time-pro[j].enter_time+pro[j].running_time)/(pro[j].running_time+0.0);
            float n = (pro[j+1].start_time-pro[j+1].enter_time+pro[j+1].running_time)/(pro[j+1].running_time+0.0);
            if(m < n){
                PCB temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}
//高响应比优先
void HRRN(PCB pro[],int num) {
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);
    PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));;
    Queueinit(queue);
    EnterQueue(queue,&pro[0]);
    int time = pro[0].enter_time;
    int pronum=1;    //记录当前的进程
    float sum_T_time = 0,sum_QT_time = 0;
    while(queue->size>0){
        PCB* curpro = poll(queue);   //从进程队列中取出进程
        if(time <  curpro->enter_time)
            time =  curpro->enter_time;
        int done_time = time+curpro->running_time;
        int T_time = done_time - curpro->enter_time;
        float QT_time = T_time / (curpro->running_time+0.0) ;
        sum_T_time += T_time;
        sum_QT_time += QT_time;
        int pre = pronum;
        for(int tt = time;tt<=done_time&&pronum<num;tt++){    //模拟进程的执行过程
            if(tt>=pro[pronum].enter_time){ // 统计从此任务开始到结束之间有几个进程到达
                pronum++;
            }
        }
        sortWithResponse(pro,pre,pronum);//将到达的进程按照响应时间排序
        for(int i=pre;i<pronum;i++){    //将进程链入队列
            EnterQueue(queue,&pro[i]);
        }
        pre = pronum;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",curpro->name,curpro->enter_time,curpro->running_time,time,done_time
               ,T_time,QT_time);
        time +=  curpro->running_time;
        if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(num+0.0),sum_QT_time/num);
}

//按权重排序
void sortWithPriority(PCB pro[],int start,int end){
    int len = end - start;
    if(len == 1) return ;
    for(int i=1;i<len;i++){
        for(int j= start;j<end-i;j++){
            if(pro[j].priority>pro[j+1].priority){
                PCB temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}
//优先级调度算法
void HPF(PCB pro[],int num){
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);
    PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));;
    Queueinit(queue);
    EnterQueue(queue,&pro[0]);
    int time = pro[0].enter_time;
    int pronum=1;    //记录当前的进程
    float sum_T_time = 0,sum_QT_time = 0;
    while(queue->size>0){
        PCB* curpro = poll(queue);   //从进程队列中取出进程
        if(time<curpro->enter_time)
            time =  curpro->enter_time;
        int done_time = time+curpro->running_time;
        int T_time = done_time - curpro->enter_time;
        float QT_time = T_time / (curpro->running_time+0.0) ;
        sum_T_time += T_time;
        sum_QT_time += QT_time;
        int pre = pronum;
        for(int tt = time;tt<=done_time&&pronum<num;tt++){    //模拟进程的执行过程
            if(tt>=pro[pronum].enter_time){ // 统计从此任务开始到结束之间有几个进程到达
                pronum++;
            }
        }
        sortWithPriority(pro,pre,pronum);//将到达的进程按照服务时间排序
        for(int i=pre;i<pronum;i++){    //将进程链入队列
            EnterQueue(queue,&pro[i]);
        }
        pre = pronum;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",curpro->name,curpro->enter_time,curpro->running_time,time,done_time
               ,T_time,QT_time);
        time +=  curpro->running_time;
        if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(num+0.0),sum_QT_time/(num+0.0));
}
//时间片轮转调度
void RR(PCB pro[],int num){
    printf("请输入时间片大小");
    int timeslice;
    scanf("%d",&timeslice);
    printf("进程 到达时间 服务时间 进入时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);
    PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));;
    Queueinit(queue);
    //第一个进程开始运行的时间就是到达时间
    pro[0].start_time = pro[0].enter_time;
    EnterQueue(queue,&pro[0]);
    int time = 0;
    int pronum = 1;
    float sum_T_time = 0,sum_QT_time = 0;
    while(queue->size>0){
        PCB* curpro = poll(queue);    // 从队列中取出头节点
        if(time<curpro->enter_time)
            time = curpro->enter_time;
        if(timeslice >= curpro->running_time){   // 如果剩余时间小于时间片  则此任务完成
            for(int tt = time;tt<=time+curpro->running_time&&pronum<num;tt++){    // 模拟进程的执行过程
                if(tt>=pro[pronum].enter_time){ // 统计从此任务开始到结束之间有几个进程到达
                    pro[pronum].start_time = tt;
                    EnterQueue(queue,&pro[pronum]);
                    pronum++;
                }
            }
            time += curpro->running_time;
            curpro->running_time = 0;
            curpro->done_time = time;
            int T_time = curpro->done_time-curpro->start_time;
            float QT_time = T_time / (curpro->copyRunning_time+0.0);
            sum_T_time += T_time;
            sum_QT_time += QT_time;
            printf("%s\t%d\t%d\t  %d\t   %d\t %d\t  %.2f\n",curpro->name,curpro->enter_time,curpro->copyRunning_time,
                   curpro->start_time,curpro->done_time,T_time,QT_time);
            if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
                pro[pronum].start_time = pro[pronum].enter_time;
                EnterQueue(queue,&pro[pronum]);
                pronum++;
            }
            continue;
        }
        // 运行时间大于时间片
        for(int tt = time;tt<=time+timeslice&&pronum<num;tt++){    //模拟进程的执行过程
            if(tt>=pro[pronum].enter_time){ // 统计从此任务开始到结束之间有几个进程到达
                pro[pronum].start_time = tt;
                EnterQueue(queue,&pro[pronum]);
                pronum++;
            }
        }
        time += timeslice;
        curpro->running_time -= timeslice;
        //当前程序未完成  继续添加到队列中
        EnterQueue(queue,curpro);
        if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            pro[pronum].start_time = pro[pronum].enter_time;
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n\n",sum_T_time/(num+0.0),sum_QT_time/(num+0.0));
}
void choiceMenu(){
    printf("请选择进程调度算法：\n\n");
    printf("1.先来先服务算法\n2.短进程优先算法\n3.高优先级优先\n4.时间片轮转算法\n5.高响应比优先算法\n6.退出\n");
}
void menu(){
    int proNum;
    printf("请输入进程的个数：");
    scanf("%d",&proNum);
    PCB pro[proNum];
    inputPCB(pro,proNum);
    choiceMenu();
    int choice;
    while(1){
        scanf("%d",&choice);
        switch(choice){
            case 1:FCFS(pro,proNum);choiceMenu();break;
            case 2:SJF(pro,proNum);choiceMenu();break;
            case 3:HPF(pro,proNum);choiceMenu();break;
            case 4:RR(pro,proNum);choiceMenu();break;
            case 5:HRRN(pro,proNum);choiceMenu();break;
            case 6:return;
        }
    }
}
int main(){
    menu();
    return 0;
}
