#include "Assignment2.h"

// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set the color the color buffer should be cleared to
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// compile and link a vertex and fragment shader pair
	gShader.compileAndLink("lighting.vert", "reflection.frag");
	Cube_envm_Map_Program.compileAndLink("lighting.vert", "lighting_cubemap.frag");
	Floor_ShaderProgram.compileAndLink("lightingAndTexture.vert", "pointLightTexture.frag");
	Line_Shaderprogram.compileAndLink("modelViewProj.vert", "color.frag");
	Walls_Shader_program.compileAndLink("normalMap.vert", "normalMap.frag");
	Painting_SP.compileAndLink("lightingAndTexture.vert", "pointLightTexture.frag");

	// initialise view matrix
	gCamera.setViewMatrix(glm::vec3(0.0f, 2.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	// initialise projection matrix
	gCamera.setProjMatrix(glm::perspective(glm::radians(100.0f),
		static_cast<float>(gWindowWidth) / gWindowHeight, 0.1f, 100.0f));

	fCamera.setViewMatrix(glm::vec3(0.0f, 2.0f, 3.0f),
		glm::vec3(0.0f, 2.0f, 1.0f));

	// initialise projection matrix
	fCamera.setProjMatrix(glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 0.1f, 100.0f));

	tCamera.setViewMatrix(glm::vec3(0.0f, 5.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	// initialise projection matrix
	tCamera.setProjMatrix(glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 0.1f, 100.0f));

	main_camera.setViewMatrix(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// initialise projection matrix
	main_camera.setProjMatrix(glm::ortho(0.f, static_cast<float>(gWindowWidth), 0.f, static_cast<float>(gWindowHeight), 0.1f, 10.f));

	// initialise directional light properties
	gLight.dir = glm::vec3(0.3f, -0.7f, -0.5f);
	gLight.La = glm::vec3(1.f);
	gLight.Ld = glm::vec3(1.0f);
	gLight.Ls = glm::vec3(1.0f);
	gLight.att = glm::vec3(1.0f, 0.f, 0.f);


	// load model
	gModel.loadModel("./models/cube.obj");

	//Generate unused VBO and VAO identifiers
	glGenBuffers(index_for_VBO_VAO, gVBO);
	glGenVertexArrays(index_for_VBO_VAO, gVAO);


	/* Cube environment mapping stuff here */

	Cube_Material.Ka = glm::vec3(0.2f);
	Cube_Material.Kd = glm::vec3(0.2f, 0.7f, 1.0f);
	Cube_Material.Ks = glm::vec3(0.2f, 0.7f, 1.0f);
	Cube_Material.shininess = 40.0f;

	//Initialising model matrices
	Cube_ModelMatrix = glm::mat4(1.f);
	//Load cube envm map texture
	Cube_envm_Map.generate(
		"./images/cm_front.bmp", "./images/cm_back.bmp",
		"./images/cm_left.bmp", "./images/cm_right.bmp",
		"./images/cm_top.bmp", "./images/cm_bottom.bmp");
	Cube_Model.loadModel("./models/torus.obj");

	/* Cube Environment map ends here */

	/* Floor stuff Starts here */

	Floor_ModelMatrix = glm::mat4(1.f);

	Floor_Material.Ka = glm::vec3(0.25f, 0.21f, 0.21f);
	Floor_Material.Kd = glm::vec3(1.0f, 0.83f, 0.83f);
	Floor_Material.Ks = glm::vec3(0.3f, 0.3f, 0.3f);
	Floor_Material.shininess = 11.3f;

	Floor_Texture.generate("./images/check.bmp");

	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Floor_Vertices.size(), &Floor_Vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(gVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, position)));	// specify format of position data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, normal)));		// specify format of colour data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, texCoord)));	// specify format of texture coordinate data

	glEnableVertexAttribArray(0);	// enable vertex attributes
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	/* Floor Stuff ends here */


	/* Line stuff starts here */

	glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * line_vertices.size(), &line_vertices[0], GL_STATIC_DRAW);


	// create VAO, specify VBO data and format of the data
	glBindVertexArray(gVAO[1]);				// create VAO
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);	// bind the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor),
		reinterpret_cast<void*>(offsetof(VertexColor, position)));	// specify format of position data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor),
		reinterpret_cast<void*>(offsetof(VertexColor, color)));		// specify format of colour data

	glEnableVertexAttribArray(0);	// enable vertex attributes
	glEnableVertexAttribArray(1);

	/* Line Stuff ends here*/


	/* Wall Stuff begins here */

	Walls_Material.Ka = glm::vec3(0.2f);
	Walls_Material.Kd = glm::vec3(0.2f, 0.7f, 1.0f);
	Walls_Material.Ks = glm::vec3(0.2f, 0.7f, 1.0f);
	Walls_Material.shininess = 40.0f;

	Walls_texture["Wall"].generate("./images/Fieldstone.bmp");
	Walls_texture["WallNormalMap"].generate("./FieldstoneBumpDOT3.bmp");

	int counter = 1;
	for (int i = 0; i < 4; i++)
	{
		Walls_Model_Matrix[i] = glm::mat4(1.f);
		glBindBuffer(GL_ARRAY_BUFFER, gVBO[i + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Walls_Vertices.size(), &Walls_Vertices[0], GL_STATIC_DRAW);

		// create VAO, specify VBO data and format of the data
		glBindVertexArray(gVAO[i + 2]);				// create VAO
		glBindBuffer(GL_ARRAY_BUFFER, gVBO[i + 2]);	// bind the VBO

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
			reinterpret_cast<void*>(offsetof(VertexNormTanTex, position)));		// specify format of position data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
			reinterpret_cast<void*>(offsetof(VertexNormTanTex, normal)));		// specify format of colour data
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
			reinterpret_cast<void*>(offsetof(VertexNormTanTex, tangent)));		// specify format of tangent data
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
			reinterpret_cast<void*>(offsetof(VertexNormTanTex, texCoord)));		// specify format of texture coordinate data

		glEnableVertexAttribArray(0);	// enable vertex attributes
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}


	/* Wall Stuff ends here */


	/* Painting Stuff starts here */

	Painting_ModelMatrix = glm::mat4(1.f);

	Painting_Material.Ka = glm::vec3(0.25f, 0.21f, 0.21f);
	Painting_Material.Kd = glm::vec3(1.0f, 0.83f, 0.83f);
	Painting_Material.Ks = glm::vec3(0.3f, 0.3f, 0.3f);
	Painting_Material.shininess = 11.3f;

	Paint_Texture.generate("./images/Smile.bmp");

	glBindBuffer(GL_ARRAY_BUFFER, gVBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Paint_Vertices.size(), &Paint_Vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(gVAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[6]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, position)));	// specify format of position data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, normal)));		// specify format of colour data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, texCoord)));	// specify format of texture coordinate data

	glEnableVertexAttribArray(0);	// enable vertex attributes
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	/* Painting Stuff ends here */
}

