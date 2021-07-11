#version 330 core

uniform vec3 lightSourceDirection;

flat in vec3 Color;
flat in vec4 Normal;

out vec4 outColor;

void main()
{


	vec3 normalVector = normalize(vec3(Normal.x, Normal.y, Normal.z));
	// vec3 lightSourceVector = normalize(lightSourceDirection);
	vec3 lightSourceVector = normalize(vec3(0.0, -1.0, -1.0));

	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

	float diff = max(dot(normalVector, -lightSourceVector), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

	vec3 lightResult = (ambient + diffuse) * Color;

	outColor = vec4(lightResult, 1.0);
}
