#define _CAMEL_PORT_C_

#include "camelOS.h"
#include "camel_task.h"
#include <csi_core.h>
//#include "ARMCM3.h"

static UBaseType_t uxCriticalNesting = 0xaaaaaaa;

// 宏定义
#define portINITIAL_XPSR        ( 0x01000000 )
#define portSTART_ADDRESS_MASK  (( StackType_t ) 0xfffffffeUL )

#define portNVIC_SYSPRI2_REG    ( *(( volatile uint32_t *) 0xe000ed20 ))

#define portNVIC_PENDSV_PRI     ((( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 16UL )
#define portNVIC_SYSTICK_PRI    ((( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

// SysTick 配置寄存器 
#define portNVIC_SYSTICK_CTRL_REG			( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG			( * ( ( volatile uint32_t * ) 0xe000e014 ) )

#ifndef configSYSTICK_CLOCK_HZ
	#define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ
	// 确保SysTick的时钟与内核时钟一致
	#define portNVIC_SYSTICK_CLK_BIT	( 1UL << 2UL )
#else
	#define portNVIC_SYSTICK_CLK_BIT	( 0 )
#endif

#define portNVIC_SYSTICK_INT_BIT			( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT			( 1UL << 0UL )



// 函数声明
void prvStartFirstTask( void );
void vPortSVCHandler( void );
void xPortPendSVHandler( void );
void vPortSetupTimerInTerrupt( void );

// 任务栈初始化函数
static void prvTaskExitError( void )
{
    // 函数停止在这里
    for(;;);
}

StackType_t *pxPortInitStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
    // 异常发生时，自动加载到 CPU 寄存器的内容
    pxTopOfStack --;
    *pxTopOfStack = portINITIAL_XPSR;                                   // xPSR 的 bit24 必须设置 1
    pxTopOfStack --;
    *pxTopOfStack = (( StackType_t )pxCode )&portSTART_ADDRESS_MASK;    // PC, 即任务入口函数
    pxTopOfStack --;
    *pxTopOfStack = ( StackType_t )prvTaskExitError;                    // LR, 函数返回地址
    pxTopOfStack -= 5;  // R12, R3, R2 and R1 默认初始化为 0
    *pxTopOfStack = ( StackType_t )pvParameters;                        // R0, 任务形参
    
    // 异常发生时，手动加载到 CPU 寄存器的内容
    pxTopOfStack -= 8;  // R11, R10, R9, R8, R7, R6, R5 and R4 默认初始化为 0
    
    // 返回栈顶指针，此时 pxTopOfStack 指向空闲栈
    return pxTopOfStack;
}

StackType_t *camel_task_stack_init(StackType_t *stack_base, uint32_t stack_size, void *arg, TaskFunction_t entry)
{
    StackType_t *stk;
    register int *gp asm("x3");
    uint32_t temp = (uint32_t)(stack_base + stack_size);

    temp &= 0xFFFFFFFCUL;

    stk = (StackType_t *)temp;

    *(--stk) = (uint32_t)entry;                   /* PC            */
    *(--stk) = (uint32_t)0x15151515L;             /* X15           */
    *(--stk) = (uint32_t)0x14141414L;             /* X14           */
    *(--stk) = (uint32_t)0x13131313L;             /* X13           */
    *(--stk) = (uint32_t)0x12121212L;             /* X12           */
    *(--stk) = (uint32_t)0x11111111L;             /* X11           */
    *(--stk) = (uint32_t)arg;                     /* X10           */
    *(--stk) = (uint32_t)0x09090909L;             /* X9            */
    *(--stk) = (uint32_t)0x08080808L;             /* X8            */
    *(--stk) = (uint32_t)0x07070707L;             /* X7            */
    *(--stk) = (uint32_t)0x06060606L;             /* X6            */
    *(--stk) = (uint32_t)0x05050505L;             /* X5            */
    *(--stk) = (uint32_t)0x04040404L;             /* X4            */
    *(--stk) = (uint32_t)gp;                      /* X3            */
    *(--stk) = (uint32_t)task_function;    		  /* X1            */

    return stk;	
}

extern void SystemInit(void);

//  调度器启动函数
BaseType_t xPortStartScheduler( void )
{
    // 配置 PendSV 和 SysTick 的中断优先级为最低
//    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
//    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
    
    // 初始化 SysTick 
//    vPortSetupTimerInTerrupt();

	
    // 启动第一个任务，不再返回
    prvStartFirstTask();
    
    return 0;
}

extern void cpu_first_task_start(void);

// 首次启动第一个任务
void prvStartFirstTask( void )
{
    cpu_first_task_start();
}


