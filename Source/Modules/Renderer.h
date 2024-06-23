/**
 * @file Renderer.h
 * @author Zenais Argos
 * @brief Provides the functionality needed to render objects / images onto the
 * window.
 * @date 2024-05-27
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_RENDERER_
#define _RENAI_RENDERER_

#include <LinkedList.h>

typedef struct Renderer
{
    LinkedList* shader_list;
    LinkedList* texture_list;
} Renderer;

#define GetRendererList(renderer, list) Get##list##List(renderer)
__INLINE LinkedList* GetTextureList(Renderer* renderer)
{
    return renderer->texture_list;
}
__INLINE LinkedList* GetShaderList(Renderer* renderer)
{
    return renderer->shader_list;
}

#define GetRendererHead(renderer, list) Get##list##ListHead(renderer)
__INLINE Node* GetTextureListHead(Renderer* renderer)
{
    return renderer->texture_list->first_node;
}
__INLINE Node* GetShaderListHead(Renderer* renderer)
{
    return renderer->shader_list->first_node;
}

#define GetNodeContents(node, type) (*Get##type##Contents(node))
__INLINE Texture* GetTextureContents(Node* node)
{
    return &node->contents.texture;
}
__INLINE u32* GetShaderContents(Node* node) { return &node->contents.shader; }

__CREATE_STRUCT_KILLFAIL(Renderer)
CreateRenderer(f32 width, f32 height, const char* caller);

void KillRenderer(Renderer* renderer);

__BOOLEAN CheckRendererValidity(Renderer* renderer, const char* caller);

void RenderWindowContent(Renderer* renderer);

#endif // _RENAI_RENDERER_
