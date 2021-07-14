#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <SFML/Graphics.hpp>
#include "entities.hpp"

void tick(sf::Window &window, State &state, double t);
void handleEvents(sf::Window &window, sf::Event event, State state);

#endif