#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H

#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>

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
        1.0, 1.0, 1.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 0.0,
        1.0, 0.0, 1.0,
        0.0, 1.0, 1.0,
        0.1, 0.1, 0.1};

    class Cube
    {
    public:
        Cube(float radius, glm::vec3 position, glm::vec3 orientation);

        glm::vec3 position;
        glm::quat rotation;
        float size;

        void rotate(float angle, glm::vec3 axis);
        void move(glm::vec3 vec);
        void scale(float diff);

        glm::mat4 genModelMatrix();
    };
}

#endif