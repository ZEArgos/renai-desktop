/**
 * @file Manager.h
 * @author Zenais Argos
 * @brief A file containing the data structures and files required to
 * utilize the scene manager.
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _RENAI_MANAGER_
#define _RENAI_MANAGER_

#include <Declarations.h>
#include <LinkedList.h>
#include <Scene.h>
#include <Texture.h>

typedef struct SceneManager
{
    char* current_scene;
    LinkedList* scene_list;
} SceneManager;

__CREATE_STRUCT(SceneManager)
CreateManager(f32 window_width, f32 window_height);

__INLINE void KillManager(SceneManager* manager)
{
    KillLinkedList(manager->scene_list);
    __FREE(manager,
           ("The scene manager freer was given an invalid value."));
    PrintWarning("The scene manager was freed.");
}

void RenderCurrentScene(SceneManager* manager, Shader* basic_shader);

#endif // _RENAI_MANAGER_