// function used to update the scene
static void update_scene(GLFWwindow* window)
{
	// stores camera forward/back, up/down and left/right movements
	float moveForward = 0.0f;
	float moveRight = 0.0f;

	// update movement variables based on keyboard input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveForward += gCamMoveSensitivity * gFrameTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveForward -= gCamMoveSensitivity * gFrameTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveRight -= gCamMoveSensitivity * gFrameTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveRight += gCamMoveSensitivity * gFrameTime;

	if (rotationtime)
	{
		RotateZ++;
	}

	// update camera position and direction
	gCamera.update(moveForward, moveRight);


	Floor_ModelMatrix = glm::translate(glm::vec3(0, 0, 0)) * glm::scale(glm::vec3(5, 1, 5));
	Walls_Model_Matrix[0] = glm::translate(glm::vec3(5, 5, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 5, 1));
	Walls_Model_Matrix[1] = glm::translate(glm::vec3(-5, 5, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 5, 1));
	Walls_Model_Matrix[2] = glm::translate(glm::vec3(0, 5, -5)) * glm::scale(glm::vec3(5, 5, 1));
	Walls_Model_Matrix[3] = glm::translate(glm::vec3(0, 5, 5)) * glm::scale(glm::vec3(5, 5, 1));
	Cube_ModelMatrix = glm::translate(glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::radians(RotateZ), glm::vec3(1, 0, 0));
	Painting_ModelMatrix = glm::translate(glm::vec3(0, 2, -4.9f)) * glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(2, 2, 2));
}

