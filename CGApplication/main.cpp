#include <iostream>
#include <fstream>
#include <vector>
#include "shader.h"
#include "line.h"
#include "circle.h"
#include "windowSetting.h"
using namespace std;

#define GLEW_STATIC
#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

const GLfloat PI = 3.14159f;
//GLfloat vertics[] = {
//	10.0f / WIDTH, 20.0f / HEIGHT, 0.0f, 1.0f, 0.0f, 0.0f,
//	-50.0f / WIDTH, 30.0f / HEIGHT, 0.0f, 0.0f, 1.0f, 0.0f,
//	30.0f / WIDTH, -30.0f / HEIGHT, 0.0f, 0.0f, 0.0f, 1.0f
//};

GLuint initVAO();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
glm::mat4 getTransformMatrix();

int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//set shader program
	GLuint myShaderProgram;
	shader *myShader = new shader("first.vert", "first.frag");
	myShaderProgram = myShader->shaderProgram;

	//set VAO
	GLuint myVAO;
	myVAO = initVAO();

	//set transform
	GLuint transformLocation = glGetUniformLocation(myShader->shaderProgram, "transform");
	glm::mat4 transformMat = getTransformMatrix();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(myShaderProgram);
		glBindVertexArray(myVAO);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
		glDrawArrays(GL_POINTS, 0, 651);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	//clean all resources
	glDeleteVertexArrays(1, &myVAO);
	glfwTerminate();
	return 0;
}


GLuint initVAO()
{
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//set vertex buffer and attribute point
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*line myLine(glm::ivec3(200, 200, 0), glm::ivec3(-200, -200, 0), glm::vec3(1.0f, 0.0f, 0.0f));
	myLine.lineUseBresenham();
	myLine.lineUseDDA();
	glBufferData(GL_ARRAY_BUFFER,myLine.getPointsNum()*myLine.getPointSize(), 
		&(myLine.getLineVertics())[0], GL_STATIC_DRAW);*/
	circle myCircle(glm::ivec3(0,0,0),115,glm::vec3(1.0f,0.0f,0.0f));
	myCircle.circleUseMidpoint();
	glBufferData(GL_ARRAY_BUFFER, myCircle.getPointsNum()*myCircle.getPointSize(),
		&(myCircle.getCircleVertics())[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//unbind VBO&VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE&&action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
glm::mat4 getTransformMatrix()
{
	glm::mat4 transformMat;
	//transformMat = glm::rotate(transformMat,(GLfloat)glfwGetTime()*PI/2, glm::vec3(0, 0, 1));
	transformMat = glm::scale(transformMat, glm::vec3(1.0,1.0,1.0));
	return transformMat;
}