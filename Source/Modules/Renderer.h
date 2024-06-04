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

// Mmm typedefs
#include "Declarations.h"

typedef struct ShaderNode
{
    struct ShaderNode* next;
    u32 shader;
    const char* name;
    u8 (*UseShader)(struct ShaderNode*);
} ShaderNode;

typedef struct Renderer
{
    ShaderNode* shader_list_head;
} Renderer;

ShaderNode* GetShader(const char* shader_name);
void InsertShaderNode(ShaderNode* node);
ShaderNode* CreateShaderNode(char* shader_name);
u8 InitializeRenderer(void);
void DestroyRenderer(void);
void RenderWindowContent(Renderer* renderer);

#endif
