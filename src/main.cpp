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
#include "gl.hpp"
#include "game.hpp"

void initialize()
{
	// Setting window settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);
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

int main()
{
	// Getting Depth Buffer to work
	sf::ContextSettings Settings;
	Settings.depthBits = 24;		// Request a 24 bits depth buffer
	Settings.antialiasingLevel = 2; // Request 2 levels of antialiasing

	sf::Window window(sf::VideoMode(1920, 1080), "3D Shenanigans", sf::Style::Default, Settings);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);
	window.setActive(true);
	sf::Event event;

	initialize();

	// Render ID struct initilization
	Glid glid;

	// Loading shaders
	compileShaders(glid);
	initializeUniforms(glid);

	// Initialization of entities
	// Player initialization
	Player::Player player(1.0, glm::vec3(0.0), glm::vec3(0.0, 0.0, -1.0));
	Floor::Floor floor(1.0, 100.0, 0.0);

	// Instantiating control state
	Control controlState{
		true,
		0.0,
		0.0,
		0.0};

	// Creating game state
	State state(player, controlState, floor);

	// Making a cube
	Cube::Cube cube1(0.5f, glm::vec3(0.0, 1.0, -2.0), glm::vec3(5.0, 3.0, 1.0));
	// Adding cube
	state.addEntity(cube1);

	// Creating vao's
	loadData(glid, state);

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
					state.control.bWindowFocused = false;
				}
				else if (event.type == sf::Event::GainedFocus)
				{
					state.control.bWindowFocused = true;
				}
				if (state.control.bWindowFocused)
				{
					handleEvents(window, event, state);
				}
			}
			// Physics and controls
			if (state.control.bWindowFocused)
			{
				tick(window, state, t);
			}

			t += dt;
			accumulator -= dt;
		}

		// Render
		render(window, glid, state);
		window.display();
	}
}