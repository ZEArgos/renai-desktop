#include "Scene.h"
#include <LinkedList.h>

Scene* TEST_SCENE(Texture* missing_texture)
{
    Scene* temp = __MALLOC(Scene, temp, ("err err"));
    temp->name = "test_scene";
    temp->scene_contents = CreateLinkedList(
        RegisterTextureNode("missing_texture", missing_texture, 0.0f,
                            0.0f, 0, 1, 1.0f, 0.0f));

    // printf("\n\n%hhu\n\n", temp->scene_contents[0]->scale);
    // exit(0);

    return temp;
}

Scene* LoadScene(const char* name)
{
    Scene* loaded_scene = __MALLOC(Scene, loaded_scene,
                                   ("Failed to allocate enough space "
                                    "for the scene '%s'. Code: %d.",
                                    name, errno));

    return loaded_scene;
}
