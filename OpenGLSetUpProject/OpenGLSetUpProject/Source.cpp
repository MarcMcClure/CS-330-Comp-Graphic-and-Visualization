#include <GLEW\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

//glm lib import
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/string_cast.hpp>

//SOIL2 for image importation
#include <SOIL2/SOIL2.h>

using namespace std;

/** declare globals**/ 
//Window dims
int width = 600, height = 600;

//declare VAOs, VBOs, and EBOs
GLuint lightCubeVAO, lightCubeVBO, lightCubeEBO,
	tapeVAO, tapeVBO, tapeEBO, tapeTexture,
	orangeVAO, orangeVBO, orangeEBO, orangeTexture,
	bottleVAO, bottleVBO, bottleEBO,
	paintingVAO, paintingVBO, paintingEBO, paintingTexture,
	backgroundVAO, backgroundVBO, backgroundEBO,
	floorVAO, floorVBO, floorEBO, floorTexture,
	pyramidVAO, pyramidVBO, pyramidTexture;

//declair shaders
GLuint shaderProgramWithTex, 
	shaderProgramWithTexAndLighting,
	shaderProgramWithTexAndLightings,
	shaderProgramWithColor,
	shaderProgramWithColorAndLighting, 
	shaderProgramWithColorAndLightings,
	shaderProgramForLighting;

const double PI = 3.14159;
const float toRadians = PI / 180.0f;

//declair vew matrix
glm::mat4 viewMatrix, projectionMatrix;

// initilize VOF
GLfloat fov = 45.0f;

//define camera atributes
glm::vec3 cameraPosition = glm::vec3(0.0f, 5.0f, 20.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget); // points behind camera
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
glm::vec3 cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)); //define camera movement increment length?

// bool array for keys and mouce buttons
bool keys[1024], mouseButtons[3];

// boolean to check camera transformation
bool isPanning = false, isOrbiting = false;

//radius pitch and yaw for orbiting
GLfloat radius = 5.0f, degreesYaw, degreesPitch, radiansYaw, radiansPitch;

// variables for setting frame rate of imput and movement speed of camera?
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;
GLfloat lastX = width / 2, lastY = width / 2, xOffset, yOffset;

// variables for setting wasdqe movement speed
GLfloat panSpeed = 5.0f, panAcceleration = 0.1f;

//bool for determinging if mouce has moved
bool firstMouseMove = false;

//bool for determing wether to use orthographic pojection
bool displayOrtho = false;

//bool for determing permid or tree
bool treeMode = false;

//disco mode
bool discoMode = false;

// light sorce position and color
glm::vec3 lightPosition(3.0f, 15.0f, 8.0f);
glm::vec3 lightColor(1.0f, 0.98f, 0.9f);

glm::vec3 lightCenter(3.0f, 15.0f, 8.0f);
GLfloat lightDistinceFromCenter = 5.0f;
GLfloat lightrotationSpeed = 1.0f;
GLfloat lightrotationAngel = 0.0f;
glm::vec3 lightColors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
glm::vec3 lightPositions[3];

/**Function declerations**/
// input callback function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// camera, input processing function decleration
void TransormCamera();	//Camera Transformation prototype changes bool based on if iniput
void initCamera(); //resets camera
void printCamera(); //prints camera vatiables to cout
void processKeyboardInput();//function to process keyboard input to transform camera WASDQEFP

// shader functino declerations
//shader error checking
void PrintShaderCompileError(GLuint shader);
void PrintShaderLinkingError(int prog);
bool IsOpenGLError();
// create and compile shaders
static GLuint CompileShader(const string& source, GLuint shaderType);
//create shader object
static GLuint CreateShaderProgram(const string& vertexShaderSource, const string& fragmentShaderSource);

/**Draw functions**/
void drawTape(GLuint modelLoc) {

	glBindTexture(GL_TEXTURE_2D, tapeTexture); //bind tape texture
	glBindVertexArray(tapeVAO);//select VAO

		for (GLfloat i = -180.0; i < 180.0f; i+=18.0f)
		{

			glm::mat4 modelMatrix;

			modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.0f, 1.13f, -7.98f));
			modelMatrix = glm::rotate(modelMatrix, 45.0f * toRadians, glm::vec3(-1.0f, -1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, i * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1.3f, 1.3f, 1.3f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			//draw primitives
			GLenum mode = GL_TRIANGLES;
			GLsizei indices = 24;

			glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);
		}

	glBindVertexArray(0);//deselect VAO
	glBindTexture(GL_TEXTURE_2D, 0); //bind orange texture

}

void drawOrange(GLuint modelLoc) {

	glBindTexture(GL_TEXTURE_2D, orangeTexture); //bind orange texture
	glBindVertexArray(orangeVAO);//select VAO

		for (GLint j : {1, -1})
		{
			for (GLfloat i = -180.0; i < 180.0f; i += 18.0f)
			{
				glm::mat4 modelMatrix;

				modelMatrix = glm::translate(modelMatrix, glm::vec3(2.8f, 6.4f, -7.2f));
				modelMatrix = glm::rotate(modelMatrix, 75 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				modelMatrix = glm::rotate(modelMatrix, 15 * toRadians, glm::vec3(0.0f, -1.0f, 1.0f));
				modelMatrix = glm::rotate(modelMatrix, i * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(j * 1.0f, 1.0f, 1.0f));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

				//draw primitives
				GLenum mode = GL_TRIANGLES;
				GLuint first = 0;
				GLsizei count = 33;

				glDrawArrays(mode, first, count);
			}
		}

	glBindVertexArray(0);//deselect VAO
	glBindTexture(GL_TEXTURE_2D, 0); //unbind orange texture

}

void drawBottle(GLuint modelLoc) {

	glBindVertexArray(bottleVAO);//select VAO

		for (GLfloat i = -180.0; i < 180.0f; i += 18.0f)
		{
			glm::mat4 modelMatrix;

			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(7.0f, 0.0f, -18.0f));
			//modelMatrix = glm::rotate(modelMatrix, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::rotate(modelMatrix, i * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));				


			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			//draw primitives
			GLenum mode = GL_TRIANGLES;
			GLuint first = 0;
			GLsizei count = 132;

			glDrawArrays(mode, first, count);
		}

	glBindVertexArray(0);//deselect VAO

}

void drawBackground(GLuint modelLoc) {
	glBindVertexArray(backgroundVAO);//select VAO

	glm::mat4 modelMatrix;

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.1f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, 0.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	//draw primitives
	GLenum mode = GL_TRIANGLES;
	GLsizei indices = 6;

	glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);

	glBindVertexArray(0);//deselect VAO
}

void drawFloor(GLuint modelLoc) {

	glBindTexture(GL_TEXTURE_2D, floorTexture); //bind floor texture
	glBindVertexArray(floorVAO);//select VAO

		glm::mat4 modelMatrix;

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, 0.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		//draw primitives
		GLenum mode = GL_TRIANGLES;
		GLsizei indices = 6;

		glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);

	glBindVertexArray(0);//deselect VAO
	glBindTexture(GL_TEXTURE_2D, 0); //unbind floor texture
}

void drawPainting(GLuint modelLoc) {
	glBindTexture(GL_TEXTURE_2D, paintingTexture); //bind painting texture
	glBindVertexArray(paintingVAO);//select VAO

		glm::mat4 modelMatrix;

		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.0f, 0.0f, -8.6f));
		modelMatrix = glm::rotate(modelMatrix, -10.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		////draw primitives
		GLenum mode = GL_TRIANGLES;
		GLsizei indices = 36;

		glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);

	glBindVertexArray(0);//deselect VAO
	glBindTexture(GL_TEXTURE_2D, 0); //unbind painting texture
}

void drawPyramid(GLuint modelLoc) {
	glBindTexture(GL_TEXTURE_2D, pyramidTexture); //bind pyramid texture

	glBindVertexArray(pyramidVAO); //select VAO will remain selected untill called again

	for (GLfloat j = 1; j < (treeMode ? 8 : 1); j++) {
		for (GLfloat i = 0; i < 4; i++)
		{
			glm::mat4 modelMatrix;

			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -j * 2 / 3 + 4.71, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, j * 125.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, i * 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(sqrt(j), sqrt(j), sqrt(j)));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			//draw primitives
			GLenum mode = GL_TRIANGLES;
			GLuint first = 0;
			GLsizei count = 6;

			glDrawArrays(mode, first, count);
		}
	}

	glBindVertexArray(0); //deselect VAO

	glBindTexture(GL_TEXTURE_2D, 0); //unbind pyramid texture
}

void drawLightCube(GLuint modelLoc, glm::vec3 position) {

	glm::vec3 scaleFactor = glm::vec3(0.5f, 0.5f, 0.5f);

	glBindVertexArray(lightCubeVAO); //select VAO will remain selected untill called again

	glm::float32 planeRotations[] = {
		0.0f, 90.0f, 180.0f, -90.0f, -90.f, 90.f
	};
	glm::vec3 planePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.5f),
		glm::vec3(0.5f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  -0.5f),
		glm::vec3(-0.5f, 0.0f,  0.0f),
		glm::vec3(0.0f, 0.5f,  0.0f),
		glm::vec3(0.0f, -0.5f,  0.0f)
	};

	for (GLuint i = 0; i < 6; i++)
	{
		glm::mat4 modelMatrix;

		modelMatrix = glm::translate(modelMatrix, planePositions[i] * scaleFactor + position);
		modelMatrix = glm::rotate(modelMatrix, planeRotations[i] * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, scaleFactor);
		if (i >= 4)
			modelMatrix = glm::rotate(modelMatrix, planeRotations[i] * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		//draw primitives
		GLenum mode = GL_TRIANGLES;
		GLsizei indices = 6;

		glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);
	}

	glBindVertexArray(pyramidVAO); //deselect VAO
}

