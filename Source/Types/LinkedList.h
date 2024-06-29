/**
 * @file LinkedList.h
 * @author Zenais Argos
 * @brief A file to declare all things linked list, implemented by
 * yours truly.
 * @date 2024-06-08
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_LINKED_LIST_
#define _RENAI_LINKED_LIST_

// Provides the various type definitions used in this file.
#include <Declarations.h>
// Provides shader loading and management functionality.
#include <Scene.h>
#include <Shader.h>
// Provides data structures and functionality needed to create and
// handle textures.
#include <Texture.h>

/**
 * @brief The possible types of node, each corresponding to its
 * respective @ref NodeContents counterpart.
 */
typedef enum NodeType
{
    /**
     * @brief The node contains a compiled OpenGL shader.
     */
    shader,
    /**
     * @brief The node contains an OpenGL texture object, and all
     * information associated with it.
     */
    texture,
    instance,
    /**
     * @brief The node contains a scene object.
     */
    scene
} NodeType;

/**
 * @brief The contents of a node. This can only be one content or the
 * other, two of these objects may not exist on one node at the same
 * time.
 */
typedef union NodeContents
{
    /**
     * @brief An OpenGL shader ID.
     */
    Shader* shader;
    /**
     * @brief A texture image and all the information associated with
     * it.
     */
    Texture* texture;
    TextureInstance* instance;
    /**
     * @brief
     */
    Scene* scene;
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

#define GetNodeContents(node, type) node->contents.type

#define CreateShaderNode(name)                                       \
    __CreateNode(shader, name, LoadShader(name))
#define CreateTextureNode(name, swidth, sheight)                     \
    __CreateNode(texture, name,                                      \
                 CreateTexture(name, tileset, swidth, sheight))
#define RegisterTextureNode(name, from, x, y, z, scale, brightness,  \
                            rotation)                                \
    __CreateNode(                                                    \
        instance, name,                                              \
        RegisterTexture(from, x, y, z, scale, brightness, rotation))

// stupid fucking solution
Node* __CreateNode(NodeType type, const char* name, void* contents);

//??!!!! brother why am i take both type and node?? type is stored in
// node?? fix
LinkedList* CreateLinkedList(Node* head);

#define KillNode(node)                                               \
    __TYPE_SWITCH(                                                   \
        current_node->type,                                          \
        KillShader(current_node->contents.shader),                   \
        KillTexture(current_node->contents.texture),                 \
        DeregisterTexture(current_node->contents.instance),          \
        KillScene(current_node->contents.scene))

void KillLinkedList(LinkedList* list);

__BOOLEAN VerifyNodeContents(NodeType type, NodeContents* contents);

void AppendNode(LinkedList* list, Node* node);
void InsertNode(LinkedList* list, Node* node);

Node* GetNode(LinkedList* list, const char* name);

#endif // _RENAI_LINKED_LIST_
