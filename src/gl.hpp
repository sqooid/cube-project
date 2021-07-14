#pragma once
#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#include "entities.hpp"
#include "tools/utility.hpp"
#include "tools/GLShader.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

void loadData(Glid &glid, State state);
void compileShaders(Glid &glid);
void render(sf::Window &window, Glid glid, State state);
void initializeUniforms(Glid &glid);

#endif