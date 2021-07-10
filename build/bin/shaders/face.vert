#version 330 core

uniform mat4 MVP;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

flat out vec3 Color;

void main()
{
	Color = color;

	vec4 pos = vec4(position, 1.0);

	gl_Position = MVP * pos;
}
