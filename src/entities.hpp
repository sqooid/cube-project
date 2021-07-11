#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H

#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace Entity
{
    class Entity
    {
    public:
        glm::vec3 position;
        glm::quat rotation;
        float size;

        void rotate(float angle, glm::vec3 axis);
        void move(glm::vec3 vec);
        void scale(float diff);
    };
}

namespace Cube
{
    static std::vector<GLfloat> vertices{
        -1.0, -1.0, 1.0,  //0
        1.0, -1.0, 1.0,   //1
        -1.0, 1.0, 1.0,   //2
        1.0, 1.0, 1.0,    //3
        -1.0, -1.0, -1.0, //4
        1.0, -1.0, -1.0,  //3
        -1.0, 1.0, -1.0,  //6
        1.0, 1.0, -1.0,   //7
    };

    static std::vector<GLuint> faceIndices{
        //Top
        2, 7, 6,
        2, 3, 7,
        //Bottom
        5, 0, 4,
        5, 1, 0,
        //Left
        0, 2, 6,
        0, 6, 4,
        //Right
        1, 7, 3,
        1, 5, 7,
        //Front
        3, 2, 0,
        3, 0, 1,
        //Back
        4, 6, 7,
        4, 7, 5};

    static std::vector<GLuint> edgeIndices{
        0, 1, 1, 3, 3, 2, 2, 0,
        1, 5, 3, 7, 2, 6, 0, 4,
        4, 5, 5, 7, 7, 6, 6, 4};

    static std::vector<GLfloat> colors{
        0.8, 0.8, 0.8, // Left
        0.8, 0.8, 0.8, // Right
        0.8, 0.8, 0.8, // Top
        0.8, 0.8, 0.8, // Front
        0.8, 0.8, 0.8, // Back
        0.8, 0.8, 0.8, // Bottom
        0.8, 0.8, 0.8, // Placeholder
        0.8, 0.8, 0.8  // Placeholder
    };

    static std::vector<GLfloat> vertexNormals{
        // For use with flat qualifier in shader
        -1.0, 0.0, 0.0, // Left
        1.0, 0.0, 0.0,  // Right
        0.0, 1.0, 0.0,  // Top
        0.0, 0.0, 1.0,  // Front
        0.0, 0.0, -1.0, // Back
        0.0, -1.0, 0.0, // Bottom
        1.0, 0.0, 0.0,  // Placeholder
        1.0, 0.0, 0.0   // Placeholder
    };

    class Cube : public Entity::Entity
    {
    public:
        Cube(float radius, glm::vec3 position, glm::vec3 orientation);

        glm::mat4 genModelMatrix();
    };
}

#endif