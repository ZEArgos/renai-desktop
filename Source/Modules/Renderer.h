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
    cstring name;
    u8 (*UseShader)(struct ShaderNode*);
} ShaderNode;

typedef struct Renderer
{
    ShaderNode* shader_list_head;
    u8 (*RenderWindowContent)(struct Renderer*, f32, f32);
} Renderer;

ShaderNode* GetShader(cstring shader_name);
null InsertShaderNode(ShaderNode* node);
ShaderNode* CreateShaderNode(string shader_name);
u8 InitializeRenderer(f32 swidth, f32 sheight);
null DestroyRenderer(void);

#endif
