#version 330 core
out vec4 fragment_color;

in vec3 in_color;
in vec2 in_texture_coordinates;

uniform sampler2D in_texture;

void main()
{
	fragment_color = texture(in_texture, in_texture_coordinates);
}