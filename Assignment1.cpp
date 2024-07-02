// include C++ headers
#define _USE_MATH_DEFINES
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
//using namespace std;	// to avoid having to use std::

// include OpenGL related headers
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <AntTweakBar.h>

#include "ShaderProgram.h"

// vertex attribute format
struct VertexColor
{
	GLfloat position[3];
	GLfloat color[3];
};


// global variables
// settings
unsigned int gWindowWidth = 1000;
unsigned int gWindowHeight = 1000;
unsigned int vertexBuffer;
unsigned int vertexArray;
const float gTranslateSensitivity = 0.05f;
const float gRotateSensitivity = 0.1f;
const float gScaleSensitivity = 0.05f;
float gFrameRate = 60.0f;
float gFrameTime = 1 / gFrameRate;
float rotateAngle = 0.0f;
float wheelRotate = 0.0f;
float gTruckSpeed = 0.0f;
float gDumpAngle = 0.0f;

// tweak bar elements
bool gWireframe = false;			// wireframe mode on or off
glm::vec3 gBackgroundColor(0.2f);
glm::vec3 dumpBoxPivotPoint(0.7f, -0.2f, 0.0f);


//scene content
ShaderProgram gShader;	// shader program object
GLuint gVBO = 0;		// vertex buffer object identifier
GLuint gVAO = 0;		// vertex array object identifier

std::map<std::string, glm::mat4> gModelMatrix; // object model matrix
glm::mat4 gGroundModelMatrix;	// ground model matrix

