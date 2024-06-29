/**
 * @file Scene.h
 * @author Zenais Argos
 * @brief Provides all the data structuring and helper functions to
 * deal with scenes--and their rendering.
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_SCENE_
#define _RENAI_SCENE_

#include <Declarations.h>
#include <Texture.h>

typedef struct LinkedList LinkedList;
typedef struct Scene
{
    LinkedList* scene_contents;
    char name[32], description[64];
} Scene;

/**
 * @brief Since we don't ship the program with scene files, we have to
 * generate them on first startup for use in the future. This takes in
 * a scene program and writes it to a binary format in the file @dir
 * ./Assets/scenes.resource .
 */
void CreateScenes(char** name_array, char** description_array,
                  char** texture_array, u16 texture_array_length,
                  u16 scene_count);

LinkedList* LoadScenes(f32 window_width, f32 window_height);

__INLINE void KillScene(Scene* scene)
{
    const char* name = scene->name;
    __FREE(scene, ("The scene freer was given an invalid scene."));
    PrintWarning("Freed scene '%s'.", name);
}

#endif // _RENAI_SCENE_
