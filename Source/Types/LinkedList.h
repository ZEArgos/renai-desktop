/**
 * @file LinkedList.h
 * @author Zenais Argos
 * @brief A file to declare all things linked list, implemented by yours truly.
 * @date 2024-06-08
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_LINKED_LIST_
#define _RENAI_LINKED_LIST_

#include <Declarations.h>
#include <Shader.h>
#include <Texture.h>

typedef enum NodeType
{
    shader,
    texture
} NodeType;

typedef union NodeContents
{
    u32 shader;
    Texture texture;
} NodeContents;

typedef struct Node
{
    struct Node* next;
    const char* name;
    NodeType type;

    NodeContents contents;
} Node;

typedef struct LinkedList
{
    NodeType type;

    Node* first_node;
    Node* last_node;
} LinkedList;

#define CreateNode(type, name, contents)                                       \
    __CreateNode(type, name, (NodeContents){contents})
Node* __CreateNode(NodeType type, const char* name, NodeContents contents);

LinkedList* CreateLinkedList(NodeType type, Node* head);

#define DestroyNode(node)       FreeItem(node)
#define DestroyLinkedList(list) FreeItem(list)

#endif // _RENAI_LINKED_LIST_