void draw() {

	if (discoMode) {

		glUseProgram(shaderProgramForLighting);//select shader program for drawing lighting

			//select uniform variable and shader
			GLuint modelLocForLighting = glGetUniformLocation(shaderProgramForLighting, "model");
			GLuint viewLocForLighting = glGetUniformLocation(shaderProgramForLighting, "view");
			GLuint projectionLocForLighting = glGetUniformLocation(shaderProgramForLighting, "projection");
			GLuint lightColorLocForLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "lightColor");

			//pass transform to shader
			glUniformMatrix4fv(viewLocForLighting, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocForLighting, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			for (int i = 0; i < 3; i++) {
				// assign object color
				glUniform3f(lightColorLocForLighting, lightColors[i].r, lightColors[i].g, lightColors[i].b);

				drawLightCube(modelLocForLighting, lightPositions[i]);
			}

		glUseProgram(0);//deselect shader program

		glUseProgram(shaderProgramWithTexAndLightings);//use shader program for drawing objects

			//select uniform variable and shader
			GLuint modelLocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "model");
			GLuint viewLocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "view");
			GLuint projectionLocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "projection");
			GLuint objectColorLocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "objectColor");
			GLuint lightColor0LocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "lightColor[0]");
			GLuint lightPos0LocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "lightPos[0]");
			GLuint lightColor1LocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "lightColor[1]");
			GLuint lightPos1LocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "lightPos[1]");
			GLuint lightColor2LocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "lightColor[2]");
			GLuint lightPos2LocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "lightPos[2]");
			GLuint viewPosLocWithTexAndLightings = glGetUniformLocation(shaderProgramWithTexAndLightings, "viewPos");

			// assign object colors and camera position
			glUniform3f(objectColorLocWithTexAndLightings, 1.0f, 1.0f, 1.0f);
			glUniform3f(viewPosLocWithTexAndLightings, cameraPosition.x, cameraPosition.y, cameraPosition.z);

			// assign light color and position
			glUniform3f(lightColor0LocWithTexAndLightings, lightColors[0].r, lightColors[0].g, lightColors[0].b);
			glUniform3f(lightPos0LocWithTexAndLightings, lightPositions[0].x, lightPositions[0].y, lightPositions[0].z);
			glUniform3f(lightColor1LocWithTexAndLightings, lightColors[1].r, lightColors[1].g, lightColors[1].b);
			glUniform3f(lightPos1LocWithTexAndLightings, lightPositions[1].x, lightPositions[1].y, lightPositions[1].z);
			glUniform3f(lightColor2LocWithTexAndLightings, lightColors[2].r, lightColors[2].g, lightColors[2].b);
			glUniform3f(lightPos2LocWithTexAndLightings, lightPositions[2].x, lightPositions[2].y, lightPositions[2].z);

			//pass transform to shader
			glUniformMatrix4fv(viewLocWithTexAndLightings, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocWithTexAndLightings, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			drawPyramid(modelLocWithTexAndLightings);
			drawOrange(modelLocWithTexAndLightings);
			drawPainting(modelLocWithTexAndLightings);
			drawFloor(modelLocWithTexAndLightings);
			drawTape(modelLocWithTexAndLightings);

		glUseProgram(0);//deselect shader program

		glUseProgram(shaderProgramWithColorAndLightings);//use shader program for drawing objects

			//select uniform variable and shader
			GLuint modelLocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "model");
			GLuint viewLocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "view");
			GLuint projectionLocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "projection");
			GLuint objectColorLocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "objectColor");
			GLuint lightColor0LocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "lightColor[0]");
			GLuint lightPos0LocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "lightPos[0]");
			GLuint lightColor1LocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "lightColor[1]");
			GLuint lightPos1LocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "lightPos[1]");
			GLuint lightColor2LocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "lightColor[2]");
			GLuint lightPos2LocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "lightPos[2]");
			GLuint viewPosLocWithColorAndLightings = glGetUniformLocation(shaderProgramWithColorAndLightings, "viewPos");

			// assign object colors and camera position
			glUniform3f(objectColorLocWithColorAndLightings, 1.0f, 1.0f, 1.0f);
			glUniform3f(viewPosLocWithColorAndLightings, cameraPosition.x, cameraPosition.y, cameraPosition.z);

			// assign light color and position
			glUniform3f(lightColor0LocWithColorAndLightings, lightColors[0].r, lightColors[0].g, lightColors[0].b);
			glUniform3f(lightPos0LocWithColorAndLightings, lightPositions[0].x, lightPositions[0].y, lightPositions[0].z);
			glUniform3f(lightColor1LocWithColorAndLightings, lightColors[1].r, lightColors[1].g, lightColors[1].b);
			glUniform3f(lightPos1LocWithColorAndLightings, lightPositions[1].x, lightPositions[1].y, lightPositions[1].z);
			glUniform3f(lightColor2LocWithColorAndLightings, lightColors[2].r, lightColors[2].g, lightColors[2].b);
			glUniform3f(lightPos2LocWithColorAndLightings, lightPositions[2].x, lightPositions[2].y, lightPositions[2].z);

			//pass transform to shader
			glUniformMatrix4fv(viewLocWithColorAndLightings, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocWithColorAndLightings, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			drawBottle(modelLocWithColorAndLightings);
			drawBackground(modelLocWithColorAndLightings);

		glUseProgram(0);//deselect shader program
	}
	else {

		glUseProgram(shaderProgramForLighting);//select shader program for drawing lighting

			//select uniform variable and shader
			GLuint modelLocForLighting = glGetUniformLocation(shaderProgramForLighting, "model");
			GLuint viewLocForLighting = glGetUniformLocation(shaderProgramForLighting, "view");
			GLuint projectionLocForLighting = glGetUniformLocation(shaderProgramForLighting, "projection");
			GLuint lightColorLocForLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "lightColor");

			// assign light and object colors
			glUniform3f(lightColorLocForLighting, lightColor.r, lightColor.g, lightColor.b);

			//pass transform to shader
			glUniformMatrix4fv(viewLocForLighting, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocForLighting, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			drawLightCube(modelLocForLighting, lightPosition);

		glUseProgram(0);//deselect shader program

		glUseProgram(shaderProgramWithTexAndLighting);//use shader program for drawing objects

			//select uniform variable and shader
			GLuint modelLocWithTexAndLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "model");
			GLuint viewLocWithTexAndLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "view");
			GLuint projectionLocWithTexAndLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "projection");
			GLuint objectColorLocWithTexAndLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "objectColor");
			GLuint lightColorLocWithTexAndLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "lightColor");
			GLuint lightPosLocWithTexAndLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "lightPos");
			GLuint viewPosLocWithTexAndLighting = glGetUniformLocation(shaderProgramWithTexAndLighting, "viewPos");

			// assign light and object colors
			glUniform3f(objectColorLocWithTexAndLighting, 1.0f, 1.0f, 1.0f);
			glUniform3f(lightColorLocWithTexAndLighting, lightColor.r, lightColor.g, lightColor.b);
			glUniform3f(lightPosLocWithTexAndLighting, lightPosition.x, lightPosition.y, lightPosition.z);
			glUniform3f(viewPosLocWithTexAndLighting, cameraPosition.x, cameraPosition.y, cameraPosition.z);

			//pass transform to shader
			glUniformMatrix4fv(viewLocWithTexAndLighting, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocWithTexAndLighting, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			drawPyramid(modelLocWithTexAndLighting);
			drawOrange(modelLocWithTexAndLighting);
			drawPainting(modelLocWithTexAndLighting);
			drawFloor(modelLocWithTexAndLighting);
			drawTape(modelLocWithTexAndLighting);

		glUseProgram(0);//deselect shader program

		glUseProgram(shaderProgramWithColorAndLighting);//use shader program for drawing objects

		//select uniform variable and shader
		GLuint modelLocWithColorAndLighting = glGetUniformLocation(shaderProgramWithColorAndLighting, "model");
		GLuint viewLocWithColorAndLighting = glGetUniformLocation(shaderProgramWithColorAndLighting, "view");
		GLuint projectionLocWithColorAndLighting = glGetUniformLocation(shaderProgramWithColorAndLighting, "projection");
		GLuint objectColorLocWithColorAndLighting = glGetUniformLocation(shaderProgramWithColorAndLighting, "objectColor");
		GLuint lightColorLocWithColorAndLighting = glGetUniformLocation(shaderProgramWithColorAndLighting, "lightColor");
		GLuint lightPosLocWithColorAndLighting = glGetUniformLocation(shaderProgramWithColorAndLighting, "lightPos");
		GLuint viewPosLocWithColorAndLighting = glGetUniformLocation(shaderProgramWithColorAndLighting, "viewPos");

		// assign light and object colors
		glUniform3f(objectColorLocWithColorAndLighting, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightColorLocWithColorAndLighting, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(lightPosLocWithColorAndLighting, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform3f(viewPosLocWithColorAndLighting, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		//pass transform to shader
		glUniformMatrix4fv(viewLocWithColorAndLighting, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocWithColorAndLighting, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		drawBottle(modelLocWithColorAndLighting);
		drawBackground(modelLocWithColorAndLighting);

		glUseProgram(0);//deselect shader program

		glUseProgram(shaderProgramWithTex);//use shader program for drawing objects

		//select uniform variable and shader
		GLuint modelLocWithTex = glGetUniformLocation(shaderProgramWithTex, "model");
		GLuint viewLocWithTex = glGetUniformLocation(shaderProgramWithTex, "view");
		GLuint projectionLocWithTex = glGetUniformLocation(shaderProgramWithTex, "projection");

		//pass transform to shader
		glUniformMatrix4fv(viewLocWithTex, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocWithTex, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		drawPyramid(projectionLocWithTex);

		glUseProgram(0);//deselect shader program

		glUseProgram(shaderProgramWithColor);//use shader program for drawing objects

		//select uniform variable and shader
		GLuint modelLocWithColor = glGetUniformLocation(shaderProgramWithColor, "model");
		GLuint viewLocWithColor = glGetUniformLocation(shaderProgramWithColor, "view");
		GLuint projectionLocWithColor = glGetUniformLocation(shaderProgramWithColor, "projection");

		//pass transform to shader
		glUniformMatrix4fv(viewLocWithColor, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocWithColor, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		//drawBackground(modelLocWithColor);

		glUseProgram(0);//deselect shader program
	}
}

/**Shader functions**/
/* GLSL Error Checking Definitions */
void PrintShaderCompileError(GLuint shader)
{
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWritten, log);
		cout << "Shader Compile Error: " << log << endl;
		free(log);
	}
}

void PrintShaderLinkingError(int prog)
{
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(prog, len, &chWritten, log);
		cout << "Shader Linking Error: " << log << endl;
		free(log);
	}
}

bool IsOpenGLError()
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

static GLuint CompileShader(const string& source, GLuint shaderType)
{
	//cerate shader object
	GLuint shaderID = glCreateShader(shaderType);
	const char* src = source.c_str();

	//attach sorce to shader object
	glShaderSource(shaderID, 1, &src, nullptr);

	//compile shader
	glCompileShader(shaderID);

	/* Shader Compliation Error Check */
	GLint shaderCompiled;
	IsOpenGLError();
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != 1)
	{
		cout << "Shader Compilation Failed!" << endl;
		PrintShaderCompileError(shaderID);
	}
	/* End here */

	//return ID of compiled shader
	return shaderID;
}

static GLuint CreateShaderProgram(const string& vertexShaderSource, const string& fragmentShaderSource)
{
	// compile vertex shader
	GLuint vertexShaderCompiled = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);


	// compile fragment shader
	GLuint FragmentShaderCompiled = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	//create program object
	GLuint shaderProgram = glCreateProgram();

	//attach compiled shaders to program object
	glAttachShader(shaderProgram, vertexShaderCompiled);
	glAttachShader(shaderProgram, FragmentShaderCompiled);

	//link shaders to create excutiable
	glLinkProgram(shaderProgram);

	/* Shader Linking Error Check */
	GLint linked;
	IsOpenGLError();
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		cout << "Shader Linking Failed!" << endl;
		PrintShaderLinkingError(shaderProgram);
	}
	/* End here */

	//valadate shader (if desired)not nesesairy
	glValidateProgram(shaderProgram);

	//delete compiled shaders
	glDeleteShader(vertexShaderCompiled);
	glDeleteShader(FragmentShaderCompiled);

	//return shader program
	return shaderProgram;
}

