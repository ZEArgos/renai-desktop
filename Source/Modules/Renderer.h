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
#include "Shader.h"

typedef struct Renderer
{
    u32* shader_list_head;
} Renderer;

Renderer* InitializeRenderer(void);

#endif
