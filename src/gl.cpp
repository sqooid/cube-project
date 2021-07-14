#include "gl.hpp"

#define glCheckError() glCheckError_(__FILE__, __LINE__)

void loadData(Glid &glid, State state)
{
	const int vaoCount = 3;
	for (int i = 0; i < vaoCount; i++)
	{
		glid.vao.push_back(0);
	}
	glGenVertexArrays(vaoCount, &glid.vao[0]);
	// Loading cube data

	// Face data [0]
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

	// Edge data [1]
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

	// Loading floor data [2]
	glBindVertexArray(glid.vao[2]);
	GLuint floorBuffer;
	glGenBuffers(1, &floorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, floorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * state.floor.vertices.size(), &state.floor.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glCheckError();

	// Unbinding all vertex arrays and buffers just in case
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
	glGetProgramiv(edgeProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "Fucking problemo with edge program (1)\n";
	}
	glid.shader.push_back(edgeProgram);

	// Floor shader [2]
	GLuint floorProgram = LoadShader("shaders/floor.vert", "shaders/floor.frag");
	glLinkProgram(floorProgram);
	glGetProgramiv(floorProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "Fucking problemo with edge program (1)\n";
	}
	glid.shader.push_back(floorProgram);
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
	glm::vec3 eye = state.player.position + glm::vec3(0.0, state.player.height, 0.0);
	glm::mat4 cameraMatrix = glm::lookAt(
		eye,
		eye + state.player.faceDirection,
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 MVP = projectionMatrix * cameraMatrix * modelMatrix;

	// Rendering faces
	glBindVertexArray(glid.vao[0]);
	glUseProgram(glid.shader[0]);
	glUniformMatrix4fv(glid.uniform[0], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glid.uniform[1], 1, GL_FALSE, &modelMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Rendering edges
	glBindVertexArray(glid.vao[1]);
	glUseProgram(glid.shader[1]);
	glUniformMatrix4fv(glid.uniform[2], 1, GL_FALSE, &MVP[0][0]);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

	// Rendering floor
	// Calculating floor offset
	float xOffset = (float)((int)state.player.position.x / state.floor.spacing) * state.floor.spacing;
	float zOffset = (float)((int)state.player.position.z / state.floor.spacing) * state.floor.spacing;

	// std::cout << state.player.position.x << "\n";
	std::cout << state.floor.spacing << "\n";

	std::vector<float> xzOffset{xOffset, zOffset};
	glBindVertexArray(glid.vao[2]);
	glUseProgram(glid.shader[2]);
	glUniform2fv(glid.uniform[4], 1, &xzOffset[0]);
	// Floor transformation matrix
	glm::mat4 VP = projectionMatrix * cameraMatrix;
	glUniformMatrix4fv(glid.uniform[3], 1, GL_FALSE, &VP[0][0]);
	glDrawArrays(GL_LINES, 0, state.floor.vertices.size());

	glBindVertexArray(0);
	glUseProgram(0);
}

void initializeUniforms(Glid &glid)
{
	// Finding uniforms
	glid.uniform.push_back(glGetUniformLocation(glid.shader[0], "MVP"));
	glid.uniform.push_back(glGetUniformLocation(glid.shader[0], "Model"));
	glid.uniform.push_back(glGetUniformLocation(glid.shader[1], "MVP"));
	glid.uniform.push_back(glGetUniformLocation(glid.shader[2], "MVP"));
	glid.uniform.push_back(glGetUniformLocation(glid.shader[2], "xzOffset"));
}