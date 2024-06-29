#include "Scene.h"
#include <LinkedList.h>
#include <stbi/stb_image.h>

#define __READ_STRING(buffer, length, file)                          \
    fread(buffer, length, 1, file);                                  \
    buffer[length] = '\0'

LinkedList* LoadScenes(f32 window_width, f32 window_height)
{
    LinkedList* loaded_scenes = NULL;

    FILE* scene_file = fopen("./Assets/scenes.resource", "rb");
    if (scene_file == NULL)
        PrintError("Renai seems to not have a scene file.");

    u8 header_beginning[2];
    fread(header_beginning, 1, 2, scene_file);
    if (header_beginning[0] != 0xFF || header_beginning[1] != 0x01)
        PrintError("Loaded scene file has been tampered with/is "
                   "malformed. Unable to continue.");

    u8 version[3];
    fread(version, 1, 3, scene_file);
    CheckVersionDifference("scenes", version);

    u16 scene_count;
    fread(&scene_count, 2, 1, scene_file);

    u8 header_end[2];
    fread(header_end, 1, 2, scene_file);
    if (header_end[0] != 0xFF || header_end[1] != 0x02)
        PrintError("Loaded scene file has been tampered with/is "
                   "malformed. Unable to continue.");

    for (u16 scene_index = 0; scene_index < scene_count;
         scene_index++)
    {
        Scene* current_scene =
            __MALLOC(Scene, current_scene,
                     ("Failed to allocate space for a scene."));
        current_scene->scene_contents = NULL;

        u16 scene_data_lengths[3];
        fread(scene_data_lengths, 2, 3, scene_file);

        __READ_STRING(current_scene->name, scene_data_lengths[0],
                      scene_file);
        __READ_STRING(current_scene->description,
                      scene_data_lengths[1], scene_file);

        for (u16 texture_list_index = 0;
             texture_list_index < scene_data_lengths[2];
             texture_list_index++)
        {
            u8 file_name_length;
            fread(&file_name_length, 1, 1, scene_file);

            char file_name[file_name_length];
            __READ_STRING(file_name, file_name_length, scene_file);

            u64 image_size;
            fread(&image_size, 8, 1, scene_file);
            u8 image[image_size];
            fread(image, 1, image_size, scene_file);

            Texture* loaded_texture = CreateTextureFromMemory(
                file_name, image, image_size, tileset, window_width,
                window_height);

            if (current_scene->scene_contents == NULL)
                current_scene->scene_contents = CreateLinkedList(
                    __CreateNode(texture, loaded_texture->name,
                                 loaded_texture));
            else
                AppendNode(current_scene->scene_contents,
                           __CreateNode(texture, loaded_texture->name,
                                        loaded_texture));
            // printf("\n\n");
            // printf("%s\n", GetNode(current_scene->scene_contents,
            //                        "texture_missing.jpg")
            //                    ->name);
            printf("\n\n%s\n",
                   current_scene->scene_contents->first_node->name);
        }
        printf("\n\n%s\n",
               current_scene->scene_contents->first_node->name);

        if (loaded_scenes == NULL)
            loaded_scenes = CreateLinkedList(__CreateNode(
                scene, current_scene->name, current_scene));
        else
            AppendNode(loaded_scenes,
                       __CreateNode(scene, current_scene->name,
                                    current_scene));
    }

    return loaded_scenes;
}

void CreateScenes(char** name_array, char** description_array,
                  char** texture_array, u16 texture_array_length,
                  u16 scene_count)
{
    FILE* created_scene = fopen("./Assets/scenes.resource", "wb");

    u8 header_beginning[5] = {0xFF, 0x01, MAJOR, MINOR, REVIS};
    fwrite(header_beginning, 1, 5, created_scene);
    fwrite(&scene_count, 2, 1, created_scene);

    u8 header_end[2] = {0xFF, 0x02};
    fwrite(&header_end, 1, 2, created_scene);

    for (u16 scene_list_index = 0; scene_list_index < scene_count;
         scene_list_index++)
    {
        const char *scene_name = name_array[scene_list_index],
                   *scene_description =
                       description_array[scene_list_index];
        u16 scene_lengths[3] = {strlen(scene_name),
                                strlen(scene_description),
                                texture_array_length};
        fwrite(&scene_lengths, 2, 3, created_scene);
        fprintf(created_scene, "%s%s", scene_name, scene_description);

        for (u16 texture_list_index = 0;
             texture_list_index < texture_array_length;
             texture_list_index++)
        {
            const char* file_name = texture_array[texture_list_index];
            u8 file_name_length = strlen(file_name);

            fwrite(&file_name_length, 1, 1, created_scene);
            fprintf(created_scene, "%s", file_name);

            char file_path[128];
            snprintf(file_path, 128, "./Assets/Tilesets/%s",
                     file_name);

            FILE* image_file = fopen(file_path, "rb");
            fseek(image_file, 0, SEEK_END);
            u64 image_size = ftell(image_file);
            fseek(image_file, 0, SEEK_SET);

            u8* image_contents = malloc(image_size);
            fread(image_contents, image_size, 1, image_file);
            fclose(image_file);

            fwrite(&image_size, 8, 1, created_scene);
            fwrite(image_contents, image_size, 1, created_scene);

            free(image_contents);
        }

        u8 file_end[2] = {0xFF, 0x03};
        fwrite(&file_end, 1, 1, created_scene);

        fclose(created_scene);
    }
}
