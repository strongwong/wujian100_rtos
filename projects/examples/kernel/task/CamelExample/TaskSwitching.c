#define _TASKSWITCHING_C_

#include "camelOS.h"
#include "camel_task.h"
#include "stdio.h"
#include <csi_core.h>

// 全局变量
portCHAR flag1;
portCHAR flag2;

extern List_t pxReadyTasksLists[ configMAX_PRIORITIES ];  

// 定义任务栈
#define TASK1_STACK_SIZE        128
StackType_t Task1Stack[TASK1_STACK_SIZE];

#define TASK2_STACK_SIZE        128
StackType_t Task2Stack[TASK2_STACK_SIZE];

// 定义任务控制块
TCB_t Task1TCB;
TCB_t Task2TCB;

// 定义任务句柄
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;


// 函数声明
void daley(uint32_t count);
void Task1_Entry( void *p_arg );
void Task2_Entry( void *p_arg );

void TaskSwitching_example(void)
{
//	portDISABLE_INTERRUPTS();
    prvInitTaskLists();
    
    Task1_Handle = xTaskCreateStatic(  Task1_Entry,
                                       "Task1_Entry",
                                       TASK1_STACK_SIZE,
                                       NULL,
									   1,
                                       Task1Stack,
                                       &Task1TCB );
    
    // 核心就是插入函数 vListInsert, 将任务插入到就绪列表中
    vListInsert(&pxReadyTasksLists[1], &Task1TCB.xStateListNode);   // wqq
    
    Task2_Handle = xTaskCreateStatic(  Task2_Entry,
                                       "Task2_Entry",
                                       TASK2_STACK_SIZE,
                                       NULL,
									   2,
                                       Task2Stack,
                                       &Task2TCB );
    vListInsert(&pxReadyTasksLists[2], &Task2TCB.xStateListNode);	// wqq
//    portENABLE_INTERRUPTS();
    vTaskStartScheduler();
}

// 软件延时
void delay( uint32_t count)
{
    for(; count != 0; count --);
}

extern void cpu_task_switch(void);

void wjYIELD(void)
{
	PSRC_ALL();
	portDISABLE_INTERRUPTS();
	vTaskSwitchContext();
	cpu_task_switch();
	portENABLE_INTERRUPTS();
}

// 定义一个任务函数 Task1 入口函数
void Task1_Entry(void *p_arg)
{
    for(;;)
    {
        flag1 = 1;
		printf("flag1 = %d \n", flag1);
		
        delay( 100 );
//        vTaskDelay( 20 );
        flag1 = 0;
		printf("flag1 = %d \n", flag1);
        delay( 100 );
//        vTaskDelay( 20 );

//      taskYIELD();        // 注意，这里是手动切换任务
		wjYIELD();
    }
}

// 定义一个任务函数 Task2 入口函数
void Task2_Entry(void *p_arg)
{
    for(;;)
    {
        flag2 = 1;
		printf("flag2 = %d \n", flag2);
        delay( 100 );
//        vTaskDelay( 20 );
        flag2 = 0;
		printf("flag2 = %d \n", flag2);
        delay( 100 );
//        vTaskDelay( 20 );

//      taskYIELD();
		wjYIELD();    // 手动切换
    }
}