void CreateShaders(){

	/** shader for light **/
	//vertex shader soce code
	string vertexShaderSourceForLighting =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"

		"void main()\n"
		"{\n"
			"gl_Position = projection * view * model * vec4(vPosition.x,vPosition.y,vPosition.z,1.0);"
		"}\n";
	//fragment shader sorce code
	string fragmentShaderSourceForLighting =
		"#version 330 core\n"
		"out vec4 fragColor;"

		"uniform vec3 lightColor;"

		"void main()\n"
		"{\n"
			"fragColor = vec4(lightColor.r,lightColor.g,lightColor.b,1.0f);"
		"}\n";
	//reate shader program
	shaderProgramForLighting = CreateShaderProgram(vertexShaderSourceForLighting, fragmentShaderSourceForLighting);

	/** shaders with texture and lighting **/
	//vertex shader soce code
	string vertexShaderSourceWithTexAndLighting =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;"
		"layout(location = 1) in vec4 aColor;"
		"layout(location = 2) in vec2 texCoord;"
		"layout(location = 3) in vec3 normal;"

		"out vec4 oColor;"
		"out vec2 oTexCoord;"
		"out vec3 oNormal;"
		"out vec3 fragPos;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"

		"void main()\n"
		"{\n"
			"gl_Position = projection * view * model * vec4(vPosition,1.0f);"
			"oColor = aColor;"
			"oTexCoord = texCoord;"
			"oNormal = mat3(transpose(inverse(model))) * normal;"	//handles lighting of non uniform scaled obects
			"fragPos = vec3(model * vec4(vPosition,1.0f));"
		"}\n";
	//fragment shader sorce code
	string fragmentShaderSourceWithTexAndLighting =
		"#version 330 core\n"

		"in vec4 oColor;"
		"in vec2 oTexCoord;"
		"in vec3 oNormal;"
		"in vec3 fragPos;"

		"out vec4 fragColor;"

		"uniform sampler2D myTexture;"
		"uniform vec3 objectColor;"
		"uniform vec3 lightColor;"
		"uniform vec3 lightPos;"
		"uniform vec3 viewPos;"

		"void main()\n"
		"{\n"
			"//Ambient lighting component\n"
			"float ambientStrength = 0.25f;"
			"vec3 ambient = ambientStrength * lightColor;"

			"//diffuse lighting component\n"
			"float diffuseStrength = 1.0f;"
			"vec3 norm = normalize(oNormal);"
			"vec3 lightDir = normalize(lightPos - fragPos);"
			"float diff = max(dot(norm,lightDir),0.0);"
			"vec3 diffuse = diffuseStrength * diff * lightColor;"

			"//specular lighting component\n"
			"float specularStrength = 0.5f;"
			"int specularSize = 32;"
			"vec3 viewDir = normalize(viewPos - fragPos);"
			"vec3 reflectDir = reflect(-lightDir,norm);"
			"float spec = pow(max(dot(viewDir,reflectDir),0.0),specularSize);"
			"vec3 specular = specularStrength * spec * lightColor;"

			"vec3 result = (ambient + diffuse + specular) * objectColor;"
			"fragColor = texture(myTexture, oTexCoord) * vec4(result, 1.0);"
		"}\n";
	//reate shader program
	shaderProgramWithTexAndLighting = CreateShaderProgram(vertexShaderSourceWithTexAndLighting, fragmentShaderSourceWithTexAndLighting);

	/** shaders with texture and lightings **/
	//vertex shader soce code
	string vertexShaderSourceWithTexAndLightings =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;"
		"layout(location = 1) in vec4 aColor;"
		"layout(location = 2) in vec2 texCoord;"
		"layout(location = 3) in vec3 normal;"

		"out vec4 oColor;"
		"out vec2 oTexCoord;"
		"out vec3 oNormal;"
		"out vec3 fragPos;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"

		"void main()\n"
		"{\n"
		"gl_Position = projection * view * model * vec4(vPosition,1.0f);"
		"oColor = aColor;"
		"oTexCoord = texCoord;"
		"oNormal = mat3(transpose(inverse(model))) * normal;"	//handles lighting of non uniform scaled obects
		"fragPos = vec3(model * vec4(vPosition,1.0f));"
		"}\n";
	//fragment shader sorce code
	string fragmentShaderSourceWithTexAndLightings =
		"#version 330 core\n"

		"in vec4 oColor;"
		"in vec2 oTexCoord;"
		"in vec3 oNormal;"
		"in vec3 fragPos;"

		"out vec4 fragColor;"

		"uniform sampler2D myTexture;"
		"uniform vec3 objectColor;"
		"uniform vec3 lightColor[3];"
		"uniform vec3 lightPos[3];"
		"uniform vec3 viewPos;"

		"void main()\n"
		"{\n"
			"vec3 result;"
			"for (int i = 0; i < 3; i++){\n"
				"int j = i;"

				"//Ambient lighting component\n"
				"float ambientStrength = 0.25f;"
				"vec3 ambient = ambientStrength * lightColor[i];"

				"//diffuse lighting component\n"
				"float diffuseStrength = 1.0f;"
				"vec3 norm = normalize(oNormal);"
				"vec3 lightDir = normalize(lightPos[i] - fragPos);"
				"float diff = max(dot(norm,lightDir),0.0);"
				"vec3 diffuse = diffuseStrength * diff * lightColor[i];"

				"//specular lighting component\n"
				"float specularStrength = 2.0f;"
				"int specularSize = 32;"
				"vec3 viewDir = normalize(viewPos - fragPos);"
				"vec3 reflectDir = reflect(-lightDir,norm);"
				"float spec = pow(max(dot(viewDir,reflectDir),0.0),specularSize);"
				"vec3 specular = specularStrength * spec * lightColor[i];"

				"result = result + (ambient + diffuse + specular) * objectColor;"
			"};"
			"fragColor = texture(myTexture, oTexCoord) * vec4(result, 1.0);"
		"}\n";
	//create shader program
	shaderProgramWithTexAndLightings = CreateShaderProgram(vertexShaderSourceWithTexAndLightings, fragmentShaderSourceWithTexAndLightings);

	/** shader with texture **/
	//vertex shader soce code
	string vertexShaderSourceWithTex =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;"
		"layout(location = 1) in vec4 aColor;"
		"layout(location = 2) in vec2 texCoord;"
		"out vec4 oColor;"
		"out vec2 oTexCoord;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main()\n"
		"{\n"
		"oColor = aColor;"
		"oTexCoord = texCoord;"
		"gl_Position = projection * view * model * vec4(vPosition,1.0);"
		"}\n";
	//fragment shader sorce code
	string fragmentShaderSourceWithTex =
		"#version 330 core\n"
		"in vec4 oColor;"
		"in vec2 oTexCoord;"
		"out vec4 fragColor;"
		"uniform sampler2D myTexture;"
		"void main()\n"
		"{\n"
		"fragColor = texture(myTexture, oTexCoord);"
		"}\n";
	//reate shader program
	shaderProgramWithTex = CreateShaderProgram(vertexShaderSourceWithTex, fragmentShaderSourceWithTex);

	/** shader with just color **/
	//vertex shader soce code
	string vertexShaderSourceWithOutTex =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;"
		"layout(location = 1) in vec4 aColor;"
		"out vec4 oColor;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main()\n"
		"{\n"
		"oColor = aColor;"
		"gl_Position = projection * view * model * vec4(vPosition,1.0);"
		"}\n";
	//fragment shader sorce code
	string fragmentShaderSourceWithOutTex =
		"#version 330 core\n"
		"in vec4 oColor;"
		"out vec4 fragColor;"
		"void main()\n"
		"{\n"
		"fragColor = oColor;"
		"}\n";
	//reate shader program
	shaderProgramWithColor = CreateShaderProgram(vertexShaderSourceWithOutTex, fragmentShaderSourceWithOutTex);

	/** shader with color and light **/
	//vertex shader soce code
	string vertexShaderSourceWithColorAndLighting =
		"#version 330 core\n"

		"layout(location = 0) in vec3 vPosition;"
		"layout(location = 1) in vec4 aColor;"
		"layout(location = 2) in vec3 normal;"

		"out vec4 oColor;"
		"out vec3 oNormal;"
		"out vec3 fragPos;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"

		"void main()\n"
		"{\n"
			"gl_Position = projection * view * model * vec4(vPosition,1.0);"
			"oColor = aColor;"
			"oNormal = mat3(transpose(inverse(model))) * normal;"	//handles lighting of non uniform scaled obects
			"fragPos = vec3(model * vec4(vPosition,1.0f));"
		"}\n";
	//fragment shader sorce code
	string fragmentShaderSourceWithColorAndLighting =
		"#version 330 core\n"

		"in vec4 oColor;"
		"in vec3 oNormal;"
		"in vec3 fragPos;"

		"out vec4 fragColor;"

		"uniform vec3 objectColor;"
		"uniform vec3 lightColor;"
		"uniform vec3 lightPos;"
		"uniform vec3 viewPos;"

		"void main()\n"
		"{\n"
			"//Ambient lighting component\n"
			"float ambientStrength = 0.25f;"
			"vec3 ambient = ambientStrength * lightColor;"

			"//diffuse lighting component\n"
			"vec3 norm = normalize(oNormal);"
			"vec3 lightDir = normalize(lightPos - fragPos);"
			"float diff = max(dot(norm,lightDir),0.0);"
			"vec3 diffuse = diff * lightColor;"

			"//specular lighting component\n"
			"float specularStrength = 5.5f;"
			"int specularSize = 512;"
			"vec3 viewDir = normalize(viewPos - fragPos);"
			"vec3 reflectDir = reflect(-lightDir,norm);"
			"float spec = pow(max(dot(viewDir,reflectDir),0.0),specularSize);"
			"vec3 specular = specularStrength * spec * lightColor;"

			"vec3 result = (ambient + diffuse + specular) * objectColor;"
			"fragColor = oColor * vec4(result, 1.0);"
		"}\n";
	//reate shader program
	shaderProgramWithColorAndLighting = CreateShaderProgram(vertexShaderSourceWithColorAndLighting, fragmentShaderSourceWithColorAndLighting);

	/** shader with color and lights **/
	//vertex shader soce code
	string vertexShaderSourceWithColorAndLightings =
		"#version 330 core\n"

		"layout(location = 0) in vec3 vPosition;"
		"layout(location = 1) in vec4 aColor;"
		"layout(location = 2) in vec3 normal;"

		"out vec4 oColor;"
		"out vec3 oNormal;"
		"out vec3 fragPos;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"

		"void main()\n"
		"{\n"
		"gl_Position = projection * view * model * vec4(vPosition,1.0);"
		"oColor = aColor;"
		"oNormal = mat3(transpose(inverse(model))) * normal;"	//handles lighting of non uniform scaled obects
		"fragPos = vec3(model * vec4(vPosition,1.0f));"
		"}\n";
	//fragment shader sorce code
	string fragmentShaderSourceWithColorAndLightings =
		"#version 330 core\n"

		"in vec4 oColor;"
		"in vec3 oNormal;"
		"in vec3 fragPos;"

		"out vec4 fragColor;"

		"uniform vec3 objectColor;"
		"uniform vec3 lightColor[3];"
		"uniform vec3 lightPos[3];"
		"uniform vec3 viewPos;"

		"void main()\n"
		"{\n"
			"vec3 result;"
			"for (int i = 0; i < 3; i++){\n"
				"int j = i;"
				"//Ambient lighting component\n"
				"float ambientStrength = 0.25f;"
				"vec3 ambient = ambientStrength * lightColor[j];"

				"//diffuse lighting component\n"
				"vec3 norm = normalize(oNormal);"
				"vec3 lightDir = normalize(lightPos[j] - fragPos);"
				"float diff = max(dot(norm,lightDir),0.0);"
				"vec3 diffuse = diff * lightColor[j];"

				"//specular lighting component\n"
				"float specularStrength = 5.5f;"
				"int specularSize = 512;"
				"vec3 viewDir = normalize(viewPos - fragPos);"
				"vec3 reflectDir = reflect(-lightDir,norm);"
				"float spec = pow(max(dot(viewDir,reflectDir),0.0),specularSize);"
				"vec3 specular = specularStrength * spec * lightColor[j];"

				"result = result + (ambient + diffuse + specular) * objectColor;"
			"};"
			"fragColor = oColor * vec4(result, 1.0);"
		"}\n";
	//reate shader program
	shaderProgramWithColorAndLightings = CreateShaderProgram(vertexShaderSourceWithColorAndLightings, fragmentShaderSourceWithColorAndLightings);
}

