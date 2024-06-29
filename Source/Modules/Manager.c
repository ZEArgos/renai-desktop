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

    manager->scene_list =
        CreateLinkedList(CreateSceneNode(missing_texture));

    manager->current_scene = "test_scene";

    return manager;
}

void RenderCurrentScene(SceneManager* manager, Shader* basic_shader)
{
    Scene* current_scene =
        GetNode(manager->scene_list, manager->current_scene)
            ->contents.scene;

    // Bind the "missing" texture to render as a placeholder.
    TextureInstance* missing_texture =
        current_scene->scene_contents->first_node->contents.instance;
    BindTexture(missing_texture->inherits);

    // Position transform the bound texture so it's within our
    // viewport.
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
    SetMat4(basic_shader->shader, "model", model);

    // Draw the texture.
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
