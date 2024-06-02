#version 330 core
layout (location = 0) in vec3 position_values;
layout (location = 1) in vec3 color_values;
layout (location = 2) in vec2 texture_coordinates;

out vec3 in_color;
out vec2 in_texture_coordinates;

void main()
{
	gl_Position = vec4(position_values, 1.0);

	in_color = color_values;
	in_texture_coordinates = vec2(texture_coordinates.x, texture_coordinates.y);
}