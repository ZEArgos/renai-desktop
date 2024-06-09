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

void DestroyLinkedList(LinkedList* list)
{
    Node* current_node = list->first_node;
    while (current_node != NULL)
    {
        Node* next_node = current_node->next;
        FreeItem(current_node);
        current_node = next_node;
    }

    FreeItem(list);
}

u8 VerifyNodeContents(NodeType type, NodeContents* contents)
{
    if (type == shader && contents->shader != 0) return SUCCESS;
    else if (type == texture && contents->texture.inner != 0 &&
             contents->texture.vao != 0)
        return SUCCESS;

    return FAILURE;
}

u8 AddNode(LinkedList* list, NodeType type, Node* node)
{
    if (!VerifyNodeContents(type, &node->contents)) return FAILURE;

    Node* current_node = list->first_node;
    while (current_node->next != NULL) current_node = current_node->next;

    current_node->next = node;
    list->last_node = current_node->next;

    return SUCCESS;
}

Node* GetNode(LinkedList* list, const char* name)
{
    if (strcmp(list->first_node->name, name) == 0) return list->first_node;
    if (strcmp(list->last_node->name, name) == 0) return list->last_node;

    Node* current_node = list->first_node->next;
    while (current_node != NULL)
    {
        if (strcmp(current_node->name, name) == 0) return current_node;
        current_node = current_node->next;
    }

    return NULL;
}