void IncrementLightPosition() {
	lightrotationAngel += lightrotationSpeed * deltaTime;
	if (lightrotationAngel > 2 * PI) lightrotationAngel -= 2 * PI;
	lightPositions[0] = lightCenter + lightDistinceFromCenter * glm::vec3(sin(lightrotationAngel + 2.0 * PI / 3.0), cos(lightrotationAngel + 2.0 * PI / 3.0),0.0f);
	lightPositions[1] = lightCenter + lightDistinceFromCenter * glm::vec3(sin(lightrotationAngel), cos(lightrotationAngel), 0.0f);
	lightPositions[2] = lightCenter + lightDistinceFromCenter * glm::vec3(sin(lightrotationAngel - 2.0 * PI / 3.0), cos(lightrotationAngel - 2.0 * PI / 3.0), 0.0f);
}

/**Create Vertices functions**/
void CreateTapeVertices() {

	//GLfloat innerRadious = 0.5; //outer radious is always 1
	//GLfloat height = 0.7; // height of tape
	//const GLfloat tan9 = 0.15838444; //const used to calculate the width of 20 sided cylinder segment
	//GLfloat yInnerSegment = innerRadious * tan9;
	//GLfloat yOuterSegment = tan9;

	//// vertex data
	//GLfloat vertices[] = {
	//	//position				color							texture maping	normal data
	//	//x,			y,				z		r		g		b		a		s		t		x		y		z
	//	//back
	//	innerRadious,	yInnerSegment,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.33,	0.00,	0.00,	-1.00,
	//	innerRadious,	-yInnerSegment,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.33,	0.00,	0.00,	-1.00,
	//	1.0,			yOuterSegment,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.66,	0.00,	0.00,	-1.00,
	//	1.0,			-yOuterSegment,	0.0,	0.0,	1.0,	0.0,	1.0,	1.0,	0.66,	0.00,	0.00,	-1.00,
	//	//front
	//	innerRadious,	yInnerSegment,	height,	1.0,	1.0,	1.0,	1.0,	0.0,	0.33,	0.00,	0.00,	1.00,
	//	innerRadious,	-yInnerSegment,	height,	1.0,	1.0,	1.0,	1.0,	1.0,	0.33,	0.00,	0.00,	1.00,
	//	1.0,			yOuterSegment,	height,	0.0,	1.0,	0.0,	1.0,	0.0,	0.66,	0.00,	0.00,	1.00,
	//	1.0,			-yOuterSegment,	height,	0.0,	1.0,	0.0,	1.0,	1.0,	0.66,	0.00,	0.00,	1.00,
	//	//inside
	//	innerRadious,	yInnerSegment,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	-1.00,	0.00,	0.00,
	//	innerRadious,	-yInnerSegment,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	-1.00,	0.00,	0.00,
	//	innerRadious,	yInnerSegment,	height,	1.0,	1.0,	1.0,	1.0,	0.0,	0.33,	-1.00,	0.00,	0.00,
	//	innerRadious,	-yInnerSegment,	height,	1.0,	1.0,	1.0,	1.0,	1.0,	0.33,	-1.00,	0.00,	0.00,
	//	//outside
	//	1.0,			yOuterSegment,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.66,	1.00,	0.00,	0.00,
	//	1.0,			-yOuterSegment,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	1.00,	1.00,	0.00,	0.00,
	//	1.0,			yOuterSegment,	height,	0.0,	1.0,	0.0,	1.0,	1.0,	0.66,	1.00,	0.00,	0.00,
	//	1.0,			-yOuterSegment,	height,	0.0,	1.0,	0.0,	1.0,	1.0,	1.00,	1.00,	0.00,	0.00
	//};

	// vertex data
	GLfloat vertices[] = {
		//position				color							texture maping	normal data
		//x,	y,		z		r		g		b		a		s		t		x		y		z
		//back
		0.6,	0.095,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.678,	0.00,	0.00,	-1.00,
		0.6,	-0.095,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.678,	0.00,	0.00,	-1.00,
		1.0,	0.159,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.343,	0.00,	0.00,	-1.00,
		1.0,	-0.159,	0.0,	0.0,	1.0,	0.0,	1.0,	1.0,	0.343,	0.00,	0.00,	-1.00,
		//front
		0.6,	0.095,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.678,	0.00,	0.00,	1.00,
		0.6,	-0.095,	1.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.678,	0.00,	0.00,	1.00,
		1.0,	0.159,	1.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.343,	0.00,	0.00,	1.00,
		1.0,	-0.159,	1.0,	0.0,	1.0,	0.0,	1.0,	1.0,	0.343,	0.00,	0.00,	1.00,
		//inside
		0.6,	0.095,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	1.00,	-1.00,	0.00,	0.00,
		0.6,	-0.095,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	1.00,	-1.00,	0.00,	0.00,
		0.6,	0.095,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.678,	-1.00,	0.00,	0.00,
		0.6,	-0.095,	1.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.678,	-1.00,	0.00,	0.00,
		//outside
		1.0,	0.159,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.343,	1.00,	0.00,	0.00,
		1.0,	-0.159,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.00,	1.00,	0.00,	0.00,
		1.0,	0.159,	1.0,	0.0,	1.0,	0.0,	1.0,	1.0,	0.343,	1.00,	0.00,	0.00,
		1.0,	-0.159,	1.0,	0.0,	1.0,	0.0,	1.0,	1.0,	0.00,	1.00,	0.00,	0.00
	};

	//index data
	GLubyte indicies[] ={
		//front
		4,5,7,
		4,6,7,
		//outside
		12,13,14,
		13,14,15,
		//back
		0,1,2,
		1,2,3,
		//inside
		8,9,10,
		9,10,11
	};

	glGenBuffers(1, &tapeVBO); //make VBO
	glGenBuffers(1, &tapeEBO); //make EBO
	glGenVertexArrays(1, &tapeVAO);//make VAO

	glBindVertexArray(tapeVAO); //select VAO will remain selected untill called again

		glBindBuffer(GL_ARRAY_BUFFER, tapeVBO); //select VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tapeEBO); //select EBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); //load indicies

		// specify vertex array (VA) location and layoit to gpu
		//position data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//color data
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		//texture data
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(7 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);
		//normal data
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(9 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(3);

	glBindVertexArray(0); //deselect VAO
}

void CreateOrangeVertices() {

	// vertex data
	GLfloat vertices[] = {
		//position				color							texture maping	normal data
		//x,	y,		z		r		g		b		a		s		t		x		y		z
		1.000,	0.000,	0.000,	0.300,	0.300,	0.000,	1.000,	0.050,	0.000,	0.002,	0.000,	0.000,
		0.996,	0.078,	0.013,	0.300,	0.300,	0.000,	1.000,	0.000,	0.100,	0.002,	0.000,	0.000,
		0.996,	0.078,	-0.013,	0.300,	0.300,	0.000,	1.000,	0.100,	0.100,	0.002,	0.000,	0.000,

		0.996,	0.078,	0.013,	0.950,	0.500,	0.000,	1.000,	0.513,	0.000,	0.007,	0.001,	0.000,
		0.996,	0.078,	-0.013,	0.950,	0.500,	0.000,	1.000,	0.487,	0.000,	0.007,	0.001,	0.000,
		0.962,	0.365,	0.060,	0.950,	0.500,	0.000,	1.000,	0.560,	0.200,	0.007,	0.001,	0.000,

		0.996,	0.078,	-0.013,	0.950,	0.500,	0.000,	1.000,	0.487,	0.000,	0.034,	0.004,	0.000,
		0.962,	0.365,	0.060,	0.950,	0.500,	0.000,	1.000,	0.560,	0.200,	0.034,	0.004,	0.000,
		0.962,	0.365,	-0.060,	0.950,	0.500,	0.000,	1.000,	0.440,	0.200,	0.034,	0.004,	0.000,

		0.962,	0.365,	0.060,	0.950,	0.500,	0.000,	1.000,	0.560,	0.200,	0.032,	0.022,	0.000,
		0.962,	0.365,	-0.060,	0.950,	0.500,	0.000,	1.000,	0.440,	0.200,	0.032,	0.022,	0.000,
		0.777,	0.629,	0.101,	0.950,	0.500,	0.000,	1.000,	0.601,	0.400,	0.032,	0.022,	0.000,

		0.962,	0.365,	-0.060,	0.950,	0.500,	0.000,	1.000,	0.440,	0.200,	0.053,	0.037,	0.000,
		0.777,	0.629,	0.101,	0.950,	0.500,	0.000,	1.000,	0.601,	0.400,	0.053,	0.037,	0.000,
		0.777,	0.629,	-0.101,	0.950,	0.500,	0.000,	1.000,	0.399,	0.400,	0.053,	0.037,	0.000,

		0.777,	0.629,	0.101,	0.950,	0.500,	0.000,	1.000,	0.601,	0.400,	0.040,	0.044,	0.000,
		0.777,	0.629,	-0.101,	0.950,	0.500,	0.000,	1.000,	0.399,	0.400,	0.040,	0.044,	0.000,
		0.559,	0.829,	0.132,	0.950,	0.500,	0.000,	1.000,	0.632,	0.600,	0.040,	0.044,	0.000,

		0.777,	0.629,	-0.101,	0.950,	0.500,	0.000,	1.000,	0.399,	0.400,	0.053,	0.058,	0.000,
		0.559,	0.829,	0.132,	0.950,	0.500,	0.000,	1.000,	0.632,	0.600,	0.053,	0.058,	0.000,
		0.559,	0.829,	-0.132,	0.950,	0.500,	0.000,	1.000,	0.368,	0.600,	0.053,	0.058,	0.000,

		0.559,	0.829,	0.132,	0.950,	0.500,	0.000,	1.000,	0.632,	0.600,	0.034,	0.086,	0.000,
		0.559,	0.829,	-0.132,	0.950,	0.500,	0.000,	1.000,	0.368,	0.600,	0.034,	0.086,	0.000,
		0.232,	0.956,	0.152,	0.950,	0.500,	0.000,	1.000,	0.652,	0.800,	0.034,	0.086,	0.000,

		0.559,	0.829,	-0.132,	0.950,	0.500,	0.000,	1.000,	0.368,	0.600,	0.039,	0.099,	0.000,
		0.232,	0.956,	0.152,	0.950,	0.500,	0.000,	1.000,	0.652,	0.800,	0.039,	0.099,	0.000,
		0.232,	0.956,	-0.152,	0.950,	0.500,	0.000,	1.000,	0.348,	0.800,	0.039,	0.099,	0.000,

		0.232,	0.956,	0.152,	0.950,	0.500,	0.000,	1.000,	0.652,	0.800,	0.013,	0.071,	0.000,
		0.232,	0.956,	-0.152,	0.950,	0.500,	0.000,	1.000,	0.348,	0.800,	0.013,	0.071,	0.000,
		0.000,	1.000,	0.159,	0.950,	0.500,	0.000,	1.000,	0.659,	1.000,	0.013,	0.071,	0.000,

		0.232,	0.956,	-0.152,	0.950,	0.500,	0.000,	1.000,	0.348,	0.800,	0.014,	0.074,	0.000,
		0.000,	1.000,	0.159,	0.950,	0.500,	0.000,	1.000,	0.659,	1.000,	0.014,	0.074,	0.000,
		0.000,	1.000,	-0.159,	0.950,	0.500,	0.000,	1.000,	0.348,	1.000,	0.014,	0.074,	0.000
	};

	glGenBuffers(1, &orangeVBO); //make VBO
	glGenVertexArrays(1, &orangeVAO);//make VAO

	glBindVertexArray(orangeVAO); //select VAO will remain selected untill called again

	glBindBuffer(GL_ARRAY_BUFFER, orangeVBO); //select VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes

	// specify vertex array (VA) location and layoit to gpu
	//position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//color data
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	//texture data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(7 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	//normal data
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(9 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0); //deselect VAO
}

void CreateBottleVertices() {

	// vertex data
	GLfloat vertices[] = {
		//position				color							normal data
		//x,	y,		z		r		g		b		a		x		y		z
		0.000,	0.050,	0.000,	0.230,	0.420,	0.000,	0.600,	0.026,	0.883,	-0.000,
		1.670,	0.000,	-0.265,	0.230,	0.420,	0.000,	0.600,	0.026,	0.883,	-0.000,
		1.670,	0.000,	0.265,	0.230,	0.420,	0.000,	0.600,	0.026,	0.883,	-0.000,

		1.670,	0.000,	-0.265,	0.230,	0.420,	0.000,	0.600,	0.053,	-0.069,	0.000,
		1.670,	0.000,	0.265,	0.230,	0.420,	0.000,	0.600,	0.053,	-0.069,	0.000,
		1.800,	0.100,	-0.285,	0.230,	0.420,	0.000,	0.600,	0.053,	-0.069,	0.000,

		1.670,	0.000,	0.265,	0.230,	0.420,	0.000,	0.600,	0.057,	-0.074,	-0.000,
		1.800,	0.100,	-0.285,	0.230,	0.420,	0.000,	0.600,	0.057,	-0.074,	-0.000,
		1.800,	0.100,	0.285,	0.230,	0.420,	0.000,	0.600,	0.057,	-0.074,	-0.000,

		1.800,	0.100,	-0.285,	0.230,	0.420,	0.000,	0.600,	0.057,	-0.043,	0.000,
		1.800,	0.100,	0.285,	0.230,	0.420,	0.000,	0.600,	0.057,	-0.043,	0.000,
		1.875,	0.200,	-0.297,	0.230,	0.420,	0.000,	0.600,	0.057,	-0.043,	0.000,

		1.800,	0.100,	0.285,	0.230,	0.420,	0.000,	0.600,	0.059,	-0.045,	-0.000,
		1.875,	0.200,	-0.297,	0.230,	0.420,	0.000,	0.600,	0.059,	-0.045,	-0.000,
		1.875,	0.200,	0.297,	0.230,	0.420,	0.000,	0.600,	0.059,	-0.045,	-0.000,

		1.875,	0.200,	-0.297,	0.230,	0.420,	0.000,	0.600,	0.059,	-0.033,	0.000,
		1.875,	0.200,	0.297,	0.230,	0.420,	0.000,	0.600,	0.059,	-0.033,	0.000,
		1.930,	0.300,	-0.306,	0.230,	0.420,	0.000,	0.600,	0.059,	-0.033,	0.000,

		1.875,	0.200,	0.297,	0.230,	0.420,	0.000,	0.600,	0.061,	-0.034,	-0.000,
		1.930,	0.300,	-0.306,	0.230,	0.420,	0.000,	0.600,	0.061,	-0.034,	-0.000,
		1.930,	0.300,	0.306,	0.230,	0.420,	0.000,	0.600,	0.061,	-0.034,	-0.000,

		1.930,	0.300,	-0.306,	0.230,	0.420,	0.000,	0.600,	0.076,	-0.021,	0.000,
		1.930,	0.300,	0.306,	0.230,	0.420,	0.000,	0.600,	0.076,	-0.021,	0.000,
		1.965,	0.425,	-0.311,	0.230,	0.420,	0.000,	0.600,	0.076,	-0.021,	0.000,

		1.930,	0.300,	0.306,	0.230,	0.420,	0.000,	0.600,	0.078,	-0.022,	-0.000,
		1.965,	0.425,	-0.311,	0.230,	0.420,	0.000,	0.600,	0.078,	-0.022,	-0.000,
		1.965,	0.425,	0.311,	0.230,	0.420,	0.000,	0.600,	0.078,	-0.022,	-0.000,

		1.965,	0.425,	-0.311,	0.230,	0.420,	0.000,	0.600,	0.140,	-0.022,	0.000,
		1.965,	0.425,	0.311,	0.230,	0.420,	0.000,	0.600,	0.140,	-0.022,	0.000,
		2.000,	0.650,	-0.317,	0.230,	0.420,	0.000,	0.600,	0.140,	-0.022,	0.000,

		1.965,	0.425,	0.311,	0.230,	0.420,	0.000,	0.600,	0.143,	-0.022,	-0.000,
		2.000,	0.650,	-0.317,	0.230,	0.420,	0.000,	0.600,	0.143,	-0.022,	-0.000,
		2.000,	0.650,	0.317,	0.230,	0.420,	0.000,	0.600,	0.143,	-0.022,	-0.000,

		2.000,	0.650,	-0.317,	0.230,	0.420,	0.000,	0.600,	4.783,	0.000,	-0.000,
		2.000,	0.650,	0.317,	0.230,	0.420,	0.000,	0.600,	4.783,	0.000,	-0.000,
		2.000,	8.200,	-0.317,	0.230,	0.420,	0.000,	0.600,	4.783,	0.000,	-0.000,

		2.000,	0.650,	0.317,	0.230,	0.420,	0.000,	0.600,	4.783,	0.000,	-0.000,
		2.000,	8.200,	-0.317,	0.230,	0.420,	0.000,	0.600,	4.783,	0.000,	-0.000,
		2.000,	8.200,	0.317,	0.230,	0.420,	0.000,	0.600,	4.783,	0.000,	-0.000,

		2.000,	8.200,	-0.317,	0.230,	0.420,	0.000,	0.600,	0.241,	0.038,	-0.000,
		2.000,	8.200,	0.317,	0.230,	0.420,	0.000,	0.600,	0.241,	0.038,	-0.000,
		1.940,	8.580,	-0.307,	0.230,	0.420,	0.000,	0.600,	0.241,	0.038,	-0.000,

		2.000,	8.200,	0.317,	0.230,	0.420,	0.000,	0.600,	0.234,	0.037,	-0.000,
		1.940,	8.580,	-0.307,	0.230,	0.420,	0.000,	0.600,	0.234,	0.037,	-0.000,
		1.940,	8.580,	0.307,	0.230,	0.420,	0.000,	0.600,	0.234,	0.037,	-0.000,

		1.940,	8.580,	-0.307,	0.230,	0.420,	0.000,	0.600,	0.175,	0.049,	-0.000,
		1.940,	8.580,	0.307,	0.230,	0.420,	0.000,	0.600,	0.175,	0.049,	-0.000,
		1.860,	8.865,	-0.295,	0.230,	0.420,	0.000,	0.600,	0.175,	0.049,	-0.000,

		1.940,	8.580,	0.307,	0.230,	0.420,	0.000,	0.600,	0.168,	0.047,	-0.000,
		1.860,	8.865,	-0.295,	0.230,	0.420,	0.000,	0.600,	0.168,	0.047,	-0.000,
		1.860,	8.865,	0.295,	0.230,	0.420,	0.000,	0.600,	0.168,	0.047,	-0.000,

		1.860,	8.865,	-0.295,	0.230,	0.420,	0.000,	0.600,	0.180,	0.065,	-0.000,
		1.860,	8.865,	0.295,	0.230,	0.420,	0.000,	0.600,	0.180,	0.065,	-0.000,
		1.750,	9.170,	-0.277,	0.230,	0.420,	0.000,	0.600,	0.180,	0.065,	-0.000,

		1.860,	8.865,	0.295,	0.230,	0.420,	0.000,	0.600,	0.169,	0.061,	-0.000,
		1.750,	9.170,	-0.277,	0.230,	0.420,	0.000,	0.600,	0.169,	0.061,	-0.000,
		1.750,	9.170,	0.277,	0.230,	0.420,	0.000,	0.600,	0.169,	0.061,	-0.000,

		1.750,	9.170,	-0.277,	0.230,	0.420,	0.000,	0.600,	0.150,	0.083,	-0.000,
		1.750,	9.170,	0.277,	0.230,	0.420,	0.000,	0.600,	0.150,	0.083,	-0.000,
		1.600,	9.440,	-0.253,	0.230,	0.420,	0.000,	0.600,	0.150,	0.083,	-0.000,

		1.750,	9.170,	0.277,	0.230,	0.420,	0.000,	0.600,	0.137,	0.076,	-0.000,
		1.600,	9.440,	-0.253,	0.230,	0.420,	0.000,	0.600,	0.137,	0.076,	-0.000,
		1.600,	9.440,	0.253,	0.230,	0.420,	0.000,	0.600,	0.137,	0.076,	-0.000,

		1.600,	9.440,	-0.253,	0.230,	0.420,	0.000,	0.600,	0.132,	0.096,	-0.000,
		1.600,	9.440,	0.253,	0.230,	0.420,	0.000,	0.600,	0.132,	0.096,	-0.000,
		1.410,	9.700,	-0.223,	0.230,	0.420,	0.000,	0.600,	0.132,	0.096,	-0.000,

		1.600,	9.440,	0.253,	0.230,	0.420,	0.000,	0.600,	0.116,	0.085,	-0.000,
		1.410,	9.700,	-0.223,	0.230,	0.420,	0.000,	0.600,	0.116,	0.085,	-0.000,
		1.410,	9.700,	0.223,	0.230,	0.420,	0.000,	0.600,	0.116,	0.085,	-0.000,

		1.410,	9.700,	-0.223,	0.230,	0.420,	0.000,	0.600,	0.100,	0.083,	-0.000,
		1.410,	9.700,	0.223,	0.230,	0.420,	0.000,	0.600,	0.100,	0.083,	-0.000,
		1.225,	9.925,	-0.194,	0.230,	0.420,	0.000,	0.600,	0.100,	0.083,	-0.000,

		1.410,	9.700,	0.223,	0.230,	0.420,	0.000,	0.600,	0.087,	0.072,	-0.000,
		1.225,	9.925,	-0.194,	0.230,	0.420,	0.000,	0.600,	0.087,	0.072,	-0.000,
		1.225,	9.925,	0.194,	0.230,	0.420,	0.000,	0.600,	0.087,	0.072,	-0.000,

		1.225,	9.925,	-0.194,	0.230,	0.420,	0.000,	0.600,	0.074,	0.083,	-0.000,
		1.225,	9.925,	0.194,	0.230,	0.420,	0.000,	0.600,	0.074,	0.083,	-0.000,
		1.010,	10.115,	-0.160,	0.230,	0.420,	0.000,	0.600,	0.074,	0.083,	-0.000,

		1.225,	9.925,	0.194,	0.230,	0.420,	0.000,	0.600,	0.061,	0.069,	-0.000,
		1.010,	10.115,	-0.160,	0.230,	0.420,	0.000,	0.600,	0.061,	0.069,	-0.000,
		1.010,	10.115,	0.160,	0.230,	0.420,	0.000,	0.600,	0.061,	0.069,	-0.000,

		1.010,	10.115,	-0.160,	0.230,	0.420,	0.000,	0.600,	0.069,	0.070,	-0.000,
		1.010,	10.115,	0.160,	0.230,	0.420,	0.000,	0.600,	0.069,	0.070,	-0.000,
		0.790,	10.330,	-0.125,	0.230,	0.420,	0.000,	0.600,	0.069,	0.070,	-0.000,

		1.010,	10.115,	0.160,	0.230,	0.420,	0.000,	0.600,	0.054,	0.055,	-0.000,
		0.790,	10.330,	-0.125,	0.230,	0.420,	0.000,	0.600,	0.054,	0.055,	-0.000,
		0.790,	10.330,	0.125,	0.230,	0.420,	0.000,	0.600,	0.054,	0.055,	-0.000,

		0.790,	10.330,	-0.125,	0.230,	0.420,	0.000,	0.600,	0.035,	0.025,	-0.000,
		0.790,	10.330,	0.125,	0.230,	0.420,	0.000,	0.600,	0.035,	0.025,	-0.000,
		0.690,	10.470,	-0.109,	0.230,	0.420,	0.000,	0.600,	0.035,	0.025,	-0.000,

		0.790,	10.330,	0.125,	0.230,	0.420,	0.000,	0.600,	0.031,	0.022,	-0.000,
		0.690,	10.470,	-0.109,	0.230,	0.420,	0.000,	0.600,	0.031,	0.022,	-0.000,
		0.690,	10.470,	0.109,	0.230,	0.420,	0.000,	0.600,	0.031,	0.022,	-0.000,

		0.690,	10.470,	-0.109,	0.230,	0.420,	0.000,	0.600,	0.036,	0.003,	-0.000,
		0.690,	10.470,	0.109,	0.230,	0.420,	0.000,	0.600,	0.036,	0.003,	-0.000,
		0.675,	10.635,	-0.107,	0.230,	0.420,	0.000,	0.600,	0.036,	0.003,	-0.000,

		0.690,	10.470,	0.109,	0.230,	0.420,	0.000,	0.600,	0.035,	0.003,	-0.000,
		0.675,	10.635,	-0.107,	0.230,	0.420,	0.000,	0.600,	0.035,	0.003,	-0.000,
		0.675,	10.635,	0.107,	0.230,	0.420,	0.000,	0.600,	0.035,	0.003,	-0.000,

		0.675,	10.635,	-0.107,	0.230,	0.420,	0.000,	0.600,	0.399,	0.050,	-0.000,
		0.675,	10.635,	0.107,	0.230,	0.420,	0.000,	0.600,	0.399,	0.050,	-0.000,
		0.440,	12.500,	-0.070,	0.230,	0.420,	0.000,	0.600,	0.399,	0.050,	-0.000,

		0.675,	10.635,	0.107,	0.230,	0.420,	0.000,	0.600,	0.260,	0.033,	-0.000,
		0.440,	12.500,	-0.070,	0.230,	0.420,	0.000,	0.600,	0.260,	0.033,	-0.000,
		0.440,	12.500,	0.070,	0.230,	0.420,	0.000,	0.600,	0.260,	0.033,	-0.000,

		0.440,	12.500,	-0.070,	0.230,	0.420,	0.000,	0.600,	0.237,	0.004,	-0.000,
		0.440,	12.500,	0.070,	0.230,	0.420,	0.000,	0.600,	0.237,	0.004,	-0.000,
		0.410,	14.200,	-0.065,	0.230,	0.420,	0.000,	0.600,	0.237,	0.004,	-0.000,

		0.440,	12.500,	0.070,	0.230,	0.420,	0.000,	0.600,	0.221,	0.004,	-0.000,
		0.410,	14.200,	-0.065,	0.230,	0.420,	0.000,	0.600,	0.221,	0.004,	-0.000,
		0.410,	14.200,	0.065,	0.230,	0.420,	0.000,	0.600,	0.221,	0.004,	-0.000
	};

	glGenBuffers(1, &bottleVBO); //make VBO
	glGenVertexArrays(1, &bottleVAO);//make VAO

	glBindVertexArray(bottleVAO); //select VAO will remain selected untill called again

		glBindBuffer(GL_ARRAY_BUFFER, bottleVBO); //select VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes

		// specify vertex array (VA) location and layoit to gpu
		//position data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//color data
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		//normal data
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT), (GLvoid*)(7 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

	glBindVertexArray(0); //deselect VAO
};

void CreateBackgroundVertices() {
	// vertex data
	GLfloat vertices[] = {
		//x,	 y,		z		r		g		b		a		x		y		z
		10.0,	0.0,	-10.0,	0.8,	0.9,	0.75,	1.0,	0.0,	0.0,	1.0,
		-10.0,	0.0,	-10.0,	0.8,	0.9,	0.75,	1.0,	0.0,	0.0,	1.0,
		10.0,	20.0,	-10.0,	0.8,	0.9,	0.75,	1.0,	0.0,	0.0,	1.0,
		-10.0,	20.0,	-10.0,	0.8,	0.9,	0.75,	1.0,	0.0,	0.0,	1.0
	};

	//index data
	GLubyte indicies[] ={
		//wall
		0,1,2,
		1,2,3,
	};

	glGenBuffers(1, &backgroundVBO); //make VBO
	glGenBuffers(1, &backgroundEBO); //make EBO
	glGenVertexArrays(1, &backgroundVAO);//make VAO

	glBindVertexArray(backgroundVAO); //select VAO will remain selected untill called again

		glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO); //select VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEBO); //select EBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); //load indicies

		// specify vertex array (VA) location and layoit to gpu
		//position data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//color data
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		//normal data
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT), (GLvoid*)(7 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

	glBindVertexArray(0); //deselect VAO
}

