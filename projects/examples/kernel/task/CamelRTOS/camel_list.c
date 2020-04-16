#define _CAMEL_LIST_C_


#include "camelOS.h"
#include <stdlib.h>
#include "camel_list.h"


// 节点初始化
void vListInitNode(ListNode_t * const pxNode)
{
	pxNode->pvContainer = NULL;
}


// 链表根节点初始化
void vListInit(List_t * const pxList)
{
	pxList->pxIndex = (ListNode_t *)&(pxList->xListEnd); // 将链表的索引指针指向最后一个节点
	pxList->xListEnd.xNodeValue = portMAX_DELAY;		 // 将链表最后一个节点的辅助排序的值设置为最大，确保该节点就是链表的最后节点
	
	pxList->xListEnd.pxNext = (ListNode_t *)&(pxList->xListEnd);  //将最后一个节点的pxNext和pxPrevious指针均指向节点自身，表示链表为空
	pxList->xListEnd.pxPrevious = (ListNode_t *)&(pxList->xListEnd);
	
	pxList->uxNumberOfNodes = (UBaseType_t) 0U;	//初始化链表节点计数器的值为0，表示链表为空
}

// 将节点插入到链表的尾部
void vListInsertEnd( List_t * const pxList, ListNode_t * const pxNewListNode )
{
	ListNode_t * const pxIndex = pxList->pxIndex;   // 链表索引节点 
	
	pxNewListNode->pxNext = pxIndex;				// 新节点的下一个指向链表头
	pxNewListNode->pxPrevious = pxIndex->pxPrevious;// 新节点的前一个指向索引节点的前一个
	pxIndex->pxPrevious->pxNext = pxNewListNode;	// 链表索引节点的前一个的下一个指向新节点
	pxIndex->pxPrevious = pxNewListNode;			// 链表索引节点的前一个指向新节点
	
	// 记住该节点所在的链表
	pxNewListNode->pvContainer = (void *) pxList;
	
	// 该链表节点计数器 ++ 自增
	( pxList->uxNumberOfNodes )++;
}

// 将节点按照升序排列插入到链表
void vListInsert( List_t * const pxList, ListNode_t * const pxNewListNode )
{
	ListNode_t *pxIterator; // 插入迭代
	
	// 获取节点的排序辅助值
	const TickType_t xValueOfInsertion = pxNewListNode->xNodeValue;
	
	// 寻找要插入的位置
	if( xValueOfInsertion == portMAX_DELAY )  //如果插入的位置是链表的最后
	{
		pxIterator = pxList->xListEnd.pxPrevious;   // 那么就把最后一个设置为该节点的前一个
	}
    else
    {
        for( pxIterator = ( ListNode_t *)&( pxList->xListEnd);
             pxIterator->pxNext->xNodeValue <= xValueOfInsertion;
             pxIterator = pxIterator->pxNext )
        {
            // 什么也不做，就是迭代直到找到要插入的位置
        }
    }
    
    // 找到后分别设置前后节点
    pxNewListNode->pxNext = pxIterator->pxNext;
    pxNewListNode->pxNext->pxPrevious = pxNewListNode;
    pxNewListNode->pxPrevious = pxIterator;
    pxIterator->pxNext = pxNewListNode;
    
    // 记住该节点的所在链表
    pxNewListNode->pvContainer = (void *) pxList;
    
    // 链表节点计数器
    ( pxList->uxNumberOfNodes ) ++;	
}

// 将节点从链表中删除
UBaseType_t uxListRemove( ListNode_t * const pxNodeToRemove )
{
    // 获取节点所在的链表
    List_t * const pxList = ( List_t *)pxNodeToRemove->pvContainer;
    
    pxNodeToRemove->pxNext->pxPrevious = pxNodeToRemove->pxPrevious;
    pxNodeToRemove->pxPrevious->pxNext = pxNodeToRemove->pxNext;
    
    // 确保索引指针指向有效节点
    if( pxList->pxIndex == pxNodeToRemove)
    {
        pxList->pxIndex = pxNodeToRemove->pxPrevious;
    }
    
    // 初始化该节点所在的链表为空，表示该节点还未插入任何链表
    pxNodeToRemove->pvContainer = NULL;
    
    // 链表节点计数器
    ( pxList->uxNumberOfNodes ) ++;
    
    // 返回链表中剩余节点的个数
    return pxList->uxNumberOfNodes;
}


