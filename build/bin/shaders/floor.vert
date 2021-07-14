#version 330 core

uniform vec2 xzOffset;
uniform mat4 MVP;
layout (location = 0) in vec3 position;

void main()
{
	vec4 pos = vec4(position.x + xzOffset.x, position.y, position.z + xzOffset.y, 1.0);

	gl_Position = MVP * pos;
}
