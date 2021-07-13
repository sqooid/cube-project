#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H

#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <SFML/Graphics.hpp>

/**
 * @brief Enumeration for types of entites
 * 
 */
enum class EntityType
{
    CUBE_E
};

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

namespace Player
{
    class Player : public Entity::Entity
    {
    public:
        Player(float height = 0.0f, glm::vec3 footPosition = glm::vec3(0.0), glm::vec3 orientation = glm::vec3(0.0, 0.0, -1.0));
        void rotate(float angle, glm::vec3 axis);

        float height;
        float fov;
        glm::vec3 faceDirection;
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
        /**
         * @brief Construct a new Player object
         * 
         * @param height Height of the player camera from foot
         * @param footPosition 
         * @param facing 
         */
        Cube(float radius = 1.0f, glm::vec3 position = glm::vec3(0.0), glm::vec3 orientation = glm::vec3(0.0, 0.0, 1.0));

        glm::mat4 genModelMatrix();
    };
}

typedef struct Control
{
    sf::Vector2i lastLeftClick;
    sf::Vector2i lastRightClick;
    bool windowFocused;
} Control;

typedef struct Glid
{
    Glid()
    {
        std::vector<GLuint> vaoVec;
        this->vao = vaoVec;
        std::vector<GLuint> shaderVec;
        this->shader = shaderVec;
        std::vector<GLint> uniformVec;
        this->uniform = uniformVec;
    }

    std::vector<GLuint> vao;
    std::vector<GLuint> shader;
    std::vector<GLint> uniform;
} Glid;

/**
 * @brief Class containing all data about entities in the world
 * 
 */
class State
{
public:
    State(Player::Player &player, Control &control);

    Player::Player player;
    Control control;
    std::vector<Cube::Cube *> cubeList;

    void addEntity(Cube::Cube &entity);
};

#endif