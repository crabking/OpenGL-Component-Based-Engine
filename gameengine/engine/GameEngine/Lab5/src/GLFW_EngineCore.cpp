#include "GLFW_EngineCore.h"

#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <../ft2build.h>
#include FT_FREETYPE_H 
#define GLM_FORCE_RADIANS
#define MOVE_VELOCITY 0.01f
#define ROTATE_VELOCITY 0.001f

std::vector<bool> GLFW_EngineCore::m_keyBuffer;
int GLFW_EngineCore::m_screenWidth;
int GLFW_EngineCore::m_screenHeight;

double previousCursorPosX, previousCursorPosY, currentCursorPosX, currentCursorPosY;

CameraComponent* m_currentCameraComponent;
SceneHandler * g_sceneHandler;


GLFW_EngineCore::GLFW_EngineCore() {
	DebugLog& log = log.getInstance();
	

}

GLFW_EngineCore::~GLFW_EngineCore()
{
	// cleanup
	glfwTerminate();
}

bool GLFW_EngineCore::initWindow(int width, int height, std::string windowName)
{
	

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	m_screenWidth = width;
	m_screenHeight = height;

	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	
	

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);
	//mouseMoveCallbackEvent(game, m_window, 1, 1);


	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

	// set the shaders to the given default ones
	setDefaultShaders();
	setupDefaultFont();
	// set the drawable model as a cube
	// note: this will be changed later when we can draw many kinds of objects
	initCubeModel();

	//-> CUSTOM CURSOR <- //
	unsigned char pixels[16 * 16 * 4];
	memset(pixels, 0xff, sizeof(pixels));
	GLFWimage image;
	image.width = 4;
	image.height = 4;
	image.pixels = pixels;
	GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
	glfwSetCursor(m_window, cursor);
	//-> CUSTOM CURSOR END <- //

	glEnable(GL_DEPTH_TEST);//! Enable depth test.

	return true;
}

bool GLFW_EngineCore::runEngine(Game& game)
{
	// for this example just give the game direct access to the engine
	// there are other ways this could be handled
	game.m_engineInterfacePtr = this;
	
	
	// message loop
	while (!glfwWindowShouldClose(m_window))
	{
		
		game.m_inputHandler->handleInputs(m_keyBuffer);
	
		
		
		
		// TODO, need to add handling here for scenes
		
		game.update(); // update game logic
		game.render(); // prepare game to send info to the renderer in engine core

		mouseMoveCallbackEvent(&game, m_window, 1, -1); // see mouse position.
		
		displayTextToScreen(m_window);
	
		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	
	return true;
}


void GLFW_EngineCore::displayTextToScreen(GLFWwindow * window) {

	DebugLog& log = log.getInstance();

	int l_vectorSize = 10;
	vector<string> l_vector;
	l_vector = log.v_messages;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		
		float messageOffset = 0.025f;
		for (int i = 0; i < l_vector.size(); i++)
		{
			renderText(l_vector[i], 0.04f, messageOffset, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			messageOffset += 0.035f;
		}
	}
	

}

void GLFW_EngineCore::renderColouredBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFW_EngineCore::renderFPS()
{
	double lastTime = glfwGetTime();
	int nbFrames = 0;


	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		// printf and reset timer
		std::cout << "ms/frame\n" << 1000.0 / double(nbFrames) << std::endl;
		nbFrames = 0;
		lastTime += 1.0;
	}

}