void CreateFloorVertices() {
	// vertex data
	GLfloat vertices[] = {
		//position				color							texture maping	normal data
		//x,	y,		z		r		g		b		a		s		t		x		y		z
		10.0,	0.0,	10.0,	0.5,	0.3,	0.0,	1.0,	3.0,	8.0,	0.0,	1.0,	0.0,
		-10.0,	0.0,	10.0,	0.5,	0.3,	0.0,	1.0,	0.0,	8.0,	0.0,	1.0,	0.0,
		10.0,	0.0,	-10.0,	0.5,	0.3,	0.0,	1.0,	3.0,	0.0,	0.0,	1.0,	0.0,
		-10.0,	0.0,	-10.0,	0.5,	0.3,	0.0,	1.0,	0.0,	0.0,	0.0,	1.0,	0.0
	};

	//index data
	GLubyte indicies[] =
	{
		//floor
		0,1,2,
		1,2,3,
	};

	glGenBuffers(1, &floorVBO); //make VBO
	glGenBuffers(1, &floorEBO); //make EBO
	glGenVertexArrays(1, &floorVAO);//make VAO

	glBindVertexArray(floorVAO); //select VAO will remain selected untill called again

		glBindBuffer(GL_ARRAY_BUFFER, floorVBO); //select VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO); //select EBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); //load indicies

		// specify vertex array (VA) location and layoit to gpu
		//position data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//color data
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		//texture data
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(7 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);
		//normal data
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(9 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(3);

	glBindVertexArray(0); //deselect VAO
}

void CreatePaintingVertices() {
	// vertex data
	GLfloat vertices[] = {
		//position				color							texture maping	normal data
		//x,	y,		z		r		g		b		a		s		t		x		y		z
		0.0,	0.0,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	1.0,	0.0,	0.0,	-1.0,//back
		12.0,	0.0,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	-1.0,
		0.0,	16.0,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	0.0,	0.0,	-1.0,
		12.0,	16.0,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	0.0,	-1.0,

		0.0,	0.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.01,	0.99,	0.0,	0.0,	1.0,	//front
		12.0,	0.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.99,	0.99,	0.0,	0.0,	1.0,
		0.0,	16.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.01,	0.01,	0.0,	0.0,	1.0,
		12.0,	16.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.99,	0.01,	0.0,	0.0,	1.0,

		0.0,	0.0,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	1.0,	-1.0,	0.0,	0.0,	//LEFT
		0.0,	16.0,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	-1.0,	0.0,	0.0,
		0.0,	0.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.01,	0.99,	-1.0,	0.0,	0.0,
		0.0,	16.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.01,	0.01,	-1.0,	0.0,	0.0,

		12.0,	0.0,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	//RIGHT
		12.0,	16.0,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	1.0,	0.0,	0.0,
		12.0,	0.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.99,	0.99,	1.0,	0.0,	0.0,
		12.0,	16.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.99,	0.01,	1.0,	0.0,	0.0,

		0.0,	0.0,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	1.0,	0.0,	-1.0,	0.0,	//BOTTOM
		12.0,	0.0,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	-1.0,	0.0,
		0.0,	0.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.01,	0.99,	0.0,	-1.0,	0.0,
		12.0,	0.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.99,	0.99,	0.0,	-1.0,	0.0,

		0.0,	16.0,	0.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	0.0,	1.0,	0.0,	//TOP
		12.0,	16.0,	0.0,	1.0,	1.0,	1.0,	1.0,	1.0,	0.0,	0.0,	1.0,	0.0,
		0.0,	16.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.01,	0.01,	0.0,	1.0,	0.0,
		12.0,	16.0,	0.5,	1.0,	1.0,	1.0,	1.0,	0.99,	0.01,	0.0,	1.0,	0.0,





	};

	//index data
	GLubyte indicies[] =
	{
		////back
		0,1,2,
		1,2,3,
		//front
		4,5,6,
		5,6,7,
		//left
		8,9,10,
		9,10,11,
		//right
		12,13,14,
		13,14,15,
		//bottom
		 16,17,18,
		 17,18,19,
		//top
		20,21,22,
		21,22,23
	};

	glGenBuffers(1, &paintingVBO); //make VBO
	glGenBuffers(1, &paintingEBO); //make EBO
	glGenVertexArrays(1, &paintingVAO);//make VAO

	glBindVertexArray(paintingVAO); //select VAO will remain selected untill called again

	glBindBuffer(GL_ARRAY_BUFFER, paintingVBO); //select VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, paintingEBO); //select EBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); //load indicies

	// specify vertex array (VA) location and layoit to gpu
	//position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//color data
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	//texture data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(7 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	//NORMAL data
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(9 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0); //deselect VAO
}

void CreatePyramidVertices() {
	// vertex data
	GLfloat vertices[] = {
		//position				color							texture maping	normals
		//x,	y,		z		r		g		b		a		u		v		x		y		z
		//triangle 1 base
		1.0,	0.0,	0.0,	0.6,	0.3,	0.0,	1.0,	1.0,	0.5,	0.0f,	-1.0f,	0.0f,
		0.0,	0.0,	1.0,	0.6,	0.3,	0.0,	1.0,	0.0,	0.5,	0.0f,	-1.0f,	0.0f,
		0.0,	0.0,	0.0,	0.6,	0.3,	0.0,	1.0,	0.5,	1.0,	0.0f,	-1.0f,	0.0f,

		//triangle 2 side
		1.0,	0.0,	0.0,	0.0,	1.0,	0.0,	1.0,	1.0,	0.5,	1.0f,	1.0f,	1.0f,
		0.0,	1.0,	0.0,	1.0,	0.8,	0.0,	1.0,	0.5,	0.0,	1.0f,	1.0f,	1.0f,
		0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.5,	1.0f,	1.0f,	1.0f
	};

	glGenBuffers(1, &pyramidVBO); //make VBO
	glGenVertexArrays(1, &pyramidVAO);//make VAO

	glBindVertexArray(pyramidVAO); //select VAO will remain selected untill called again

	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO); //select VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes

	// specify vertex array (VA) location and layoit to gpu
	//position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//color data
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	//texture data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(7 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	//normal data
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GL_FLOAT), (GLvoid*)(9 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0); //deselect VAO
}

