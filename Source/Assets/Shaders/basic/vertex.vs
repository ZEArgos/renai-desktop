#version 330 core
layout (location = 0) in vec3 position_values;
layout (location = 1) in vec2 texture_coordinates;

out vec2 in_texture_coordinates;
uniform mat4 model, projection;

void main()
{
    gl_Position = projection * model * vec4(position_values, 1.0f);
    in_texture_coordinates = vec2(texture_coordinates.x, 1.0 - texture_coordinates.y);
}