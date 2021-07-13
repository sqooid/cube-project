#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <tools/GLShader.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <cstring>
#include <iostream>
#include "entities.hpp"
#include "tools/utility.hpp"

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void initialize()
{
	// Setting window settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// glEnable(GL_BLEND);

	// OpenGL version printing
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Vendor            : %s\n", vendor);
	printf("GL Renderer          : %s\n", renderer);
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);

	glewInit();
}

void loadData(Glid &glid)
{
	const int vaoCount = 2;
	for (int i = 0; i < vaoCount; i++)
	{
		glid.vao.push_back(0);
	}
	glGenVertexArrays(vaoCount, &glid.vao[0]);
	// Loading cube data
	glBindVertexArray(glid.vao[0]);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Cube::vertices.size(), &(Cube::vertices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glCheckError();

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Cube::vertexNormals.size(), &Cube::vertexNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glCheckError();

	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Cube::colors.size(), &(Cube::colors[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glCheckError();

	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Cube::faceIndices.size(), &(Cube::faceIndices[0]), GL_STATIC_DRAW);
	glCheckError();

	// Edges
	// Rebinding vertices
	glBindVertexArray(glid.vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glCheckError();

	GLuint edgeIndexBuffer;
	glGenBuffers(1, &edgeIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Cube::edgeIndices.size(), &(Cube::edgeIndices[0]), GL_STATIC_DRAW);
	glCheckError();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void compileShaders(Glid &glid)
{
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	// Creating shader programs
	// Face shader [0]
	GLuint faceProgram = LoadShader("shaders/face.vert", "shaders/face.frag");
	glLinkProgram(faceProgram);
	GLint status;
	glGetProgramiv(faceProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "Fucking problemo with face program (1)\n";
	}
	glid.shader.push_back(faceProgram);

	// Edge shader [1]
	GLuint edgeProgram = LoadShader("shaders/edge.vert", "shaders/edge.frag");
	glLinkProgram(edgeProgram);
	glGetProgramiv(faceProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "Fucking problemo with edge program (1)\n";
	}
	glid.shader.push_back(edgeProgram);
}

/**
 * @brief Function that handles all rendering given openGL id's and game state
 * 
 * @param glid 
 * @param state 
 */
void render(sf::Window &window, Glid glid, State state)
{
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setting uniforms
	glm::mat4 modelMatrix = state.cubeList[0]->genModelMatrix();
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(state.player.fov),
		(float)window.getSize().x / window.getSize().y,
		0.1f,
		100.0f);

	// Setting view matrix
	glm::vec3 eye = state.player.position + glm::vec3(0.0, 0.0, state.player.height);
	glm::mat4 cameraMatrix = glm::lookAt(
		eye,
		eye + state.player.faceDirection,
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 MVP = projectionMatrix * cameraMatrix * modelMatrix;

	// std::cout << "Printing matrices"
	// 		  << "\n";
	// printMatrix(modelMatrix);
	// printMatrix(projectionMatrix);
	// printMatrix(cameraMatrix);
	// printMatrix(MVP);

	glUseProgram(glid.shader[0]);
	glUniformMatrix4fv(glid.uniform[0], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glid.uniform[1], 1, GL_FALSE, &modelMatrix[0][0]);
	glUseProgram(glid.shader[1]);
	glUniformMatrix4fv(glid.uniform[2], 1, GL_FALSE, &MVP[0][0]);

	// Rendering faces
	glBindVertexArray(glid.vao[0]);
	glUseProgram(glid.shader[0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Rendering edges
	glBindVertexArray(glid.vao[1]);
	glUseProgram(glid.shader[1]);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void initializeUniforms(std::vector<GLuint> shaders)
{
}

State tick(sf::Window &window, State state)
{
	// Control sensitivities
	float rotationSpeed = 300.0;
	float movementSpeed = 0.01;

	sf::Vector2i currPosition = sf::Mouse::getPosition(window);
	// Left click drag for rotation
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (state.control.lastLeftClick == sf::Vector2i(-1, -1) || state.control.lastLeftClick == currPosition)
		{
			state.control.lastLeftClick = currPosition;
		}
		else
		{
			sf::Vector2i direction = currPosition - state.control.lastLeftClick;
			glm::vec3 zPos(0.0, 0.0, 1.0);
			glm::vec3 rotationAxis = glm::cross(zPos, glm::vec3(direction.x, -direction.y, 0.0));
			float distance = glm::length(rotationAxis) / (float)window.getSize().x * rotationSpeed;
			std::cout << "Angle: " << distance << "\n";

			rotationAxis = rotationAxis / distance;
			state.cubeList[0]->rotate(glm::radians(distance), glm::normalize(rotationAxis));
			state.control.lastLeftClick = currPosition;
		}
	}
	else
	{
		state.control.lastLeftClick = sf::Vector2i(-1, -1);
	}

	// Right click for movement
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (state.control.lastRightClick == sf::Vector2i(-1, -1) || state.control.lastRightClick == currPosition)
		{
			state.control.lastRightClick = currPosition;
		}
		else
		{
			sf::Vector2i direction = currPosition - state.control.lastRightClick;
			glm::vec3 directionVec(direction.x, -direction.y, 0.0);
			state.cubeList[0]->move(directionVec * movementSpeed);

			state.control.lastRightClick = currPosition;
		}
	}
	else
	{
		state.control.lastRightClick = sf::Vector2i(-1, -1);
	}

	return state;
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

int main()
{
	// Getting Depth Buffer to work
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	// Settings.stencilBits       = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2; // Request 2 levels of antialiasing

	sf::Window window(sf::VideoMode(800, 600), "3D Shenanigans", sf::Style::Default, Settings);
	window.setActive(true);
	sf::Event event;

	initialize();

	// Render ID struct initilization
	Glid glid;

	// Loading shaders
	compileShaders(glid);

	// Initialization of entities
	// Player initialization
	Player::Player player(1.0, glm::vec3(0.0), glm::vec3(0.0, 0.0, -1.0));

	// Instantiating control state
	Control controlState{
		sf::Vector2i(-1, -1),
		sf::Vector2i(-1, -1),
		true};

	// Creating game state
	State state(player, controlState);

	// Making a cube
	Cube::Cube cube1(0.5f, glm::vec3(0.0, 0.0, -2.0), glm::vec3(0.0, 0.0, 1.0));
	// Adding cube
	state.addEntity(cube1);
	std::cout << "cube list size" << state.cubeList.size() << "\n";

	// Creating vao's
	loadData(glid);

	// Finding uniforms
	glid.uniform.push_back(glGetUniformLocation(glid.shader[0], "MVP"));
	glid.uniform.push_back(glGetUniformLocation(glid.shader[0], "Model"));
	glid.uniform.push_back(glGetUniformLocation(glid.shader[1], "MVP"));
	std::cout << "uniform id's" << glid.uniform[0] << ", " << glid.uniform[1] << ", " << glid.uniform[2] << "\n";

	// Clock
	sf::Clock clock;
	double t = 0.0;
	double dt = 1.0 / 144.0;
	double accumulator = 0.0;

	while (window.isOpen())
	{
		double frameTime = clock.getElapsedTime().asSeconds();
		accumulator += frameTime;
		clock.restart();
		while (accumulator >= dt)
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::LostFocus)
				{
					state.control.windowFocused = false;
				}
				else if (event.type == sf::Event::GainedFocus)
				{
					state.control.windowFocused = true;
				}
				if (state.control.windowFocused)
				{
					handleEvents(window, event, state);
				}
			}
			// Physics and controls
			state = tick(window, state);

			t += dt;
			accumulator -= dt;
		}

		// Render
		render(window, glid, state);
		window.display();
	}
}