void Draw_Cube_Environment_Map(std::string camera)
{
	Cube_envm_Map_Program.use();

	// set light properties
	Cube_envm_Map_Program.setUniform("uLight.dir", gLight.dir);
	Cube_envm_Map_Program.setUniform("uLight.La", gLight.La);
	Cube_envm_Map_Program.setUniform("uLight.Ld", gLight.Ld);
	Cube_envm_Map_Program.setUniform("uLight.Ls", gLight.Ls);

	// set material properties
	Cube_envm_Map_Program.setUniform("uMaterial.Ka", Cube_Material.Ka);
	Cube_envm_Map_Program.setUniform("uMaterial.Kd", Cube_Material.Kd);
	Cube_envm_Map_Program.setUniform("uMaterial.Ks", Cube_Material.Ks);
	Cube_envm_Map_Program.setUniform("uMaterial.shininess", Cube_Material.shininess);

	// set viewing position
	Cube_envm_Map_Program.setUniform("uViewpoint", gCamera.getPosition());

	glm::mat4 MVP;
	if (camera == "Right")
	{
		MVP = gCamera.getProjMatrix() * gCamera.getViewMatrix() * Cube_ModelMatrix;
	}
	else if (camera == "Left")
	{
		MVP = fCamera.getProjMatrix() * fCamera.getViewMatrix() * Cube_ModelMatrix;
	}
	else
	{
		MVP = tCamera.getProjMatrix() * tCamera.getViewMatrix() * Cube_ModelMatrix;
	}

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(Cube_ModelMatrix)));

	// set uniform variables
	Cube_envm_Map_Program.setUniform("uModelViewProjectionMatrix", MVP);
	Cube_envm_Map_Program.setUniform("uModelMatrix", Cube_ModelMatrix);
	Cube_envm_Map_Program.setUniform("uNormalMatrix", normalMatrix);

	// set cube environment map
	Cube_envm_Map_Program.setUniform("uEnvironmentMap", 0);

	glActiveTexture(GL_TEXTURE0);
	Cube_envm_Map.bind();

	// render model
	Cube_Model.drawModel();

}

void Draw_Floor(std::string camera)
{
	Floor_ShaderProgram.use();

	// set light properties
	Floor_ShaderProgram.setUniform("uLight.pos", gLight.pos);
	Floor_ShaderProgram.setUniform("uLight.La", gLight.La);
	Floor_ShaderProgram.setUniform("uLight.Ld", gLight.Ld);
	Floor_ShaderProgram.setUniform("uLight.Ls", gLight.Ls);
	Floor_ShaderProgram.setUniform("uLight.att", gLight.att);

	// set material properties
	Floor_ShaderProgram.setUniform("uMaterial.Ka", Floor_Material.Ka);
	Floor_ShaderProgram.setUniform("uMaterial.Kd", Floor_Material.Kd);
	Floor_ShaderProgram.setUniform("uMaterial.Ks", Floor_Material.Ks);
	Floor_ShaderProgram.setUniform("uMaterial.shininess", Floor_Material.shininess);

	// set viewing position
	Floor_ShaderProgram.setUniform("uViewpoint", glm::vec3(0.0f, 2.0f, 4.0f));

	// calculate matrices
	glm::mat4 MVP;
	if (camera == "Right")
	{
		MVP = gCamera.getProjMatrix() * gCamera.getViewMatrix() * Floor_ModelMatrix;
	}
	else if (camera == "Left")
	{
		MVP = fCamera.getProjMatrix() * fCamera.getViewMatrix() * Floor_ModelMatrix;
	}
	else
	{
		MVP = tCamera.getProjMatrix() * tCamera.getViewMatrix() * Floor_ModelMatrix;
	}

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(Floor_ModelMatrix)));

	// set uniform variables
	Floor_ShaderProgram.setUniform("uModelViewProjectionMatrix", MVP);
	Floor_ShaderProgram.setUniform("uModelMatrix", Floor_ModelMatrix);
	Floor_ShaderProgram.setUniform("uNormalMatrix", normalMatrix);

	// set texture
	Floor_ShaderProgram.setUniform("uTextureSampler", 0);

	glActiveTexture(GL_TEXTURE0);
	Floor_Texture.bind();

	glBindVertexArray(gVAO[0]);				// make VAO active
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices
}

void Draw_Lines()
{
	Line_Shaderprogram.use();
	glBindVertexArray(gVAO[1]);

	glm::mat4 MVP;
	MVP = main_camera.getProjMatrix() * main_camera.getViewMatrix();

	Line_Shaderprogram.setUniform("uModelViewProjectionMatrix", MVP);
	glDrawArrays(GL_LINES, 0, 4);

}

