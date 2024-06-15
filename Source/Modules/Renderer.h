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

//! make these into inlines
#define RendererTextureList renderer->texture_list
#define RendererShaderList  renderer->shader_list
#define RendererTextureListHead                                                \
    renderer->texture_list->first_node->texture_contents
#define RendererShaderListHead                                                 \
    renderer->shader_list->first_node->shader_contents
#define RendererTextureListHeadNode renderer->texture_list->first_node
#define RendererShaderListHeadNode  renderer->shader_list->first_node

typedef struct Renderer
{

    LinkedList* shader_list;

    LinkedList* texture_list;
} Renderer;

__CREATE_STRUCT_KILLFAIL(Renderer)
CreateRenderer(f32 width, f32 height, const char* caller);

void KillRenderer(Renderer* renderer);

u8 CheckRendererValidity(Renderer* renderer, const char* caller);

void RenderWindowContent(Renderer* renderer);

#endif // _RENAI_RENDERER_
