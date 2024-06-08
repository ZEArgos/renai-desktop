#include "LinkedList.h"
#include <Logger.h>

Node* __CreateNode(NodeType type, const char* name, NodeContents contents)
{
    Node* created_node = malloc(sizeof(struct Node));
    created_node->next = NULL;
    created_node->name = name;
    created_node->type = type;
    created_node->contents = contents;

    return created_node;
}
LinkedList* CreateLinkedList(NodeType type, Node* head)
{
    if (head == NULL)
    {
        PrintWarning("Cannot initialize linked list with a null head node.");
        return NULL;
    }

    LinkedList* created_list = malloc(sizeof(struct LinkedList));
    created_list->type = type;
    created_list->first_node = head;
    created_list->last_node = head;

    return created_list;
}