void Draw_Walls(std::string camera)
{
	for (int counter = 0; counter < 4; counter++)
	{
		Walls_Shader_program.use();


		Walls_Shader_program.setUniform("uLight.pos", gLight.pos);
		Walls_Shader_program.setUniform("uLight.La", gLight.La);
		Walls_Shader_program.setUniform("uLight.Ld", gLight.Ld);
		Walls_Shader_program.setUniform("uLight.Ls", gLight.Ls);
		Walls_Shader_program.setUniform("uLight.att", gLight.att);

		Walls_Shader_program.setUniform("uMaterial.Ka", Walls_Material.Ka);
		Walls_Shader_program.setUniform("uMaterial.Kd", Walls_Material.Kd);
		Walls_Shader_program.setUniform("uMaterial.Ks", Walls_Material.Ks);
		Walls_Shader_program.setUniform("uMaterial.shininess", Walls_Material.shininess);

		Walls_Shader_program.setUniform("uViewpoint", glm::vec3(0.0f, 0.0f, 4.0f));

		glm::mat4 MVP;
		if (camera == "Right")
		{
			MVP = gCamera.getProjMatrix() * gCamera.getViewMatrix() * Walls_Model_Matrix[counter];
		}
		else if (camera == "Left")
		{
			MVP = fCamera.getProjMatrix() * fCamera.getViewMatrix() * Walls_Model_Matrix[counter];
		}
		else
		{
			MVP = tCamera.getProjMatrix() * tCamera.getViewMatrix() * Walls_Model_Matrix[counter];
		}
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(Walls_Model_Matrix[counter])));

		// set uniform variables
		Walls_Shader_program.setUniform("uModelViewProjectionMatrix", MVP);
		Walls_Shader_program.setUniform("uModelMatrix", Walls_Model_Matrix[counter]);
		Walls_Shader_program.setUniform("uNormalMatrix", normalMatrix);

		// set texture and normal map
		Walls_Shader_program.setUniform("uTextureSampler", 0);
		Walls_Shader_program.setUniform("uNormalSampler", 1);

		glActiveTexture(GL_TEXTURE0);
		Walls_texture["Wall"].bind();

		glActiveTexture(GL_TEXTURE1);
		Walls_texture["WallNormalMap"].bind();

		glBindVertexArray(gVAO[counter + 2]);				// make VAO active
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices
	}


}

void Draw_Painting(std::string camera)
{
	Painting_SP.use();

	// set light properties
	Painting_SP.setUniform("uLight.pos", gLight.pos);
	Painting_SP.setUniform("uLight.La", gLight.La);
	Painting_SP.setUniform("uLight.Ld", gLight.Ld);
	Painting_SP.setUniform("uLight.Ls", gLight.Ls);
	Painting_SP.setUniform("uLight.att", gLight.att);

	// set material properties
	Painting_SP.setUniform("uMaterial.Ka", Painting_Material.Ka);
	Painting_SP.setUniform("uMaterial.Kd", Painting_Material.Kd);
	Painting_SP.setUniform("uMaterial.Ks", Painting_Material.Ks);
	Painting_SP.setUniform("uMaterial.shininess", Painting_Material.shininess);

	// set viewing position
	Floor_ShaderProgram.setUniform("uViewpoint", glm::vec3(0.0f, 2.0f, 4.0f));

	// calculate matrices
	glm::mat4 MVP;
	if (camera == "Right")
	{
		MVP = gCamera.getProjMatrix() * gCamera.getViewMatrix() * Painting_ModelMatrix;
	}
	else if (camera == "Left")
	{
		MVP = fCamera.getProjMatrix() * fCamera.getViewMatrix() * Painting_ModelMatrix;
	}
	else
	{
		MVP = tCamera.getProjMatrix() * tCamera.getViewMatrix() * Painting_ModelMatrix;
	}
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(Painting_ModelMatrix)));

	// set uniform variables
	Painting_SP.setUniform("uModelViewProjectionMatrix", MVP);
	Painting_SP.setUniform("uModelMatrix", Floor_ModelMatrix);
	Painting_SP.setUniform("uNormalMatrix", normalMatrix);

	// set texture
	Painting_SP.setUniform("uTextureSampler", 0);

	glActiveTexture(GL_TEXTURE0);
	Paint_Texture.bind();

	glBindVertexArray(gVAO[6]);				// make VAO active
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices
}

