#include "game.hpp"

void tick(sf::Window &window, State &state, double t)
{
	// Control sensitivities
	float rotationSpeed = 300.0;
	float movementSpeed = 0.01;
	float playerMovementSpeed = 0.05;
	float cameraSensitivity = 0.001;

	// Values
	int windowXCenter = window.getSize().x / 2;
	int windowYCenter = window.getSize().y / 2;

	// Handling mouse movement (camera movement)
	sf::Vector2i currPosition = sf::Mouse::getPosition(window);
	int xDelta = windowXCenter - currPosition.x;
	int yDelta = windowYCenter - currPosition.y;
	if (xDelta != 0 || yDelta != 0)
	{
		sf::Mouse::setPosition(sf::Vector2i(windowXCenter, windowYCenter), window);

		// Rotation axes
		const glm::vec3 yawAxis(0.0, 1.0, 0.0);
		const glm::vec3 pitchAxis = glm::cross(glm::normalize(glm::vec3(state.player.faceDirection.x, 0.0, state.player.faceDirection.z)), yawAxis);

		glm::quat yawQuat(1.0, 0.0, 0.0, 0.0);
		if (xDelta != 0)
		{
			yawQuat = glm::angleAxis((float)xDelta * cameraSensitivity, yawAxis);
		}
		glm::quat pitchQuat(1.0, 0.0, 0.0, 0.0);
		if (yDelta != 0 && (yDelta * state.player.faceDirection.y < 0 || std::acos(std::abs(state.player.faceDirection.y)) - 0.02 > std::abs(yDelta * cameraSensitivity)))
		{
			pitchQuat = glm::angleAxis((float)yDelta * cameraSensitivity, pitchAxis);
		}

		glm::mat4 yawMatrix = glm::toMat4(yawQuat);
		glm::mat4 pitchMatrix = glm::toMat4(pitchQuat);

		glm::vec4 cameraDirection = glm::vec4(state.player.faceDirection, 0.0);
		cameraDirection = pitchMatrix * yawMatrix * cameraDirection;
		state.player.faceDirection = cameraDirection;
	}

	// Left click drag for rotation
	// if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	// {
	// 	if (state.control.lastLeftClick == sf::Vector2i(-1, -1) || state.control.lastLeftClick == currPosition)
	// 	{
	// 		state.control.lastLeftClick = currPosition;
	// 	}
	// 	else
	// 	{
	// 		sf::Vector2i direction = currPosition - state.control.lastLeftClick;
	// 		glm::vec3 zPos(0.0, 0.0, 1.0);
	// 		glm::vec3 rotationAxis = glm::cross(zPos, glm::vec3(direction.x, -direction.y, 0.0));
	// 		float distance = glm::length(rotationAxis) / (float)window.getSize().x * rotationSpeed;
	// 		// std::cout << "Angle: " << distance << "\n";

	// 		rotationAxis = rotationAxis / distance;
	// 		state.cubeList[0]->rotate(glm::radians(distance), glm::normalize(rotationAxis));
	// 		state.control.lastLeftClick = currPosition;
	// 	}
	// }
	// else
	// {
	// 	state.control.lastLeftClick = sf::Vector2i(-1, -1);
	// }

	// // Right click for object movement
	// if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	// {
	// 	if (state.control.lastRightClick == sf::Vector2i(-1, -1) || state.control.lastRightClick == currPosition)
	// 	{
	// 		state.control.lastRightClick = currPosition;
	// 	}
	// 	else
	// 	{
	// 		sf::Vector2i direction = currPosition - state.control.lastRightClick;
	// 		glm::vec3 directionVec(direction.x, -direction.y, 0.0);
	// 		state.cubeList[0]->move(directionVec * movementSpeed);

	// 		state.control.lastRightClick = currPosition;
	// 	}
	// }
	// else
	// {
	// 	state.control.lastRightClick = sf::Vector2i(-1, -1);
	// }

	//
	// Keyboard controls
	//

	float xComponent = state.player.faceDirection.x;
	float zComponent = state.player.faceDirection.z;
	glm::vec2 sumDirection(0.0);

	bool bWPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool bSPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool bAPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool bDPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool bMoved = bWPressed || bSPressed || bAPressed || bDPressed;

	//
	// Forwards and backwards movement
	//

	if (bMoved)
	{
		if (bWPressed || bSPressed)
		{
			if (bWPressed && state.control.wPressTime == 0.0)
			{
				state.control.wPressTime = t;
			}
			else if (!bWPressed)
			{
				state.control.wPressTime = 0.0;
			}

			if (bSPressed && state.control.sPressTime == 0.0)
			{
				state.control.sPressTime = t;
			}
			else if (!bSPressed)
			{
				state.control.sPressTime = 0.0;
			}
			if (state.control.wPressTime > state.control.sPressTime)
			{
				sumDirection += glm::vec2(xComponent, zComponent);
			}
			else
			{
				sumDirection -= glm::vec2(xComponent, zComponent);
			}
		}

		//
		// Sideways movement
		//

		if (bAPressed || bDPressed)
		{
			if (bAPressed && state.control.aPressTime == 0.0)
			{
				state.control.aPressTime = t;
			}
			else if (!bAPressed)
			{
				state.control.aPressTime = 0.0;
			}
			if (bDPressed && state.control.dPressTime == 0.0)
			{
				state.control.dPressTime = t;
			}
			else if (!bDPressed)
			{
				state.control.dPressTime = 0.0;
			}
			if (state.control.aPressTime > state.control.dPressTime)
			{
				sumDirection += glm::vec2(zComponent, -xComponent);
			}
			else
			{
				sumDirection -= glm::vec2(zComponent, -xComponent);
			}
		}

		sumDirection = playerMovementSpeed * glm::normalize(sumDirection);
		state.player.move(glm::vec3(sumDirection.x, 0.0, sumDirection.y));
	}
}

void handleEvents(sf::Window &window, sf::Event event, State state)
{
	// Scale speed
	float zoomSpeed = 0.2;

	// Events
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;

	case sf::Event::Resized:
		glViewport(0, 0, (GLsizei)event.size.width, (GLsizei)event.size.height);
		break;

	case sf::Event::MouseWheelMoved:
		float wheelMovement = event.mouseWheel.delta;
		state.cubeList[0]->move(wheelMovement * glm::vec3(0.0, 0.0, zoomSpeed));
		break;
	}
}