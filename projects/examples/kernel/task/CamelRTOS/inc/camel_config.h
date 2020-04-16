#ifndef _CAMEL_CONFIG_H_
#define _CAMEL_CONFIG_H_

#define configUSE_16_BIT_TICKS	          0
#define configMAX_TASK_NAME_LEN         ( 16 )
#define configSUPPORT_STATIC_ALLOCATION   1
#define configMAX_PRIORITIES            ( 5 )

#define configKERNEL_INTERRUPT_PRIORITY   255   // 高四位有效，即 0xFF,或 15
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  191 // 高四位有效，即 0xb0, 或 11

#define xPortPendSVHandler  PendSV_Handler
//#define xPortSysTickHandler SysTick_Handler
#define vPortSVCHandler     SVC_Handler


#define configMINIMAL_STACK_SIZE    (( unsigned short ) 128)  // 字

#define configCPU_CLOCK_HZ			( ( unsigned long ) 25000000 )	
#define configTICK_RATE_HZ			( ( TickType_t ) 100 )      // 10ms

#endif // _CAMEL_CONFIG_H_