//build circle
void buildCircle(float centerX, float centerY, float radius)
{
	const int numSegments = 30;
	const float angleStep = 2 * M_PI / numSegments;

	std::vector<VertexColor> vertices;
	vertices.reserve(numSegments + 1);

	//Center vertex
	VertexColor centerVertex;
	centerVertex.position[0] = centerX;
	centerVertex.position[1] = centerY;
	centerVertex.position[2] = 0.0f;
	centerVertex.color[0] = 0.2f;
	centerVertex.color[1] = 0.2f;
	centerVertex.color[2] = 0.2f;
	vertices.push_back(centerVertex);

	//Outer vertex
	for (int i = 0; i <= numSegments; ++i) {
		float currentAngle = i * angleStep;
		float x = centerX + radius * cos(currentAngle);
		float y = centerY + radius * sin(currentAngle);

		VertexColor vertex;
		vertex.position[0] = x;
		vertex.position[1] = y;
		vertex.position[2] = 0.0f;
		vertex.color[0] = 0.3f;
		vertex.color[1] = 0.3f;
		vertex.color[2] = 0.3f;
		vertices.push_back(vertex);
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), reinterpret_cast<void*>(offsetof(VertexColor, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), reinterpret_cast<void*>(offsetof(VertexColor, color)));

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void buildRim(float centerX, float centerY, float radius) {
	const int numSegments = 30;
	const float angleStep = 2 * M_PI / numSegments;

	std::vector<VertexColor> vertices;
	vertices.reserve(numSegments + 3);

	// Center vertex
	VertexColor centerVertex;
	centerVertex.position[0] = centerX;
	centerVertex.position[1] = centerY;
	centerVertex.position[2] = 0.0f;
	centerVertex.color[0] = 0.8f;
	centerVertex.color[1] = 0.8f;
	centerVertex.color[2] = 0.8f;
	vertices.push_back(centerVertex);

	// Calculate the indices for the lighter segments
	int segmentIndex1 = numSegments / 4; // First lighter segment index
	int segmentIndex2 = (numSegments / 4) + (numSegments / 2); // Second lighter segment index (opposite of the first)

	// Outer vertices
	for (int i = 0; i <= numSegments; ++i)
	{
		float currentAngle = i * angleStep;
		float x = centerX + radius * cos(currentAngle);
		float y = centerY + radius * sin(currentAngle);

		VertexColor vertex;
		vertex.position[0] = x;
		vertex.position[1] = y;
		vertex.position[2] = 0.0f;

		// Set the color based on the segment index
		if (i == segmentIndex1 || i == segmentIndex2)
		{
			// Lighter shade
			vertex.color[0] = 0.6f;
			vertex.color[1] = 0.6f;
			vertex.color[2] = 0.6f;
		}
		else
		{
			// Darker shade
			vertex.color[0] = 0.4f;
			vertex.color[1] = 0.4f;
			vertex.color[2] = 0.4f;
		}

		vertices.push_back(vertex);
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), reinterpret_cast<void*>(offsetof(VertexColor, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), reinterpret_cast<void*>(offsetof(VertexColor, color)));

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void buildGround(float screenWidth)
{
	std::vector<VertexColor> vertices;

	// Define the vertices of the ground
	VertexColor vertex1;
	vertex1.position[0] = -screenWidth / 2.0f;
	vertex1.position[1] = -0.45f;
	vertex1.position[2] = 0.0f;
	vertex1.color[0] = 0.0f;  // Red component
	vertex1.color[1] = 1.0f;  // Green component
	vertex1.color[2] = 0.533f;  // Blue component
	vertices.push_back(vertex1);

	VertexColor vertex2;
	vertex2.position[0] = screenWidth / 2.0f;
	vertex2.position[1] = -0.45f;
	vertex2.position[2] = 0.0f;
	vertex2.color[0] = 0.0f;
	vertex2.color[1] = 1.0f;
	vertex2.color[2] = 0.533f;
	vertices.push_back(vertex2);

	VertexColor vertex3;
	vertex3.position[0] = screenWidth / 2.0f;
	vertex3.position[1] = -1.0f;
	vertex3.position[2] = 0.0f;
	vertex3.color[0] = 0.0f;
	vertex3.color[1] = 0.4f;
	vertex3.color[2] = 0.0f;
	vertices.push_back(vertex3);

	VertexColor vertex4;
	vertex4.position[0] = -screenWidth / 2.0f;
	vertex4.position[1] = -1.0f;
	vertex4.position[2] = 0.0f;
	vertex4.color[0] = 0.0f;
	vertex4.color[1] = 0.4f;
	vertex4.color[2] = 0.0f;
	vertices.push_back(vertex4);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), reinterpret_cast<void*>(offsetof(VertexColor, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), reinterpret_cast<void*>(offsetof(VertexColor, color)));

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set the color the color buffer should be cleared to
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//compile and link a vertex and fragment shader pair
	gShader.compileAndLink("modelTransform.vert", "color.frag");

	//initialise model matrices to identity matrix
	gModelMatrix["Truck"] = glm::mat4(1.0f);
	gModelMatrix["DumpBox"] = glm::mat4(1.0f);
	gModelMatrix["Wheels1"] = glm::mat4(1.0f);
	gModelMatrix["Wheels2"] = glm::mat4(1.0f);

	std::vector<GLfloat> vertices =
	{
		//truck head
		-0.5f, 0.5f, 0.0f,		// vertex 0
		0.0f, 1.0f, 0.0f,
		-0.2f, 0.5f, 0.0f,		// vertex 1
		0.0f, 1.0f, 0.0f,
		-0.7f, 0.2f, 0.0f,		// vertex 2
		0.0f, 1.0f, 0.0f,
		-0.7f, 0.2f, 0.0f,		// vertex 3
		0.0f, 1.0f, 0.0f,
		-0.2f, 0.2f, 0.0f,		// vertex 4
		1.0f, 0.0f, 0.0f,
		-0.2f, 0.5f, 0.0f,		// vertex 5
		0.0f, 1.0f, 0.0f,
		-0.7f, 0.2f, 0.0f,		// vertex 6
		0.0f, 1.0f, 0.0f,
		-0.7f, -0.2f, 0.0f,		// vertex 7
		1.0f, 0.0f, 0.0f,
		-0.2f, 0.2f, 0.0f,		// vertex 8
		1.0f, 0.0f, 0.0f,
		-0.7f, -0.2f, 0.0f,		// vertex 9
		1.0f, 0.0f, 0.0f,
		-0.2f, -0.2f, 0.0f,		// vertex 10
		1.0f, 0.0f, 0.0f,
		-0.2f, 0.2f, 0.0f,		// vertex 11
		1.0f, 0.0f, 0.0f,

		//truck chassis
		-0.7f, -0.2f, 0.0f,		// vertex 12
		0.5f, 0.5f, 0.5f,
		-0.7f, -0.3f, 0.0f,		// vertex 13
		0.4f, 0.4f, 0.4f,
		0.7f, -0.2f, 0.0f,		// vertex 14
		0.5f, 0.5f, 0.5f,
		-0.7f, -0.3f, 0.0f,		// vertex 15
		0.4f, 0.4f, 0.4f,
		0.7f, -0.3f, 0.0f,		// vertex 16
		0.4f, 0.4f, 0.4f,
		0.7f, -0.2f, 0.0f,		// vertex 17
		0.5f, 0.5f, 0.5f,

		//truck window
		-0.5f, 0.43f, 0.0f,		// vertex 18
		0.2f, 0.2f, 0.2f,
		-0.35f, 0.43f, 0.0f,	// vertex 19
		0.2f, 0.2f, 0.2f,
		-0.65f, 0.2f, 0.0f,		// vertex 20 
		0.2f, 0.2f, 0.2f,
		-0.65f, 0.2f, 0.0f,		// vertex 21
		0.2f, 0.2f, 0.2f,
		-0.35f, 0.2f, 0.0f,		// vertex 22
		0.2f, 0.2f, 0.2f,
		-0.35f, 0.43f, 0.0f,	// vertex 23
		0.2f, 0.2f, 0.2f,

		// dump box
		- 0.2f, 0.1f, 0.0f,		// vertex 24
		1.0f, 0.0f, 0.0f,
		0.0f, 0.4f, 0.0f,		// vertex 25
		1.0f, 0.0f, 0.0f,
		0.7f, 0.4f, 0.0f,		// vertex 26
		1.0f, 0.0f, 0.0f,
		0.7f, 0.4f, 0.0f,		// vertex 27
		1.0f, 0.0f, 0.0f,
		-0.2f, 0.1f, 0.0f,		// vertex 28		
		1.0f, 0.0f, 0.0f,
		0.9f, 0.1f, 0.0f,		// vertex 29
		0.0f, 1.0f, 0.0f,
		-0.2f, 0.1f, 0.0f,		// vertex 30
		1.0f, 0.0f, 0.0f,
		0.0f, -0.2f, 0.0f,		// vertex 31
		0.0f, 1.0f, 0.0f,
		0.9f, 0.1f, 0.0f,		// vertex 32
		0.0f, 1.0f, 0.0f,
		0.9f, 0.1f, 0.0f,		// vertex 33
		0.0f, 1.0f, 0.0f,
		0.0f, -0.2f, 0.0f,		// vertex 34
		0.0f, 1.0f, 0.0f,
		0.7f, -0.2f, 0.0f,		// vertex 35
		0.0f, 1.0f, 0.0f
	};

	// create VBO and buffer the data
	glGenBuffers(1, &gVBO);					// generate unused VBO identifier
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);	// bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// create VAO, specify VBO data and format of the data
	glGenVertexArrays(1, &gVAO);			// generate unused VAO identifier
	glBindVertexArray(gVAO);				// create VAO
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);	// bind the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor),
		reinterpret_cast<void*>(offsetof(VertexColor, position)));	// specify format of position data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor),
		reinterpret_cast<void*>(offsetof(VertexColor, color)));	//specify format of colour data

	glEnableVertexAttribArray(0);	// enable vertex attributes
	glEnableVertexAttribArray(1);
}

