#ifndef _CAMEL_LIST_H_
#define _CAMEL_LIST_H_

#include "camelOS.h"


// 链表结构体定义

// 节点结构体定义
struct xLIST_NODE
{
	TickType_t xNodeValue;				// 辅助值，用于帮助节点做顺序排列
	struct xLIST_NODE *  pxNext;		// 指向链表的下一个节点
	struct xLIST_NODE *  pxPrevious;	// 指向链表的上一个节点
	void * pvOwner;						// 指向拥有该节点的内核对象，通常是 TCB
	void * pvContainer; 				// 指向该节点所在的链表
};
typedef struct xLIST_NODE ListNode_t;	// 节点数据类型重定义

// mini 节点结构体定义
// 用于双向链表的结尾
struct xMINI_LIST_NODE
{
	TickType_t xNodeValue;				// 辅助值，用于帮助节点做顺序排列
	struct xLIST_NODE * pxNext;			// 指向链表的下一个节点
	struct xLIST_NODE * pxPrevious;		// 指向链表的上一个节点
};
typedef struct xMINI_LIST_NODE MiniListNode_t;	// 最小节点数据类型定义

// 链表结构体定义
typedef struct xLIST
{
	UBaseType_t uxNumberOfNodes;	// 链表节点计数器
	ListNode_t * pxIndex;			// 链表节点索引指针
	MiniListNode_t xListEnd;		// 链表最后一个节点
} List_t;


// ======================================================
// ====================== 宏定义 ========================
// ======================================================

// 初始化节点的拥有者
#define listSET_LIST_NODE_OWNER( pxListNode, pxOwner )  ((pxListNode)->pvOwner = ( void *)(pxOwner))
// 获取节点拥有者
#define listGET_LIST_NODE_OWNER( pxListNode ) (( pxListNode )->pvOwner )

// 初始化节点排序辅助值
#define listSET_LIST_NODE_VALUE( pxListNode, xValue ) (( pxListNode )->xNodeValue = ( xValue ))

// 获取节点排序辅助值
#define listGET_LIST_NODE_VALUE( pxListNode )   (( pxListNode )->xNodeValue )

// 获取链表根节点的节点计数器的值
#define listGET_NODE_VALUE_OF_HEAD_ENTRY( pxList )  (( pxList )->xListEnd ).pxNext->xNodeValue )

// 获取链表的入口节点
#define listGET_HEAD_ENTRY( pxList )    ((( pxList )->xListEnd ).pxNext )

// 获取链表的第一个节点
#define listGET_NEXT( pxListNode )  (( pxListNode )->pxNext)

// 获取链表的最后一个节点
#define listGET_END_MARKER( pxList )    (( ListNode_t const *)( &(( pxList )->xListEnd )))
    
// 判断链表是否为空
#define listLIST_IS_EMPTY( pxList ) (( BaseType_t )(( pxList )->uxNumberOfNodes == ( UBaseType_t )0))

// 获取链表的节点数
#define listCURRENT_LIST_LENGTH( pxList )   (( pxList )->uxNuberOfNodes )

// 获取链表节点的 OWNER，即 TCB 
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )                                    \
{                                                                                       \
    List_t * const pxConstList = ( pxList );                                            \
                                                                                        \
    ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;                        \
                                                                                        \
    if( ( void * )( pxConstList )->pxIndex == ( void * ) &(( pxConstList )->xListEnd )) \
    {                                                                                   \
        ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;                    \
    }                                                                                   \
                                                                                        \
    ( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;                                      \
}

// 获取 TCB 入口
#define listGET_OWNER_OF_HEAD_ENTRY( pxList )   (( &(( pxList )->xListEnd ))->pxNext->pvOwner )



// 函数声明
void vListInitNode(ListNode_t * const pxNode);
void vListInit(List_t * const pxList);
void vListInsertEnd( List_t * const pxList, ListNode_t * const pxNewListNode );
void vListInsert( List_t * const pxList, ListNode_t * const pxNewListNode );
UBaseType_t uxListRemove( ListNode_t * const pxNodeToRemove );


#endif

