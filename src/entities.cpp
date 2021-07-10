#include "entities.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "tools/utility.hpp"


Cube::Cube(float radius)
{
	vertices = std::vector<GLfloat> 
	{
		-radius, -radius,  radius, //0
         radius, -radius,  radius, //1
        -radius,  radius,  radius, //2
         radius,  radius,  radius, //3
        -radius, -radius, -radius, //4
         radius, -radius, -radius, //3
        -radius,  radius, -radius, //6
         radius,  radius, -radius, //7
	};
	faceIndices = std::vector<GLuint>
	{
		//Top
        2, 7, 6,
        2, 3, 7,
        //Bottom
        0, 4, 5,
        0, 5, 1,
        //Left
        6, 0, 2,
        6, 4, 0,
        //Right
        1, 7, 3,
        1, 5, 7,
        //Front
        3, 2, 0,
        3, 0, 1,
        //Back
        4, 6, 7,
        4, 7, 5
	};
	edgeIndices = std::vector<GLuint>
	{
		0, 1, 1, 3, 3, 2, 2, 0,
		1, 5, 3, 7, 2, 6, 0, 4, 
		4, 5, 5, 7, 7, 6, 6, 4
	};
	colors = std::vector<GLfloat>
	{
		1.0, 1.0, 1.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.1, 0.1, 0.1
	};
	position = glm::vec3 (0.0f, 0.0f, 0.0f);
	glm::quat quatInit(1.0, 0.0, 0.0, 0.0);
	rotation = quatInit;
	size = 1.0f;
}

void Cube::rotate(float angle, glm::vec3 axis)
{
	glm::quat newQuat = glm::angleAxis(angle, axis);
	rotation = glm::normalize(newQuat * rotation);
}

void Cube::move(glm::vec3 vec)
{
	position += vec;
}

void Cube::scale(float diff)
{
	if(size + diff > 0)
	{
		size += diff;
	}
}

glm::mat4 Cube::genModelMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size));

	// std::cout << "Quat: " << rotation.x << " " << rotation.y << " " << rotation.z << " " << rotation.w << "\n";

	// printMatrix(translationMatrix);
	// printMatrix(rotationMatrix);
	// printMatrix(scaleMatrix);

	glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	return modelMatrix;
}