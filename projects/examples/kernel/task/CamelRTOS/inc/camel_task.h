#ifndef _CAMEL_TASK_H_
#define _CAMEL_TASK_H_

//#include "camel_config.h"
#include "camel_list.h"
#include "camel_portmacro.h"

#define taskIDLE_PRIORITY		(( UBaseType_t ) 0U )
#define taskYIELD()   portYIELD()

#define taskENTER_CRITICAL()			portENTER_CRITICAL()
#define taskENTER_CRITICAL_FROM_ISR()	portSET_INTERRUPT_MASK_FROM_ISR()

#define taskEXIT_CRITICAL()				portEXIT_CRITICAL()
#define taskEXIT_CRITICAL_FROM_ISR( x )	portEXIT_CRITICAL_FROM_ISR( x )


// 定义任务控制块
typedef struct tskTaskControlBlock
{
    volatile StackType_t    *pxTopOfStack;  // 栈顶
    ListNode_t              xStateListNode; // 任务节点
    StackType_t             *pxStack;       // 任务栈起始地址
    char                    pcTaskName[configMAX_TASK_NAME_LEN]; 
    
    TickType_t              xTicksToDelay;  // 用于延时
	
	UBaseType_t				uxPriority;		// 优先级
} tskTCB;

typedef tskTCB TCB_t;


// 定义任务句柄
typedef void * TaskHandle_t;


// 函数声明
#if( configSUPPORT_STATIC_ALLOCATION == 1)
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
								UBaseType_t	uxPriority,			
                                StackType_t * const puxStackBuffer,
                                TCB_t * const pxTaskBuffer );
#endif  // configSUPPORT_STATIC_ALLOCATION


uint32_t camel_intrpt_enter(void);
void camel_intrpt_exit(void);


void vTaskStartScheduler(void);
void prvInitTaskLists(void);
void vTaskSwitchContext(void);
                                
void vTaskDelay(const TickType_t xTicksToDelay);
void xTaskIncrementTick(void);

void task_function(void);
                                
#endif // _CAMEL_TASK_H_

