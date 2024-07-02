#ifndef ASSIGNMENT2_H_
#define ASSIGNMENT2_H_

#include "utilities.h"
#include "Camera.h"
#include "SimpleModel.h"
#include "Texture.h"

// global variables
// settings
unsigned int gWindowWidth = 800;
unsigned int gWindowHeight = 800;
const float gCamMoveSensitivity = 1.0f;
const float gCamRotateSensitivity = 0.1f;

// frame stats
float gFrameRate = 60.0f;
float gFrameTime = 1 / gFrameRate;

// scene content
ShaderProgram gShader;	// shader program object

const int index_for_VBO_VAO = 7;
GLuint gVBO[index_for_VBO_VAO];		// vertex buffer object identifier
GLuint gVAO[index_for_VBO_VAO];		// vertex array object identifier

Camera gCamera;					// camera object
Camera fCamera;
Camera tCamera;
std::map<std::string, glm::mat4> gModelMatrix;	// object matrix

Light gLight;					// light properties
std::map<std::string, Material>  gMaterial;		// material properties
SimpleModel gModel;				// object model

// controls
bool gWireframe = false;	// wireframe control
float gAlpha = 0.5f;		// reflective amount


/*

 Section for Cube Environmental mapping

*/

ShaderProgram Cube_envm_Map_Program;
glm::mat4 Cube_ModelMatrix;
Material Cube_Material;
SimpleModel Cube_Model;
Texture Cube_envm_Map;
bool rotationtime = false;
float RotateZ = 1;


void Draw_Cube_Environment_Map(std::string camera);

/*

 Section for the floor of the map

*/
ShaderProgram Floor_ShaderProgram;
glm::mat4 Floor_ModelMatrix;
Material Floor_Material;
Texture Floor_Texture;

std::vector<GLfloat> Floor_Vertices =
{
	-1.0f, 0.0f, 1.0f,	// vertex 0: position
	0.0f, 1.0f, 0.0f,	// vertex 0: normal
	0.0f, 0.0f,			// vertex 0: texture coordinate
	1.0f, 0.0f, 1.0f,	// vertex 1: position
	0.0f, 1.0f, 0.0f,	// vertex 1: normal
	1.0f, 0.0f,			// vertex 1: texture coordinate
	-1.0f, 0.0f, -1.0f,	// vertex 2: position
	0.0f, 1.0f, 0.0f,	// vertex 2: normal
	0.0f, 1.0f,			// vertex 2: texture coordinate
	1.0f, 0.0f, -1.0f,	// vertex 3: position
	0.0f, 1.0f, 0.0f,	// vertex 3: normal
	1.0f, 1.0f,			// vertex 3: texture coordinate
};

void Draw_Floor(std::string camera);

/*

 Section for the Walls, using Normal mapping

*/

ShaderProgram Walls_Shader_program;
glm::mat4 Walls_Model_Matrix[4];
Material Walls_Material;
std::map<std::string, Texture> Walls_texture;

std::vector<GLfloat> Walls_Vertices =
{
	-1.0f, -1.0f, 0.0f,	// vertex 0: position
	0.0f, 0.0f, 1.0f,	// vertex 0: normal
	1.0f, 0.0f, 0.0f,	// vertex 0: tangent
	0.0f, 0.0f,			// vertex 0: texture coordinate
	1.0f, -1.0f, 0.0f,	// vertex 1: position
	0.0f, 0.0f, 1.0f,	// vertex 1: normal
	1.0f, 0.0f, 0.0f,	// vertex 1: tangent
	1.0f, 0.0f,			// vertex 1: texture coordinate
	-1.0f, 1.0f, 0.0f,	// vertex 2: position
	0.0f, 0.0f, 1.0f,	// vertex 2: normal
	1.0f, 0.0f, 0.0f,	// vertex 2: tangent
	0.0f, 1.0f,			// vertex 2: texture coordinate
	1.0f, 1.0f, 0.0f,	// vertex 3: position
	0.0f, 0.0f, 1.0f,	// vertex 3: normal
	1.0f, 0.0f, 0.0f,	// vertex 3: tangent
	1.0f, 1.0f,			// vertex 3: texture coordinate
};

void Draw_Walls(std::string camera);


/*

 Section for the Lines

*/

ShaderProgram Line_Shaderprogram;
Camera main_camera;
glm::mat4 Line_Modelmatrix;

std::vector<GLfloat> line_vertices = {
	// lines
		0.0f, 400.0f, 0.0f,		// line 1 vertex 0: position
		1.0f, 1.0f, 1.0f,		// line 1 vertex 0: colour
		800.0f, 400.0f, 0.0f,	// line 1 vertex 1: position
		1.0f, 1.0f, 1.0f,		// line 1 vertex 1: colour
		400.0f, 0.0f, 0.0f,		// line 2 vertex 0: position
		1.0f, 1.0f, 1.0f,		// line 2 vertex 0: colour
		400.0f, 800.0f, 0.0f,	// line 2 vertex 1: position
		1.0f, 1.0f, 1.0f,		// line 2 vertex 1: colour
};


void Draw_Lines();

/*

 This section is for the painting

*/

ShaderProgram Painting_SP;
glm::mat4 Painting_ModelMatrix;
Material Painting_Material;
Texture Paint_Texture;

std::vector<GLfloat> Paint_Vertices =
{
	-1.0f, 0.0f, 1.0f,	// vertex 0: position
	0.0f, 1.0f, 0.0f,	// vertex 0: normal
	0.0f, 0.0f,			// vertex 0: texture coordinate
	1.0f, 0.0f, 1.0f,	// vertex 1: position
	0.0f, 1.0f, 0.0f,	// vertex 1: normal
	1.0f, 0.0f,			// vertex 1: texture coordinate
	-1.0f, 0.0f, -1.0f,	// vertex 2: position
	0.0f, 1.0f, 0.0f,	// vertex 2: normal
	0.0f, 1.0f,			// vertex 2: texture coordinate
	1.0f, 0.0f, -1.0f,	// vertex 3: position
	0.0f, 1.0f, 0.0f,	// vertex 3: normal
	1.0f, 1.0f,			// vertex 3: texture coordinate
};

void Draw_Painting(std::string camera);

#endif
