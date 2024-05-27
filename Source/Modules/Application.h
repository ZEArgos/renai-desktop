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

/**
 * @brief Initialize the application class and start the game's functionality
 * loop. The game doesn't run until @ref RunApplication is called, but this sets
 * everything up to run. The function kills the application on failure.
 * @param title The application's title.
 * @param bgr The red value of the background.
 * @param bgg The green value of the background.
 * @param bgb The blue value of the background.
 */
null InitializeApplication(string title, f32 bgr, f32 bgg, f32 bgb);
u8 RunApplication(void);
null DestroyApplication(void);

#endif // _RENAI_APPLICATION
