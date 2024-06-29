#include "LinkedList.h"
#include <Logger.h>

#define __TYPE_SWITCH(type, ex1, ex2, ex3, ex4)                      \
    switch (type)                                                    \
    {                                                                \
        case shader:   ex1; break;                                   \
        case texture:  ex2; break;                                   \
        case instance: ex3; break;                                   \
        case scene:    ex4; break;                                      \
    }

Node* __CreateNode(NodeType type, const char* name, void* contents)
{
    Node* created_node = __MALLOC(
        Node, created_node,
        ("Failed to allocate node '%s'. Code: %d.", name, errno));
    created_node->next = NULL;
    created_node->name = name;
    created_node->type = type;

    __TYPE_SWITCH(type, created_node->contents.shader = contents,
                  created_node->contents.texture = contents,
                  created_node->contents.instance = contents,
                  created_node->contents.scene = contents);

    return created_node;
}

LinkedList* CreateLinkedList(Node* head)
{
    LinkedList* created_list = __MALLOC(
        LinkedList, created_list,
        ("Failed to allocate node list of type %d. Code: %d.",
         head->type, errno));
    created_list->type = head->type;
    created_list->first_node = head;
    created_list->last_node = head;

    return created_list;
}

void KillLinkedList(LinkedList* list)
{
    Node* current_node = list->first_node;
    while (current_node != NULL)
    {
        Node* next_node = current_node->next;
        KillNode(current_node);
        current_node = next_node;
    }

    __FREE(list,
           ("The linked list freer was given an invalid list."));
}

//! outdated asf
__BOOLEAN VerifyNodeContents(NodeType type, NodeContents* contents)
{
    if (type == shader && contents->shader != 0) return true;
    else if (type == texture && contents->texture->texture != 0 &&
             contents->texture->vao != 0)
        return true;

    return false;
}

void AppendNode(LinkedList* list, Node* node)
{
    (list)->last_node->next = node;
    (list)->last_node = (list)->last_node->next;
}
void InsertNode(LinkedList* list, Node* node)
{
    Node* last_head = list->first_node;
    list->first_node = node;
    list->first_node->next = last_head;
}

Node* GetNode(LinkedList* list, const char* name)
{
    if (strcmp(list->first_node->name, name) == 0)
        return list->first_node;
    if (strcmp(list->last_node->name, name) == 0)
        return list->last_node;

    Node* current_node = list->first_node->next;
    while (current_node != NULL)
    {
        if (strcmp(current_node->name, name) == 0)
            return current_node;
        current_node = current_node->next;
    }

    return NULL;
}
