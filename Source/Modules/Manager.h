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
#include <Texture.h>

typedef struct SceneManager
{
    LinkedList* registered_textures;
} SceneManager;

__CREATE_STRUCT(SceneManager)
CreateManager(Texture* missing_texture);

__INLINE void KillManager(SceneManager* manager)
{
    KillLinkedList(manager->registered_textures);
    __FREE(manager,
           ("The scene manager freer was given an invalid value."));
    PrintWarning("The scene manager was freed.");
}

#endif // _RENAI_MANAGER_
