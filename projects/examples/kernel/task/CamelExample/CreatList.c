#define _CREATLIST_C_

#include "Camel_list.h"
#include "CreatList.h"

// 定义根节点
struct xLIST list_test_root;

// 定义 3 个节点
struct xLIST_NODE test_node1;
struct xLIST_NODE test_node2;
struct xLIST_NODE test_node3;

void List_example(void)
{
    // 初始化根节点
    vListInit(&list_test_root);
    
    // 节点初始化
    vListInitNode(&test_node1);
    test_node1.xNodeValue = 1;
    
    vListInitNode(&test_node2);
    test_node2.xNodeValue = 2;
    
    vListInitNode(&test_node3);
    test_node3.xNodeValue = 3;
    
    
    // 将节点插入链表
    vListInsert(&list_test_root, &test_node2);
    vListInsert(&list_test_root, &test_node1);

    vListInsert(&list_test_root, &test_node3);
    
    for(;;)
    {
        // 不干什么
    }
}
