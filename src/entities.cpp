#include "entities.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "tools/utility.hpp"

/**
 * Game state object definitions
 */
State::State(Player::Player &player, Control &control)
{
	this->player = player;
	this->control = control;
	std::vector<Cube::Cube *> cubeVector;
	this->cubeList = cubeVector;
}

void State::addEntity(Cube::Cube &entity)
{
	this->cubeList.push_back(&entity);
}

/**
 * Base class Entity definitions
 * */

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

/**
 * Player class memeber definitions
 * */

/**
 * @brief Construct a new Player object
 * 
 * @param height Height of the player camera from foot
 * @param footPosition 
 * @param facing 
 */
Player::Player::Player(float height, glm::vec3 footPosition, glm::vec3 facing)
{
	this->height = height;
	this->position = footPosition;
	this->faceDirection = facing;
	this->fov = 90.0f;
}

void Player::Player::rotate(float angle, glm::vec3 axis)
{
	glm::quat newQuat = glm::angleAxis(angle, glm::normalize(axis));
	glm::mat4 rotationMatrix = glm::toMat4(newQuat);
	this->faceDirection = rotationMatrix * glm::vec4(this->faceDirection, 0.0);
}

/**
 * Cube class member definitions
 * */

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