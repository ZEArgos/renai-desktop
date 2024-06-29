#include "Manager.h"

__CREATE_STRUCT(SceneManager)
CreateManager(Texture* missing_texture)
{
    SceneManager* manager = __MALLOC(
        SceneManager, manager,
        ("Failed to allocate space for the scene manager. Code: %d.",
         errno));
    PrintSuccess("Allocated space for the scene manager: %d bytes.",
                 sizeof(SceneManager));

    manager->registered_textures = CreateLinkedList(
        RegisterTextureNode("missing_texture_tile", missing_texture,
                            0.0f, 0.0f, 0, 1, 1.0f, 0.0f));

    return manager;
}