// function to render the scene
static void render_scene()
{
	/************************************************************************************
	 * Clear colour buffer, depth buffer and stencil buffer
	 ************************************************************************************/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(0, 0, 400, 400);

	/************************************************************************************
	 * Draw the scene
	 ************************************************************************************/


	Draw_Cube_Environment_Map("Left");
	Draw_Floor("Left");
	Draw_Walls("Left");
	Draw_Painting("Left");

	glViewport(400, 0, 400, 400);

	/************************************************************************************
	 * Draw the scene
	 ************************************************************************************/


	Draw_Cube_Environment_Map("Right");
	Draw_Floor("Right");
	Draw_Walls("Right");
	Draw_Painting("Right");



	//Top-down view
	glViewport(400, 400, 400, 400);
	Draw_Cube_Environment_Map("Top");
	Draw_Floor("Top");
	Draw_Walls("Top");
	Draw_Painting("Top");

	glViewport(0, 0, 800, 800);
	Draw_Lines();
	// flush the graphics pipeline
	glFlush();
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// close the window when the ESCAPE key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

// mouse movement callback function
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// pass cursor position to tweak bar
	TwEventMousePosGLFW(static_cast<int>(xpos), static_cast<int>(ypos));

	// previous cursor coordinates
	static glm::vec2 previousPos = glm::vec2(xpos, ypos);
	static int counter = 0;

	// allow camera rotation when right mouse button held
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// stablise cursor coordinates for 5 updates
		if (counter < 5)
		{
			// set previous cursor coordinates
			previousPos = glm::vec2(xpos, ypos);
			counter++;
		}

		// change based on previous cursor coordinates
		float deltaYaw = (previousPos.x - xpos) * gCamRotateSensitivity * gFrameTime;
		float deltaPitch = (previousPos.y - ypos) * gCamRotateSensitivity * gFrameTime;

		// update camera's yaw and pitch
		gCamera.updateRotation(deltaYaw, deltaPitch);

		// set previous cursor coordinates
		previousPos = glm::vec2(xpos, ypos);
	}
	else
	{
		counter = 0;
	}
}

// mouse button callback function
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// pass mouse button status to tweak bar
	TwEventMouseButtonGLFW(button, action);
}

// error callback function
static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;	// output error description
}

// create and populate tweak bar elements
TwBar* create_UI(const std::string name)
{
	// create a tweak bar
	TwBar* twBar = TwNewBar(name.c_str());

	// give tweak bar the size of graphics window
	TwWindowSize(gWindowWidth, gWindowHeight);
	TwDefine(" TW_HELP visible=false ");	// disable help menu
	TwDefine(" GLOBAL fontsize=3 ");		// set large font size

	TwDefine(" Main label='Controls' refresh=0.02 text=light size='250 250' position='10 10' ");

	// create frame stat entries
	TwAddVarRO(twBar, "Frame Rate", TW_TYPE_FLOAT, &gFrameRate, " group='Frame Stats' precision=2 ");
	TwAddVarRO(twBar, "Frame Time", TW_TYPE_FLOAT, &gFrameTime, " group='Frame Stats' ");

	TwAddVarRW(twBar, "Toggle", TW_TYPE_BOOLCPP, &rotationtime, " group='Animation' ");

	// light controls
	TwAddVarRW(twBar, "Position X", TW_TYPE_FLOAT, &gLight.pos.x, " group='Light' min=-3.0 max=3.0 step=0.01 ");
	TwAddVarRW(twBar, "Position Y", TW_TYPE_FLOAT, &gLight.pos.y, " group='Light' min=-3.0 max=3.0 step=0.01 ");
	TwAddVarRW(twBar, "Position Z", TW_TYPE_FLOAT, &gLight.pos.z, " group='Light' min=-3.0 max=3.0 step=0.01 ");

	// camera controls
	TwAddVarRW(twBar, "Yaw", TW_TYPE_FLOAT, &gCamera.mYaw, " group='Camera' min=-6.3 max=6.3 step=0.01 ");
	TwAddVarRW(twBar, "Pitch", TW_TYPE_FLOAT, &gCamera.mPitch, " group='Camera' min=-6.3 max=6.3 step=0.01 ");




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
	window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Assignment 2", nullptr, nullptr);

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

	// set GLFW callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// initialise scene and render settings
	init(window);

	// initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, nullptr);
	TwBar* tweakBar = create_UI("Main");		// create and populate tweak bar elements

	// timing data
	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time since last update
	int frameCount = 0;						// number of frames since last update

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window);	// update the scene

		// if wireframe set polygon render mode to wireframe
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		render_scene();			// render the scene

		// set polygon render mode to fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TwDraw();				// draw tweak bar

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events

		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime;	// time since last update

		// if elapsed time since last update > 1 second
		if (elapsedTime > 1.0)
		{
			gFrameTime = elapsedTime / frameCount;	// average time per frame
			gFrameRate = 1 / gFrameTime;			// frames per second
			lastUpdateTime = glfwGetTime();			// set last update time to current time
			frameCount = 0;							// reset frame counter
		}
	}

	// clean up
	glDeleteBuffers(1, gVBO);
	glDeleteVertexArrays(1, gVAO);

	// uninitialise tweak bar
	TwDeleteBar(tweakBar);
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}