/**
 * @file Application.h
 * @author Zenais Argos
 * @brief Provides the methods required for running Renai's basest functionality
 * easily.
 * @date 2024-05-26
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_APPLICATION_
#define _RENAI_APPLICATION_

// Gimme them typedefs baby
#include "Declarations.h"

u8 InitializeApplication(string title, f32 bgr, f32 bgg, f32 bgb);
u8 RunApplication(void);
null DestroyApplication(void);

#endif // _RENAI_APPLICATION
