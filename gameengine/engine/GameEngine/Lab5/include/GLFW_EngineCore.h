#pragma once
/*!
\file GLFW_EngineCore.h
*/

#include "IEngineCore.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include "Game.h"

#include "Model.h"

#include "DebugLog.h"
#include "SceneHandler.h"


/*!
\class GLFW_EngineCore
\brief handles all engine related tasks such as rendering and drawing, setting cameras.
*/
class GLFW_EngineCore : public IEngineCore
{
private:

	GLFWwindow* m_window; //!< GLFWwindow pointer variable.

	GLuint m_defaultShaderProgram; //!< GLuint default shader.
	GLuint m_fontShaderProgram; //!< GLuint font shader.

	//! character Struct, hold character variables for text.
	/*!
	*/
	struct Character {
		GLuint textureID;   // ID handle of the glyph texture
		glm::ivec2 size;    // Size of glyph
		glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
		GLuint advance;    // Horizontal offset to advance to next glyph
	};
	std::map<GLchar, Character> Characters; //!< map of GLchar and Characters.
	GLuint font_VAO, font_VBO; //!< font VAO and VBO variables.


	//bool b_ThirdPersonCamera = false; //!< Bool for setting camera.
	static int m_screenWidth; //!< screen width.
	static int m_screenHeight;//!< screen height.
	static std::vector<bool> m_keyBuffer; //!< key buffer.
	static const int m_keyBufferSize = 400; //!< key buffer size.


	//! Static mouse callback function for mouse movement.
	/*!
	\param game reference to game object.
	\param window ptr to window object.
	\param xPos x position of mouse.
	\param yPos y position of mouse.
	*/
	static void mouseMoveCallbackEvent(Game* game, GLFWwindow* window, double xPos, double yPos);

	//! Static key callback for keyboard input.
	/*!
	\param window ptr to window object.
	\param key keycodes for keyboard.
	\param scancode
	\param action
	\param mods
	*/
	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

	//! Window resize call back event.
	/*!
	\param window ptr to window object.
	\param width width of window.
	\param height height of window.
	*/

	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height);

	//! Load shader function.
	/*!
	\param vertexShaderFile string that holds vertex shader file name.
	\param fragmentShaderFile string that holds fragment shader file name.
	\param shaderProgram reference to shader.
	*/
	void loadShader(std::string vertexShaderFile, std::string fragmentShaderFile, GLuint& shaderProgram);

	//! set default shader function.
	/*!
	*/
	void setDefaultShaders();

	//! initialize cube model function.
	/*!
	*/
	void initCubeModel();

	//! set up default font function.
	/*!
	*/
	void setupDefaultFont();
public:

	GLFW_EngineCore();
	~GLFW_EngineCore() override;

	//! Initializes window.
	/*!
	\param width width of window.
	\param height height of window.
	\param param windowName name of the window.
	*/
	bool initWindow(int width, int height, std::string windowName) override;

	//! Run the engine.
	/*!
	\param game reference to game class.
	*/
	bool runEngine(Game& game) override;

	//! Renders coloured background.
	/*!
	\param r red.
	\param g green.
	\param b blue.
	*/
	void renderColouredBackground(float r, float g, float b) override;

	//! Sets third person camera.
	/*!
	\param tpcam ThirdPersonCameraComponent pointer.
	*/
	void setThirdPersonCamera(ThirdPersonCameraComponent* tpcam) override;

	//! Sets first person camera.
	/*!
	\param fpcam FirstPersonCameraComponent pointer.
	*/
	void setFirstPersonCamera(FirstPersonCameraComponent* fpcam) override;

	//! Rendedrs frames per second.
	/*!
	*/
	void renderFPS();

	void displayTextToScreen(GLFWwindow * window) ;

	//! Renders text to screen.
	/*!
	\param text string that holds text value.
	\param x x position
	\param y y position
	\param scale scale of text
	\param colour colour of text
	*/
	void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) override;

	//! Draws model to screen.
	/*!
	\param model model pointer.
	\param modelMatrix the model matrix.
	*/
	void drawModel(Model* model, glm::mat4& modelMatrix) override;
	
	//! Getter function for GLFWwindow.
	/*!
	\returns m_window
	*/
	GLFWwindow* getGLFWwindow() { return m_window; }
	



};