// SVCHandler 进行任务切换
void vPortSVCHandler( void )
{
//    extern pxCurrentTCB;
//    
//    PRESERVE8
//    
//    ldr r3, = pxCurrentTCB  // 加载 pxCurrentTCB 的地址到 r3
//    ldr r1, [r3]            // 加载 pxCurrentTCB 到 r1
//    ldr r0, [r1]            // 加载 pxCurrentTCB 指向的值到 r0, 即当前第一个任务的任务栈栈顶指针
//    ldmia r0!, {r4-r11}     // 以 r0 为基地址，将栈里面的内容加载到 r4-r11 寄存器，同时 r0 会递增
//    msr psp, r0             // 将 r0 的值，即任务栈指针更新到 psp
//    isb
//    mov r0, #0              // 将 r0 的值，设置为 0
//    msr basepri, r0         // 将 basepri 寄存器设置为0，即所有的中断都没有被屏蔽
//    orr r14, #0x0d           // 当从 SVC 中断服务退出前，通过向 R14 最后4位按位或上 0x0d,
//                            // 使得硬件在退出时，使用进程堆栈指针 PSP 完成出栈操作并返回后进入线程模式、返回 Thumb 状态
//    //  r14 的 bit1 : 0 PSP 1 MSP；bit2: 0 特权模式  1 用户模式
//    
//    bx r14                  // 异常返回，这个时候栈中的剩下内容将会自动加载到 CPU 寄存器
//                            // xPSR,PC(任务入口地址),R14,R12,R3,R2,R1,R0(任务形参) 同时 PSP 的值也将更新，即指向任务栈的栈顶
}

void xPortPendSVHandler(void)
{
//    extern pxCurrentTCB;
//    extern vTaskSwitchContext;
//    
//    PRESERVE8
//    
//    // 当进入 PendSVC Handler 时，上一个任务运行环境，即：
//    // xPSR,PC(任务入口地址),R14,R12,R3,R2,R1,R0(任务形参),这些将自动保存到任务栈中，剩下的r4-r11需要手动保存
//    // 获取任务栈指针到 r0
//    mrs r0, psp
//    isb
//    
//    ldr r3, =pxCurrentTCB   // 加载 pxCurrentTCB 的地址到 r3
//    ldr r2, [r3]            // 加载 pxCurrentTCB 到 r2
//    
//    stmdb r0!, {r4-r11}     // 将 CPU 寄存器 r4-r11 的值存储到 r0 指向的地址
//    str r0, [r2]            // 将任务栈的新的栈顶指针存储到当前任务TCB的第一个成员，即栈顶指针
//    
//    stmdb sp!,{r3,r14}      // 将 r3 和 r14 临时压入堆栈，因为即将调用函数 
//                            // 调用函数时，返回地址自动保存到 r14 中，导致 r14 的值会被覆盖，所以 r14 的值需要入栈保护
//                            // r3 保存的当前激活的任务 TCB 指针( pxCurrentTCB ),函数调用后会用到，因此也需要入栈保护
//    
//    mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY    // 进入临界段
//    msr basepri, r0         // 屏蔽所有中断
//    dsb
//    isb
//    bl vTaskSwitchContext   // 调用函数 vTaskSwitchContext，寻找新的任务运行，通过使变量 pxCurrentTCB 指向新的任务来实现任务切换
//    mov r0, #0              // 退出临界段
//    msr basepri, r0
//    ldmia sp!, {r3,r14}     // 恢复 r3, r14
//    
//    ldr r1, [r3]            
//    ldr r0, [r1]            // 当前激活的任务 TCB 第一项保存了任务堆栈的栈顶指针，现在栈顶值存入了 r0
//    ldmia r0!,{r4-r11}      // 出栈
//    msr psp, r0
//    isb
//    bx r14                  // 异常发生时，R14 中保存异常返回标志，包括返回后进入线程模式还是处理器模式
//                            // 使用 psp 堆栈指针还是 msp 堆栈指针，当调用 bx r14 指令后，硬件会知道要从硬件返回
//                            // 然后出栈，这个时候堆栈指针 psp 硬件指向了 新任务堆栈的正确位置
//                            // 当新任务的运行地址被出栈到 pc 寄存器后，新的任务也会被执行
//    nop    
}



///  临界段相关函数
void vPortEnterCritical( void )
{
    portDISABLE_INTERRUPTS();
    uxCriticalNesting ++;
    
    if( uxCriticalNesting == 1)
    {
        
    }
}

void vPortExitCritical( void )
{
    uxCriticalNesting --;
    if( uxCriticalNesting == 0)
    {
        portENABLE_INTERRUPTS();
    }
}


// ================================================================= //
// ===================== SysTick 初始化 ============================ //
// ================================================================= //
void vPortSetupTimerInTerrupt( void )
{
    // 设置 Reload 寄存器的值
    portNVIC_SYSTICK_LOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
    
    // 设置系统定时器的时钟为内核时钟
    // 使能 System Tick 定时器中断
    // 使能 System Tick 定时器
    portNVIC_SYSTICK_CTRL_REG = (   portNVIC_SYSTICK_CLK_BIT | 
                                    portNVIC_SYSTICK_INT_BIT |
                                    portNVIC_SYSTICK_ENABLE_BIT );
}

// System Tick 中断服务函数
void xPortSysTickHandler( void )
{
	PSRC_ALL();
    // 关中断
//    vPortRaiseBASEPRI();
	portDISABLE_INTERRUPTS();
    
    // 更新系统时基
    xTaskIncrementTick();
    
    // 开中断
//    vPortClearBASEPRIFromISR();
	portENABLE_INTERRUPTS();
    
}


