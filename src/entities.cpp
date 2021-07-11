#include "entities.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "tools/utility.hpp"

// Base class Entity definitions

void Entity::Entity::rotate(float angle, glm::vec3 axis)
{
	glm::quat newQuat = glm::angleAxis(angle, axis);
	rotation = glm::normalize(newQuat * rotation);
}

void Entity::Entity::move(glm::vec3 vec)
{
	position += vec;
}

void Entity::Entity::scale(float diff)
{
	if (size + diff > 0)
	{
		size += diff;
	}
}

Cube::Cube::Cube(float radius, glm::vec3 position, glm::vec3 eulerOrientation)
{
	this->position = position;
	glm::quat rotation(eulerOrientation);
	this->rotation = rotation;
	this->size = radius;
}

glm::mat4 Cube::Cube::genModelMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size));

	return translationMatrix * rotationMatrix * scaleMatrix;
}