void CreateLightCubeVertices() {

	GLfloat vertices[] = {
		//x		y		z
		-0.5,	-0.5,	0.0,
		-0.5,	0.5,	0.0,
		0.5,	-0.5,	0.0,
		0.5,	0.5,	0.0
	};

	//index data
	GLubyte indicies[] = {
		0,1,2,
		1,2,3
	};

	glGenBuffers(1, &lightCubeVBO); //make VBO
	glGenBuffers(1, &lightCubeEBO); //make EBO
	glGenVertexArrays(1, &lightCubeVAO);//make VAO

	glBindVertexArray(lightCubeVAO); //select VAO will remain selected untill called again

	glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO); //select VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightCubeEBO); //select EBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //load vertex atributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); //load indicies

	// specify vertex array (VA) location and layoit to gpu
	//position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); //deselect VAO
}

void CreateVertices() {
	CreateLightCubeVertices();
	CreatePyramidVertices();
	CreateTapeVertices();
	CreateOrangeVertices();
	CreatePaintingVertices(); 
	CreateBackgroundVertices();
	CreateFloorVertices(); 
	CreateBottleVertices();
}

/**load texture functions**/
void LoadPyramidTexture() {
	int pyramidTexWidth, pyramidTexHeight;
	unsigned char* pyrmidImage = SOIL_load_image("pine tree texture.png", &pyramidTexWidth, &pyramidTexHeight, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &pyramidTexture);
	glBindTexture(GL_TEXTURE_2D, pyramidTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pyramidTexWidth, pyramidTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pyrmidImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(pyrmidImage); //free image to save memory
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadOrangeTexture() {
	int orangeTexWidth, orangeTexHeight;
	unsigned char* orangeImage = SOIL_load_image("OrangeTexture.png", &orangeTexWidth, &orangeTexHeight, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &orangeTexture);
	glBindTexture(GL_TEXTURE_2D, orangeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, orangeTexWidth, orangeTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, orangeImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(orangeImage); //free image to save memory
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadPaintingTexture() {
	int paintingTexWidth, paintingTexHeight;
	unsigned char* paintingImage = SOIL_load_image("PaintingTexture.jpg", &paintingTexWidth, &paintingTexHeight, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &paintingTexture);
	glBindTexture(GL_TEXTURE_2D, paintingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, paintingTexWidth, paintingTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, paintingImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(paintingImage); //free image to save memory
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadTapeTexture() {
	int tapeTexWidth, tapeTexHeight;
	unsigned char* tapeImage = SOIL_load_image("tapeTexture.png", &tapeTexWidth, &tapeTexHeight, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &tapeTexture);
	glBindTexture(GL_TEXTURE_2D, tapeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tapeTexWidth, tapeTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tapeImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(tapeImage); //free image to save memory
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadFloorTexture() {
	int floorTexWidth, floorTexHeight;
	unsigned char* floorImage = SOIL_load_image("FLOOR BOARD.png", &floorTexWidth, &floorTexHeight, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floorTexWidth, floorTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, floorImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(floorImage); //free image to save memory
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadTextures() {
	LoadPyramidTexture();
	LoadOrangeTexture();
	LoadTapeTexture();
	LoadPaintingTexture();
	LoadFloorTexture();
}

/** define input callback functions **/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Display ASCII keyvode
	//cout << "ASCII: " << key << endl;

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	/*scroll to change wasdqe pan speed*/
	panSpeed += yoffset * panAcceleration;
	if (panSpeed < 0)panSpeed = 0;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//display mouce x and y
	//cout << "xpos: " << xpos << "\nypos: " << ypos << endl;

	if (not firstMouseMove) {
		lastX = xpos;
		lastY = ypos;
		firstMouseMove = true;
	}

	// calculate mouse currsor offset
	xOffset = -(xpos - lastX);
	yOffset = -(ypos - lastY); //inverted vertical panning

	lastX = xpos;
	lastY = ypos;

	//pan camera if is panning
	if (isPanning)
	{
		GLfloat cameraSpeed = xOffset * deltaTime;
		cameraPosition += cameraSpeed * cameraRight;
		cameraTarget += cameraSpeed * cameraRight;

		cameraSpeed = yOffset * deltaTime;
		cameraPosition += cameraSpeed * cameraUp;
		cameraTarget += cameraSpeed * cameraUp;
	}

	//orbit camera im pretty sure all the rad and degrees are backwards here
	if (isOrbiting) {
		GLfloat sensitivity = 0.1f; // sets sensitivity of orbital movement
		degreesYaw += xOffset * sensitivity;
		degreesPitch += yOffset * sensitivity;

		// clamps pitch -89 to 89
		degreesYaw = glm::mod(degreesYaw, 360.0f);
		degreesPitch = glm::clamp(degreesPitch, -89.0f, 89.0f);

		//convert yaw and pitch to degrees
		radiansYaw = glm::radians(degreesYaw);
		radiansPitch = glm::radians(degreesPitch);

		//calculate camera direction with azimuth and altitude formula then normilize
		cameraDirection.x = cosf(radiansPitch) * sinf(radiansYaw);
		cameraDirection.y = -sinf(radiansPitch);
		cameraDirection.z = cosf(radiansPitch) * cosf(radiansYaw);
		cameraDirection = glm::normalize(cameraDirection);

		//calculate target, right, and up vectors based on direction and position
		cameraTarget = cameraDirection + cameraPosition;
		cameraRight = -glm::normalize(glm::cross(worldUp, cameraDirection));
		cameraUp = -glm::normalize(glm::cross(cameraDirection, cameraRight));
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	////display if left, center, and right mouce button is clicked
	//if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	//	cout << "left mouce button clicked" << endl;
	//if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	//	cout << "middle mouce button clicked" << endl;
	//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	//	cout << "right mouce button clicked" << endl;

	if (action == GLFW_PRESS)
		mouseButtons[button] = true;
	else if (action == GLFW_RELEASE)
		mouseButtons[button] = false;
}

/** define Camera Function and input functions **/
void TransormCamera() {
	// pan camera if left alt and middle mouce button are pressed
	if (keys[GLFW_KEY_LEFT_ALT] && mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE])
		isPanning = true;
	else
		isPanning = false;

	// orbit camera if left alt and left mouce button or right mouse button are pressed
	if (keys[GLFW_KEY_LEFT_ALT] && mouseButtons[GLFW_MOUSE_BUTTON_LEFT] || mouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
		isOrbiting = true;
	else
		isOrbiting = false;

	processKeyboardInput();
}

void processKeyboardInput() {

	//reset camera
	if (keys[GLFW_KEY_F])
		initCamera();
	//wasdqe
	GLfloat cameraSpeed = panSpeed * deltaTime;
	if (keys[GLFW_KEY_W]) {
		cameraPosition += cameraSpeed * cameraDirection;
		cameraTarget += cameraSpeed * cameraDirection;
	}
	if (keys[GLFW_KEY_S]) {
		cameraPosition -= cameraSpeed * cameraDirection;
		cameraTarget - cameraSpeed * cameraDirection;
	}
	if (keys[GLFW_KEY_A]) {
		cameraPosition -= cameraSpeed * cameraRight;
		cameraTarget -= cameraSpeed * cameraRight;
	}
	if (keys[GLFW_KEY_D]) {
		cameraPosition += cameraSpeed * cameraRight;
		cameraTarget += cameraSpeed * cameraRight;
	}
	if (keys[GLFW_KEY_Q]) {
		cameraPosition += cameraSpeed * cameraUp;
		cameraTarget += cameraSpeed * cameraUp;
	}
	if (keys[GLFW_KEY_E]) {
		cameraPosition -= cameraSpeed * cameraUp;
		cameraTarget -= cameraSpeed * cameraUp;
	}
	//change perspective with p
	if (keys[GLFW_KEY_P])
		displayOrtho = !displayOrtho;
	//press T to togel tree mode for pyrmid
	if (keys[GLFW_KEY_T])
		treeMode = !treeMode;
	//press L to togel disco mode
	if (keys[GLFW_KEY_L])
		discoMode = !discoMode;
}

void initCamera() {
	cameraPosition = glm::vec3(0.0f, 5.0f, 20.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(cameraTarget - cameraPosition);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(cameraDirection, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
	cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)); //define camera movement increment length?

	//reset angel variables
	radius = 5.0f;
	radiansYaw = atan2(cameraDirection.x, cameraDirection.z);
	radiansPitch = -asinf(cameraDirection.y);
	degreesYaw = glm::degrees(radiansYaw);
	degreesPitch = glm::degrees(radiansPitch);
}

void printCamera() {
	//viewMatrix
	cout << glm::to_string(viewMatrix) << endl;
	cout << glm::to_string(viewMatrix[0]) << endl;
	cout << glm::to_string(viewMatrix[1]) << endl;
	cout << glm::to_string(viewMatrix[2]) << endl;
	cout << glm::to_string(viewMatrix[3]) << endl;

	//camera variables
	cout << "cameraPosition = " << glm::to_string(cameraPosition) << endl;
	cout << "cameraTarget = " << glm::to_string(cameraTarget) << endl;
	cout << "cameraDirection = " << glm::to_string(cameraDirection) << endl;
	cout << "worldUp = " << glm::to_string(worldUp) << endl;
	cout << "cameraRight = " << glm::to_string(cameraRight) << endl;
	cout << "cameraUp  = " << glm::to_string(cameraUp) << endl;
	cout << "cameraFront = " << glm::to_string(cameraFront) << endl;

	//angel variables
	cout << "radius = " << radius << endl;
	cout << "degreesYaw = " << degreesYaw << endl;
	cout << "degreesPitch = " << degreesPitch << endl;
	cout << "radiansYaw = " << radiansYaw << endl;
	cout << "radiansPitch = " << radiansPitch << endl;

	cout << "--------------------------------------------------" << endl;
}

int main(void) {

	GLFWwindow* window; //reate the window that willbe used for render the scene

	initCamera(); // initilize the camera
	IncrementLightPosition(); //initilize light position

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(width, height, "Definitly not a christmass tree", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// set imput allback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
		cout << "Error!" << endl;

	//setting global varibales in openGL
	glEnable(GL_DEPTH_TEST); //enable depth buffer
	glEnable(GL_BLEND);	//enable blending for partially opaque glass
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //sets how blending works

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wire frame mode for debugging and developement

	//creating and loading data to be used for rendering objecs
	LoadTextures(); //loading textures
	CreateShaders();//creating shaders
	CreateVertices();// Generate vertices, creat buffers, load vertices to VBOs and indicies to EBOs

	//set background color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);

	while (!glfwWindowShouldClose(window)) {
		//set Delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//RESIZEING WINDOW and graphics
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//create transforms
		viewMatrix = glm::lookAt(cameraPosition, cameraTarget, worldUp);

		if (displayOrtho)
			projectionMatrix = glm::ortho(-(GLfloat)width / 100, (GLfloat)width / 100, -(GLfloat)height / 100, (GLfloat)height / 100, 1.0f, 100.0f);
		else
			projectionMatrix = glm::perspective(fov * toRadians, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); //fov is global zoom

		draw();

		glfwSwapBuffers(window);

		//poll and process events
		glfwPollEvents();

		//poll camera transformation
		TransormCamera();

		//move lights
		IncrementLightPosition();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}