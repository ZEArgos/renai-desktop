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

// Provides the various type definitions used in this file.
#include <Declarations.h>
// Provides shader loading and management functionality.
#include <Shader.h>
// Provides data structures and functionality needed to create and handle
// textures.
#include <Texture.h>

/**
 * @brief The possible types of node, each corresponding to its respective @ref
 * NodeContents counterpart.
 */
typedef enum NodeType
{
    /**
     * @brief The node contains a compiled OpenGL shader.
     */
    shader,
    /**
     * @brief The node contains an OpenGL texture object, and all information
     * associated with it.
     */
    texture
} NodeType;

/**
 * @brief The contents of a node. This can only be one content or the other, two
 * of these objects may not exist on one node at the same time.
 */
typedef union NodeContents
{
    /**
     * @brief An OpenGL shader ID.
     */
    u32 shader;
    /**
     * @brief A texture image and all the information associated with it.
     */
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

// #define list_head_contents first_node->contents
// #define list_head_texture  first_node->contents.texture
// #define list_head_shader   first_node->contents.shader
// #define texture_contents   contents.texture
// #define shader_contents    contents.shader

// #define GetShaderNode(list, name)  GetNode(list, name)->shader_contents
// #define GetTextureNode(list, name) GetNode(list, name)->texture_contents
// #define GetShaderListHead(list)    list->list_head_shader
// #define GetTextureListHead(list)   list->list_head_texture

#define CreateShaderNode(type, name)                                           \
    __CreateNode(type, name, LoadShader(name, __func__), TEXTURE_EMPTY_INIT)
#define CreateTextureNode(type, name, swidth, sheight)                         \
    __CreateNode(type, name, 0, LoadTextureFromFile(name, swidth, sheight))

// stupid fucking solution
Node* __CreateNode(NodeType type, const char* name, u32 shader,
                   Texture texture);

//??!!!! brother why am i take both type and node?? type is stored in node?? fix
LinkedList* CreateLinkedList(NodeType type, Node* head);

#define DestroyNode(node) FreeItem(node)
void DestroyLinkedList(LinkedList* list);

__BOOLEAN VerifyNodeContents(NodeType type, NodeContents* contents);

void AppendNode(LinkedList* list, NodeType type, Node* node);
void InsertNode(LinkedList* list, NodeType type, Node* node);

Node* GetNode(LinkedList* list, const char* name);

#endif // _RENAI_LINKED_LIST_