// function used to update scene
static void update_scene(GLFWwindow* window) {
	//Calculate the displacement based on truck speed and frame time
	float displacement = gTruckSpeed * gFrameTime;

	// declare variables to transform the object
	glm::vec3 moveVec(0.0f);
	glm::vec3 scaleVec(1.0f);
	

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		moveVec.x -= gTranslateSensitivity;
		wheelRotate += gRotateSensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		moveVec.x += gTranslateSensitivity;
		wheelRotate += gRotateSensitivity;
	}

	gModelMatrix["Truck"] *= glm::translate(moveVec);
	gGroundModelMatrix = gModelMatrix["Truck"];

	//Rotate wheels based on truck speed
	wheelRotate += gTruckSpeed * gFrameTime * 10.0f;

	glClearColor(gBackgroundColor.r, gBackgroundColor.g, gBackgroundColor.b, 1.0f);
}
// function to render the scene
static void render_scene()
{
	// clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	gShader.use();

	glBindVertexArray(gVAO);			// make VAO active

	// Truck object
	gShader.setUniform("uModelMatrix", gModelMatrix["Truck"]);
	glDrawArrays(GL_TRIANGLES, 0, 24);

	// Rotate dump box
	glm::vec3 gDumpBoxRotationAxis(0.0f, 0.0f, 1.0f);
	glm::mat4 gDumpBoxRotationMatrix = glm::rotate(glm::radians(gDumpAngle), -gDumpBoxRotationAxis);
	gModelMatrix["DumpBox"] = glm::translate(gModelMatrix["Truck"], dumpBoxPivotPoint) * gDumpBoxRotationMatrix
								* glm::translate(-dumpBoxPivotPoint);
	// Dump Box object
	gShader.setUniform("uModelMatrix", gModelMatrix["DumpBox"]);
	glDrawArrays(GL_TRIANGLES, 24, 36);

	

	//Wheels1 object
	glm::mat4 wheels1ModelMatrix = gModelMatrix["Truck"];
	wheels1ModelMatrix = glm::translate(wheels1ModelMatrix, glm::vec3(-0.4f, -0.25f, 0.0f));
	wheels1ModelMatrix = glm::rotate(wheels1ModelMatrix, -wheelRotate, glm::vec3(0.0f, 0.0f, 1.0f));
	gShader.setUniform("uModelMatrix", wheels1ModelMatrix);
	buildCircle(0.0f, 0.0f, 0.2f);
	buildRim(0.0f, 0.0f, 0.13);

	//Wheels2 object
	glm::mat4 wheels2ModelMatrix = gModelMatrix["Truck"];
	wheels2ModelMatrix = glm::translate(wheels2ModelMatrix, glm::vec3(0.4f, -0.25f, 0.0f));
	wheels2ModelMatrix = glm::rotate(wheels2ModelMatrix, -wheelRotate, glm::vec3(0.0f, 0.0f, 1.0f));
	gShader.setUniform("uModelMatrix", wheels2ModelMatrix);
	buildCircle(0.0f, 0.0f, 0.2f);
	buildRim(0.0f, 0.0f, 0.13f);

	//render ground
	gShader.setUniform("uModelMatrix", gGroundModelMatrix);
	buildGround(static_cast<float>(gWindowWidth));
	
	// flush the graphics pipeline
	glFlush();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//close window when ESCAPE key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		//set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	TwEventMousePosGLFW(static_cast<int>(xpos), static_cast<int>(ypos));
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	TwEventMouseButtonGLFW(button, action);
}

