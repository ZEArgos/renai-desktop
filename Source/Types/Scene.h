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
    const char* name;
} Scene;

Scene* TEST_SCENE(Texture* missing_texture);

/**
 * @brief Since we don't ship the program with scene files, we have to
 * generate them on first startup for use in the future. This takes in
 * a scene program and writes it to a binary format in the @dir
 * ./Assets/Scenes/[NAME].scene .
 * @return true The operation was a success.
 * @return false The operation failed.
 */
bool CreateScene(void);

Scene* LoadScene(const char* name);

__INLINE void KillScene(Scene* scene)
{
    const char* name = scene->name;
    __FREE(scene, ("The scene freer was given an invalid scene."));
    PrintWarning("Freed scene '%s'.", name);
}

#endif // _RENAI_SCENE_
