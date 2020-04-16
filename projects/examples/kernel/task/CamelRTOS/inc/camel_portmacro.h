#ifndef _CAMEL_PORTMACRO_H_
#define _CAMEL_PORTMACRO_H_

#include "stdint.h"
#include "stddef.h"


// 数据类型重定义
#define portCHAR 	char
#define portFLOAT	float
#define portDOUBLE	double
#define portLONG	long
#define portSHORT	short
#define portSTACK_TYPE	uint32_t
#define portBASE_TYPE	long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if (configUSE_16_BIT_TICKS == 1)
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xfffffffUL
#endif

static int clzInC(unsigned int x);

// 多优先级宏定义
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#endif

#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
	// 检测优先级配置
	#if( configMAX_PRIORITIES > 32 )
		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32. It is very rare that a system requiress 
	#endif
	
	// 根据优先级设置/清除优先级位图中相应的位
	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ))
	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ))

	// ------------------------------- //
	#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31UL - ( uint32_t ) clzInC( ( uxReadyPriorities )))

#endif // taskRECORD_READY_PRIORITY 



//中断控制状态寄存器，0xe000ed04
// bit 28 PENDSVSET： PendSV 挂起
#define portNVIC_INT_CTRL_REG   (*(( volatile uint32_t *)0xe000ed04 ))
#define portNVIC_PENDSVSET_BIT  ( 1UL << 28UL )     // 第 28 位设置为 1

#define portSY_FULL_READ_WRITE  (15)

extern void cpu_task_switch(void);
extern int cpu_intrpt_switch(void);

#define portYIELD()                                 \
{   PSRC_ALL();                                     \
	portDISABLE_INTERRUPTS();                       \
	vTaskSwitchContext();							\
	portENABLE_INTERRUPTS();	                    \
}

//	cpu_intrpt_switch();						

// 临界段的保护
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );




// 空闲任务
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )


//#define portINLINE  __inline

extern uint32_t cpu_intrpt_save(void);
extern void cpu_intrpt_restore(uint32_t psr_c);
uint32_t psr_c;

#define PSRC_ALL()  uint32_t psr_c

#define portDISABLE_INTERRUPTS()    do{ psr_c = cpu_intrpt_save(); } while(0) //vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()     do{ cpu_intrpt_restore(psr_c); } while(0) //vPortSetBASEPRI( 0 )

#define portENTER_CRITICAL()        vPortEnterCritical()
#define portEXIT_CRITICAL()         vPortExitCritical()

//#define portSET_INTERRUPT_MASK_FROM_ISR()   ulPortRaiseBASEPRI()    // 在中断中使用
//#define portCLEAR_INTERRUPT_MASK_FROM_ISR() vPortSetBASEPRI( x )



//  关中断
//void vPortRaiseBASEPRI( void )
//{
	
//    uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
//    
//    __asm
//    {
//        msr basepri , ulNewBASEPRI     // 11 大于11 的不能被响应
//        dsb
//        isb
//    }
//}

// 开中断
//void vPortClearBASEPRIFromISR( void )
//{
//    __asm
//    {
//        msr basepri, #0
//    }
//}


//uint32_t ulPortRaiseBASEPRI( void )
//{
//    uint32_t ulReturn, ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
//    
//    __asm
//    {
//        mrs ulReturn, basepri       // 将原先 basepri 的值保存在 返回值 中
//        msr basepri, ulNewBASEPRI   // 再设置 basepri
//        dsb
//        isb
//    }
//    
//    return ulReturn;
//}


//void vPortSetBASEPRI( uint32_t ulBASEPRI )
//{
//    __asm
//    {
//        msr basepri, ulBASEPRI
//    }
//}

static int clzInC(unsigned int x)
{
  if (!x) return 32;
  int e = 31;
  //1111 1111 1111 1111 0000 0000 0000 0000
  if (x&0xFFFF0000)   { e -=16; x >>=16; }
  //0000 0000 0000 0000 1111 1111 0000 0000
  if (x&0x0000FF00)   { e -= 8; x >>= 8; }
  //0000 0000 0000 0000 0000 0000 1111 0000
  if (x&0x000000F0)   { e -= 4; x >>= 4; }
  //0000 0000 0000 0000 0000 0000 0000 1100
  if (x&0x0000000C)   { e -= 2; x >>= 2; }
  //0000 0000 0000 0000 0000 0000 0000 0010
  if (x&0x00000002)   { e -= 1; }
  return e;
}


#endif  // _CAMEL_PORTMACRO_H_

