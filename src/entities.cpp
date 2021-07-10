#include "entities.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "tools/utility.hpp"

Cube::Cube::Cube(float radius, glm::vec3 position, glm::vec3 eulerOrientation)
{
	this->position = position;
	glm::quat rotation(eulerOrientation);
	this->rotation = rotation;
	this->size = radius;
}

void Cube::Cube::rotate(float angle, glm::vec3 axis)
{
	glm::quat newQuat = glm::angleAxis(angle, axis);
	rotation = glm::normalize(newQuat * rotation);
}

void Cube::Cube::move(glm::vec3 vec)
{
	position += vec;
}

void Cube::Cube::scale(float diff)
{
	if (size + diff > 0)
	{
		size += diff;
	}
}

glm::mat4 Cube::Cube::genModelMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size));

	return translationMatrix * rotationMatrix * scaleMatrix;
}