// error callback function
static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;	// output error description
}

void TW_CALL GetDumpBoxAngle(void* value, void* clientData)
{
	*static_cast<float*>(value) = gDumpAngle;
}

void TW_CALL SetDumpBoxAngle(const void* value, void* clientData)
{
	gDumpAngle = *static_cast<const float*>(value);
}

TwBar* create_UI(const std::string name)
{
	// create a tweak bar
	TwBar* twBar = TwNewBar(name.c_str());

	// allows tweak bar to know size of window 
	TwWindowSize(gWindowWidth, gWindowHeight);

	TwDefine(" TW_HELP visible=false "); // disable help menu
	TwDefine(" GLOBAL fontsize=3 "); // set large font size
	TwDefine(" Main label='MyGUI' refresh=0.02 text=light size='220 320' ");

	// create display entries
	TwAddVarRW(twBar, "Wireframe", TW_TYPE_BOOLCPP, &gWireframe, " group='Display' ");
	TwAddVarRW(twBar, "BgColor", TW_TYPE_COLOR3F, &gBackgroundColor, " label='Background Color' group = 'Display' opened = true ");

	// create frame info entries
	TwAddVarRO(twBar, "FPS", TW_TYPE_FLOAT, &gFrameRate, " group='Frame Statistics' precision = 2 ");
	TwAddVarRO(twBar, "Frame Time", TW_TYPE_FLOAT, &gFrameTime, " group='Frame Statistics' ");
	TwAddVarCB(twBar, "Angle", TW_TYPE_FLOAT, SetDumpBoxAngle, GetDumpBoxAngle, NULL, " group='Tipper Angle' min=0 max=45 step=1 ");

	return twBar;
}

int main(void)
{
	GLFWwindow* window = nullptr;	// GLFW window handle

	glfwSetErrorCallback(error_callback);	// set GLFW error callback function

	// initialise GLFW
	if (!glfwInit())
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Assignment1", nullptr, nullptr);

	// check if window created successfully
	if (window == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialise GLEW
		std::cerr << "GLEW initialisation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);
	TwBar* tweakBar = create_UI("Main");

	update_scene(window);

	// initialise scene and render settings
	init(window);

	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time since last update
	int frameCount = 0;						// number of frames 

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);

		update_scene(window);	// update scene

		if (gWireframe)

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		render_scene();		// render the scene

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TwDraw();

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events
		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime;	// time since last update

		if (elapsedTime > 1.0)
		{
			gFrameTime = elapsedTime / frameCount;	// average time per frame
			gFrameRate = 1 / gFrameTime;			// frames per second
			lastUpdateTime = glfwGetTime();			// set last update time to current time
			frameCount = 0;							// reset frame counter
		}
	}

	// clean up
	glDeleteBuffers(1, &gVBO);
	glDeleteVertexArrays(1, &gVAO);
	glDeleteVertexArrays(1, &gVAO);

	// delete and uninitialise tweak bar
	TwDeleteBar(tweakBar);
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}