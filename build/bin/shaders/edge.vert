#version 330 core

uniform mat4 MVP;
layout (location = 0) in vec3 position;

void main()
{
	vec4 pos = vec4(position, 1.0);

	gl_Position = MVP * pos;
}
