#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H

#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>

class Cube
{
	public:
        Cube(float radius);

        std::vector<GLfloat> vertices;
        std::vector<GLuint> faceIndices;
        std::vector<GLuint> edgeIndices;
        std::vector<GLfloat> colors;
        glm::vec3 position;
        glm::quat rotation;
        float size;

        void rotate(float angle, glm::vec3 axis);
        void move(glm::vec3 vec);
        void scale(float diff);

        glm::mat4 genModelMatrix();
};

#endif