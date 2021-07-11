#version 330 core

uniform mat4 MVP;
uniform mat4 Model;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

flat out vec3 Color;
flat out vec4 Normal;

void main()
{
	Color = color;

	vec4 normalVec = vec4(normal, 0.0);
	vec4 pos = vec4(position, 1.0);

	Normal = Model * normalVec;
	gl_Position = MVP * pos;
}