// screen is between 0 and 1 with (0,0) at the bottom left corner
// typical use: m_engineInterfacePtr->renderText("Awesome Game Debug Mode", 0.05f, 0.05f, 1.0f, glm::vec3(0.9, 0.9f, 0.1f));
void GLFW_EngineCore::renderText(std::string text, float x, float y, float scale, glm::vec3 colour)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// set the window to orthographic
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));

	float pixelValueX = x * m_screenWidth;
	float pixelValueY = y * m_screenHeight;

	glUseProgram(m_fontShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_fontShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Activate corresponding render state	
	glUniform3f(glGetUniformLocation(m_fontShaderProgram, "textColour"), colour.x, colour.y, colour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = pixelValueX + ch.bearing.x * scale;
		GLfloat ypos = pixelValueY - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pixelValueX += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// go back to default shader
	glUseProgram(m_defaultShaderProgram);
}


void GLFW_EngineCore::mouseMoveCallbackEvent(Game* game, GLFWwindow * window, double xPos, double yPos)
{
	TransformComponent* playerTransform = game->getCurrentScene()->getControlledPlayerObject()->getComponent<TransformComponent>();
	g_sceneHandler = game->getSceneHandler();
	// ---------------------- LOCAL VARIABLE INITALIZATION FROM GAME OBJECT CLASS ----------------------//
	
	glm::quat orientation = playerTransform->getOrientation();
	glm::vec3 position = playerTransform->getPosition();
	glm::vec3 relativePosition = game->getCurrentScene()->getControlledPlayerObject()->getRelativePos();
	bool firstPersonCamera = game->getCurrentScene()->getControlledPlayerObject()->firstPerson;
	
	//----------------------- MOUSE CODE STARTS HERE ----------------------------------------------------//
	glfwGetCursorPos(window, &currentCursorPosX, &currentCursorPosY); //! Built int glfw get cursor pos function.
	//glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	float deltaxPos = (float)(previousCursorPosX - currentCursorPosX); //! Delta x, change in mouseposX
	float deltayPos = (float)(previousCursorPosY - currentCursorPosY); //! Delt y, change in mouseposY

	//std::cout << "Quaternion Rot deltaY: " << deltayPos << std::endl;
	//std::cout << "Quaternion Rot deltaX: " << deltaxPos << std::endl;

	// ---------------------------- FIRST PERSON CAMERA STUFF  ----------------------------------------//
	if (firstPersonCamera)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			playerTransform->cameraQuatRotate(deltayPos * ROTATE_VELOCITY, deltaxPos * ROTATE_VELOCITY);

			position += orientation * relativePosition;

			m_currentCameraComponent->setOrientation(orientation);

		}
	}
	// ------------------------------- THIRD PERSON CAMERA STUFF --------------------------------------//
	if (!firstPersonCamera)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			playerTransform->cameraQuatRotate(deltayPos * ROTATE_VELOCITY, deltaxPos * ROTATE_VELOCITY);

			position += orientation * relativePosition;

			m_currentCameraComponent->setOrientation(orientation);

		}
		
	}
	
	
	previousCursorPosX = currentCursorPosX; //Stores cursor pos
	previousCursorPosY = currentCursorPosY; //Stores cursor pos

	
}

//-----------------------------Private functions------------------------------
void GLFW_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{

		g_sceneHandler->swapScene();
		

	}

}

void GLFW_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
}


void GLFW_EngineCore::loadShader(string vertexShaderFile, string fragmentShaderFile, GLuint& shaderProgram)
{
	

	// Load contents of vertex file
	std::ifstream inFile(vertexShaderFile);
	if (!inFile) {
		string errorMsg = "Error opening shader file: " + vertexShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());
	const GLchar* vertex_shader[] = { codeStr.c_str() };

	// Load contents of fragment file
	std::ifstream inFile2(fragmentShaderFile);
	if (!inFile2) {
		string errorMsg = "Error opening shader file: " + fragmentShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code2;
	code2 << inFile2.rdbuf();
	inFile2.close();
	std::string codeStr2(code2.str());
	const GLchar* fragment_shader[] = { codeStr2.c_str() };

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertex_shader, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragment_shader, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


// loading some default shaders to get things up and running
void GLFW_EngineCore::setDefaultShaders()
{
	DebugLog& log = log.getInstance();

	loadShader("assets/shaders/defaultShader.vert", "assets/shaders/defaultShader.frag", m_defaultShaderProgram);
	loadShader("assets/shaders/fontShader.vert", "assets/shaders/fontShader.frag", m_fontShaderProgram);
	
	// set the default shader
	glUseProgram(m_defaultShaderProgram);

	log.writeLog("Default Shaders Loaded.");
}

// based on tutorial from https://learnopengl.com/#!In-Practice/Text-Rendering
void GLFW_EngineCore::setupDefaultFont()
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &font_VAO);
	glGenBuffers(1, &font_VBO);
	glBindVertexArray(font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
	// dynamic draw as the text may change frequently
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//a simple function to initialise a cube model in memory
void GLFW_EngineCore::initCubeModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}



void GLFW_EngineCore::setFirstPersonCamera(FirstPersonCameraComponent* fpcam)
{
	
	m_currentCameraComponent = fpcam;

	glm::mat4 projection = glm::perspective(glm::radians(fpcam->getFOV()), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 500.0f);
	// set the view and projection components of our shader to the camera values 
	//0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 0.1f, 100.0f
	//glm::mat4 projection = glm::ortho(-(float)m_screenWidth, (float)m_screenWidth,-(float)m_screenHeight, (float)m_screenHeight, 0.1f, 50.0f);
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(fpcam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 10, glm::value_ptr(fpcam->getPosition()));
	

}

void GLFW_EngineCore::setThirdPersonCamera(ThirdPersonCameraComponent* tpcam)
{
	m_currentCameraComponent = tpcam;

	glm::mat4 projection = glm::perspective(glm::radians(tpcam->getFOV()), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 500.0f);
	// set the view and projection components of our shader to the camera values 
	//0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 0.1f, 100.0f
	//glm::mat4 projection = glm::ortho(-(float)m_screenWidth, (float)m_screenWidth,-(float)m_screenHeight, (float)m_screenHeight, 0.1f, 50.0f);
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(tpcam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 10, glm::value_ptr(tpcam->getPosition()));

}


void GLFW_EngineCore::drawModel(Model* model, glm::mat4& modelMatrix)
{
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->render(m_defaultShaderProgram);
}

